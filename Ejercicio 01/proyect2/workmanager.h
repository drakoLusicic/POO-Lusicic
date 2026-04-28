#ifndef WORKMANAGER_H
#define WORKMANAGER_H

#include <QDateTime>
#include <QJsonObject>
#include <QList>
#include <QString>

struct Work {
    QString id;
    QString title;
    QString description;
    QString status;
    QString priority;
    QDateTime dueDate;
    QDateTime createdDate;
    QString createdBy;
    QString notes;
};

class WorkManager {
public:
    WorkManager();

    QList<Work> getAllWorks() const;
    Work getWorkById(const QString& id) const;

    QString addWork(const QString& title, const QString& description,
                    const QString& priority, const QDateTime& dueDate,
                    const QString& createdBy);
    bool updateWork(const QString& id, const Work& updatedWork);
    bool deleteWork(const QString& id);
    bool updateWorkNotes(const QString& id, const QString& notes);

private:
    QString worksFilePath;
    QString dataPath;
    QList<Work> works;

    void loadWorks();
    void saveWorks() const;
    void ensureDataDirectory() const;

    QJsonObject workToJson(const Work& work) const;
    Work jsonToWork(const QJsonObject& json) const;
};

#endif // WORKMANAGER_H