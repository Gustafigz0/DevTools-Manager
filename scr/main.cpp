#include <QApplication>
#include "MainWindow.h"
#include "UserAuthDialog.h"
#include "UserAuthManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Initialize auth manager (loads users from JSON)
    UserAuthManager::instance().loadUsers();

    QString loggedInUser;
    bool authenticated = false;

    // Show login dialog
    while (!authenticated) {
        UserAuthDialog loginDialog(UserAuthDialog::Login);
        
        if (loginDialog.exec() == QDialog::Accepted) {
            loggedInUser = loginDialog.username();
            authenticated = !loggedInUser.isEmpty();
        } else {
            // User cancelled login
            return 0;
        }
    }

    // Show main window after successful login
    MainWindow w(loggedInUser);
    w.show();

    return app.exec();
}
