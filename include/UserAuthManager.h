#pragma once

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class UserAuthManager {
public:
    static UserAuthManager& instance();
    
    // User operations
    bool registerUser(const QString& username, const QString& password, QString& errorMsg);
    bool login(const QString& username, const QString& password, QString& errorMsg);
    bool changePassword(const QString& username, const QString& oldPassword, const QString& newPassword, QString& errorMsg);
    bool userExists(const QString& username) const;
    
    // Data persistence
    bool loadUsers();
    bool saveUsers();
    
    // Password hashing
    QString hashPassword(const QString& password, const QString& salt = QString()) const;
    bool verifyPassword(const QString& password, const QString& hash, const QString& salt) const;
    QString generateSalt() const;
    
private:
    UserAuthManager();
    ~UserAuthManager() = default;
    UserAuthManager(const UserAuthManager&) = delete;
    UserAuthManager& operator=(const UserAuthManager&) = delete;
    
    QString usersFilePath_;
    QJsonArray usersArray_;
    bool usersLoaded_;
    
    QString findUserHash(const QString& username, QString& salt) const;
};

