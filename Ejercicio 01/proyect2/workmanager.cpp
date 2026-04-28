#include "workmanager.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QUuid>

WorkManager::WorkManager() {
    dataPath = QCoreApplication::applicationDirPath() + "/data";
    worksFilePath = dataPath + "/works.json";
    ensureDataDirectory();
    loadWorks();
}

QList<Work> WorkManager::getAllWorks() const {
    return works;
}

Work WorkManager::getWorkById(const QString& id) const {
    for (const Work& work : works) {
        if (work.id == id) {
            return work;
        }
    }

    return Work();
}

QString WorkManager::addWork(const QString& title, const QString& description,
                             const QString& priority, const QDateTime& dueDate,
                             const QString& createdBy) {
    Work work;
    work.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    work.title = title;
    work.description = description;
    work.status = "Pendiente";
    work.priority = priority;
    work.dueDate = dueDate;
    work.createdDate = QDateTime::currentDateTime();
    work.createdBy = createdBy;
    work.notes = QString();

    works.append(work);
    saveWorks();
    return work.id;
}

bool WorkManager::updateWork(const QString& id, const Work& updatedWork) {
    for (int i = 0; i < works.size(); ++i) {
        if (works[i].id == id) {
            works[i] = updatedWork;
            works[i].id = id;
            saveWorks();
            return true;
        }
    }

    return false;
}

bool WorkManager::deleteWork(const QString& id) {
    for (int i = 0; i < works.size(); ++i) {
        if (works[i].id == id) {
            works.removeAt(i);
            saveWorks();
            return true;
        }
    }

    return false;
}

bool WorkManager::updateWorkNotes(const QString& id, const QString& notes) {
    for (Work& work : works) {
        if (work.id == id) {
            work.notes = notes;
            saveWorks();
            return true;
        }
    }

    return false;
}

void WorkManager::loadWorks() {
    works.clear();

    QFile file(worksFilePath);
    if (!file.exists()) {
        saveWorks();
        return;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!doc.isArray()) {
        return;
    }

    const QJsonArray array = doc.array();
    for (const QJsonValue& value : array) {
        if (value.isObject()) {
            works.append(jsonToWork(value.toObject()));
        }
    }
}

void WorkManager::saveWorks() const {
    QJsonArray array;
    for (const Work& work : works) {
        array.append(workToJson(work));
    }

    const QJsonDocument doc(array);
    QFile file(worksFilePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

void WorkManager::ensureDataDirectory() const {
    QDir dir(dataPath);
    if (!dir.exists()) {
        dir.mkpath(dataPath);
    }
}

QJsonObject WorkManager::workToJson(const Work& work) const {
    QJsonObject obj;
    obj["id"] = work.id;
    obj["title"] = work.title;
    obj["description"] = work.description;
    obj["status"] = work.status;
    obj["priority"] = work.priority;
    obj["dueDate"] = work.dueDate.toString(Qt::ISODate);
    obj["createdDate"] = work.createdDate.toString(Qt::ISODate);
    obj["createdBy"] = work.createdBy;
    obj["notes"] = work.notes;
    return obj;
}

Work WorkManager::jsonToWork(const QJsonObject& json) const {
    Work work;
    work.id = json.value("id").toString();
    work.title = json.value("title").toString();
    work.description = json.value("description").toString();
    work.status = json.value("status").toString("Pendiente");
    work.priority = json.value("priority").toString("Normal");
    work.dueDate = QDateTime::fromString(json.value("dueDate").toString(), Qt::ISODate);
    work.createdDate = QDateTime::fromString(json.value("createdDate").toString(), Qt::ISODate);
    work.createdBy = json.value("createdBy").toString();
    work.notes = json.value("notes").toString();
    return work;
}