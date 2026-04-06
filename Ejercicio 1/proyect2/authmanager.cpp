#include "authmanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>
#include <QDir>

AuthManager::AuthManager() {
    dataPath = QCoreApplication::applicationDirPath() + "/data";
    usersFilePath = dataPath + "/users.json";
    sessionFilePath = dataPath + "/session.json";

    ensureDataDirectory();
    loadUsers();
}

void AuthManager::ensureDataDirectory() {
    QDir dir(dataPath);
    if (!dir.exists()) {
        dir.mkpath(dataPath);
    }
}

void AuthManager::loadUsers() {
    QFile file(usersFilePath);

    if (!file.exists()) {
        // Crear archivo con usuario por defecto
        users = QJsonArray();
        QJsonObject defaultUser;
        defaultUser["username"] = "admin";
        defaultUser["password"] = "admin";
        users.append(defaultUser);

        QJsonObject defaultUser2;
        defaultUser2["username"] = "estudiante1";
        defaultUser2["password"] = "password123";
        users.append(defaultUser2);

        saveUsers();
        return;
    }

    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (doc.isObject()) {
            users = doc.object()["users"].toArray();
        }
        file.close();
    }
}

void AuthManager::saveUsers() {
    QJsonObject root;
    root["users"] = users;
    QJsonDocument doc(root);

    QFile file(usersFilePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

bool AuthManager::authenticate(const QString& username, const QString& password) {
    for (const auto& userVal : users) {
        QJsonObject userObj = userVal.toObject();
        if (userObj["username"].toString() == username &&
            userObj["password"].toString() == password) {
            return true;
        }
    }
    return false;
}

void AuthManager::saveSession(const QString& username) {
    QJsonObject session;
    session["username"] = username;
    session["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    QJsonDocument doc(session);
    QFile file(sessionFilePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

QString AuthManager::getActiveSession() {
    QFile file(sessionFilePath);
    if (!file.exists()) {
        return QString();
    }

    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (doc.isObject()) {
            QJsonObject session = doc.object();
            QDateTime sessionTime = QDateTime::fromString(
                session["timestamp"].toString(), Qt::ISODate);

            if (isSessionValid(sessionTime)) {
                return session["username"].toString();
            }
        }
        file.close();
    }

    logout();

    return QString();
}

void AuthManager::logout() {
    QFile file(sessionFilePath);
    if (file.exists()) {
        file.remove();
    }
}

bool AuthManager::createUser(const QString& username, const QString& password) {
    // Verificar si usuario ya existe
    for (const auto& userVal : users) {
        QJsonObject userObj = userVal.toObject();
        if (userObj["username"].toString() == username) {
            return false;
        }
    }

    // Crear nuevo usuario
    QJsonObject newUser;
    newUser["username"] = username;
    newUser["password"] = password;
    users.append(newUser);

    saveUsers();
    return true;
}

bool AuthManager::isSessionValid(const QDateTime& sessionTime) {
    QDateTime now = QDateTime::currentDateTime();
    qint64 diffMinutes = sessionTime.msecsTo(now) / 60000;
    return diffMinutes < 5;
}