#ifndef HISTORYMANAGER_H
#define HISTORYMANAGER_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QList>

struct HistoryEntry {
    QDateTime timestamp;
    QString username;
    QString actionType;
    QString description;
    QString workId;
};

class HistoryManager {
public:
    HistoryManager();

    void loadHistory();
    QList<HistoryEntry> getHistory() const;
    QList<HistoryEntry> getHistoryByUser(const QString& username) const;
    QList<HistoryEntry> getHistoryByActionType(const QString& actionType) const;
    QList<HistoryEntry> getHistoryByWorkId(const QString& workId) const;
    void addEntry(const QString& username, const QString& actionType,
                  const QString& description, const QString& workId = "");
    void clearHistory();

private:
    QString historyFilePath;
    QString dataPath;
    QList<HistoryEntry> history;

    QJsonObject entryToJson(const HistoryEntry& entry) const;
    HistoryEntry jsonToEntry(const QJsonObject& json) const;
    void saveHistory();
    void ensureDataDirectory();
};

#endif // HISTORYMANAGER_H