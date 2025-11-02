#include "UserAuthManager.h"
#include <QFile>
#include <QDir>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QDebug>

UserAuthManager::UserAuthManager() 
    : usersFilePath_("dados/users.json"), usersLoaded_(false)
{
    loadUsers();
}

UserAuthManager& UserAuthManager::instance() {
    static UserAuthManager inst;
    return inst;
}

bool UserAuthManager::loadUsers() {
    QFile file(usersFilePath_);
    if (!file.exists()) {
        // Create directory if it doesn't exist
        QDir dir;
        dir.mkpath("dados");
        usersArray_ = QJsonArray();
        usersLoaded_ = true;
        return true;
    }
    
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open users file for reading";
        usersArray_ = QJsonArray();
        usersLoaded_ = true;
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        qDebug() << "Invalid JSON in users file";
        usersArray_ = QJsonArray();
        usersLoaded_ = true;
        return false;
    }
    
    QJsonObject root = doc.object();
    usersArray_ = root["users"].toArray();
    usersLoaded_ = true;
    
    qDebug() << "Loaded" << usersArray_.size() << "users";
    return true;
}

bool UserAuthManager::saveUsers() {
    QDir dir;
    dir.mkpath("dados");
    
    QFile file(usersFilePath_);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open users file for writing";
        return false;
    }
    
    QJsonObject root;
    root["users"] = usersArray_;
    
    QJsonDocument doc(root);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    
    qDebug() << "Saved" << usersArray_.size() << "users";
    return true;
}

QString UserAuthManager::generateSalt() const {
    const QString chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    QString salt;
    salt.reserve(32);
    for (int i = 0; i < 32; ++i) {
        salt.append(chars.at(QRandomGenerator::global()->bounded(chars.length())));
    }
    return salt;
}

QString UserAuthManager::hashPassword(const QString& password, const QString& salt) const {
    QString saltedPassword = password + salt;
    QByteArray hash = QCryptographicHash::hash(
        saltedPassword.toUtf8(), 
        QCryptographicHash::Sha256
    );
    return hash.toHex();
}

bool UserAuthManager::verifyPassword(const QString& password, const QString& hash, const QString& salt) const {
    QString computedHash = hashPassword(password, salt);
    return computedHash == hash;
}

QString UserAuthManager::findUserHash(const QString& username, QString& salt) const {
    for (const QJsonValue& value : usersArray_) {
        QJsonObject user = value.toObject();
        if (user["username"].toString() == username) {
            salt = user["salt"].toString();
            return user["password_hash"].toString();
        }
    }
    return QString();
}

bool UserAuthManager::userExists(const QString& username) const {
    QString dummy;
    return !findUserHash(username, dummy).isEmpty();
}

bool UserAuthManager::registerUser(const QString& username, const QString& password, QString& errorMsg) {
    if (!usersLoaded_) {
        errorMsg = "User database not loaded";
        return false;
    }
    
    QString trimmedUsername = username.trimmed();
    if (trimmedUsername.isEmpty()) {
        errorMsg = "Username cannot be empty";
        return false;
    }
    
    if (password.isEmpty()) {
        errorMsg = "Password cannot be empty";
        return false;
    }
    
    if (userExists(trimmedUsername)) {
        errorMsg = "Username already exists";
        return false;
    }
    
    QString salt = generateSalt();
    QString hash = hashPassword(password, salt);
    
    QJsonObject newUser;
    newUser["username"] = trimmedUsername;
    newUser["password_hash"] = hash;
    newUser["salt"] = salt;
    
    usersArray_.append(newUser);
    
    if (!saveUsers()) {
        errorMsg = "Failed to save user data";
        return false;
    }
    
    qDebug() << "Registered user:" << trimmedUsername;
    return true;
}

bool UserAuthManager::login(const QString& username, const QString& password, QString& errorMsg) {
    if (!usersLoaded_) {
        errorMsg = "User database not loaded";
        return false;
    }
    
    QString trimmedUsername = username.trimmed();
    if (trimmedUsername.isEmpty()) {
        errorMsg = "Username cannot be empty";
        return false;
    }
    
    QString salt;
    QString hash = findUserHash(trimmedUsername, salt);
    
    if (hash.isEmpty()) {
        errorMsg = "Invalid username or password";
        return false;
    }
    
    // Handle legacy bcrypt format (migration)
    if (hash.startsWith("$2b$") || hash.startsWith("$2a$") || hash.startsWith("$2y$")) {
        // Legacy bcrypt hash - for migration, we'll allow creation of new accounts
        // or the user can change password to migrate to new format
        errorMsg = "Legacy password format detected. Please change your password or create a new account.";
        return false;
    }
    
    if (!verifyPassword(password, hash, salt)) {
        errorMsg = "Invalid username or password";
        return false;
    }
    
    qDebug() << "Login successful for user:" << trimmedUsername;
    return true;
}

bool UserAuthManager::changePassword(const QString& username, const QString& oldPassword, const QString& newPassword, QString& errorMsg) {
    if (!usersLoaded_) {
        errorMsg = "User database not loaded";
        return false;
    }
    
    QString trimmedUsername = username.trimmed();
    if (!userExists(trimmedUsername)) {
        errorMsg = "User does not exist";
        return false;
    }
    
    if (newPassword.isEmpty()) {
        errorMsg = "New password cannot be empty";
        return false;
    }
    
    QString salt;
    QString hash = findUserHash(trimmedUsername, salt);
    
    // Verify old password
    if (!hash.startsWith("$2b$") && !hash.startsWith("$2a$") && !hash.startsWith("$2y$")) {
        // New format
        if (!verifyPassword(oldPassword, hash, salt)) {
            errorMsg = "Current password is incorrect";
            return false;
        }
    } else {
        // Legacy format - just allow password change
        errorMsg = "Legacy password format. Please create a new account or contact administrator.";
        return false;
    }
    
    // Update password
    QString newSalt = generateSalt();
    QString newHash = hashPassword(newPassword, newSalt);
    
    for (int i = 0; i < usersArray_.size(); ++i) {
        QJsonObject user = usersArray_[i].toObject();
        if (user["username"].toString() == trimmedUsername) {
            user["password_hash"] = newHash;
            user["salt"] = newSalt;
            usersArray_[i] = user;
            break;
        }
    }
    
    if (!saveUsers()) {
        errorMsg = "Failed to save user data";
        return false;
    }
    
    qDebug() << "Password changed for user:" << trimmedUsername;
    return true;
}

