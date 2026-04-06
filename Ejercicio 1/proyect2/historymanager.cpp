#include "historymanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QCoreApplication>
#include <QDir>
#include <algorithm>

HistoryManager::HistoryManager() {
    dataPath = QCoreApplication::applicationDirPath() + "/data";
    historyFilePath = dataPath + "/history.json";

    ensureDataDirectory();
    loadHistory();
}

void HistoryManager::ensureDataDirectory() {
    QDir dir(dataPath);
    if (!dir.exists()) {
        dir.mkpath(dataPath);
    }
}

void HistoryManager::loadHistory() {
    QFile file(historyFilePath);
    history.clear();

    if (!file.exists()) {
        return;
    }

    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (doc.isArray()) {
            QJsonArray array = doc.array();
            for (const auto& entryVal : array) {
                history.append(jsonToEntry(entryVal.toObject()));
            }
        }
        file.close();
    }
}

QList<HistoryEntry> HistoryManager::getHistory() const {
    QList<HistoryEntry> sorted = history;
    std::sort(sorted.begin(), sorted.end(),
              [](const HistoryEntry& a, const HistoryEntry& b) {
                  return a.timestamp > b.timestamp;
              });
    return sorted;
}

QList<HistoryEntry> HistoryManager::getHistoryByUser(const QString& username) const {
    QList<HistoryEntry> filtered;
    for (const auto& entry : history) {
        if (entry.username == username) {
            filtered.append(entry);
        }
    }
    return filtered;
}

QList<HistoryEntry> HistoryManager::getHistoryByActionType(const QString& actionType) const {
    QList<HistoryEntry> filtered;
    for (const auto& entry : history) {
        if (entry.actionType == actionType) {
            filtered.append(entry);
        }
    }
    return filtered;
}

QList<HistoryEntry> HistoryManager::getHistoryByWorkId(const QString& workId) const {
    QList<HistoryEntry> filtered;
    for (const auto& entry : history) {
        if (entry.workId == workId) {
            filtered.append(entry);
        }
    }
    return filtered;
}

void HistoryManager::addEntry(const QString& username, const QString& actionType,
                              const QString& description, const QString& workId) {
    HistoryEntry entry;
    entry.timestamp = QDateTime::currentDateTime();
    entry.username = username;
    entry.actionType = actionType;
    entry.description = description;
    entry.workId = workId;

    history.append(entry);
    saveHistory();
}

void HistoryManager::clearHistory() {
    history.clear();
    saveHistory();
}

QJsonObject HistoryManager::entryToJson(const HistoryEntry& entry) const {
    QJsonObject obj;
    obj["timestamp"] = entry.timestamp.toString(Qt::ISODate);
    obj["username"] = entry.username;
    obj["actionType"] = entry.actionType;
    obj["description"] = entry.description;
    obj["workId"] = entry.workId;
    return obj;
}

HistoryEntry HistoryManager::jsonToEntry(const QJsonObject& json) const {
    HistoryEntry entry;
    entry.timestamp = QDateTime::fromString(json["timestamp"].toString(), Qt::ISODate);
    entry.username = json["username"].toString();
    entry.actionType = json["actionType"].toString();
    entry.description = json["description"].toString();
    entry.workId = json["workId"].toString();
    return entry;
}

void HistoryManager::saveHistory() {
    QJsonArray array;
    for (const auto& entry : history) {
        array.append(entryToJson(entry));
    }

    QJsonDocument doc(array);
    QFile file(historyFilePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}