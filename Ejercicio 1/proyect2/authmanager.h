#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>

class AuthManager {
public:
    AuthManager();

    void loadUsers();
    bool authenticate(const QString& username, const QString& password);
    void saveSession(const QString& username);
    QString getActiveSession();
    void logout();
    bool createUser(const QString& username, const QString& password);

private:
    QString usersFilePath;
    QString sessionFilePath;
    QString dataPath;

    QJsonArray users;

    bool isSessionValid(const QDateTime& sessionTime);
    void saveUsers();
    void ensureDataDirectory();
};

#endif // AUTHMANAGER_H