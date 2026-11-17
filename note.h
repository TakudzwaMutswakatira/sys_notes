#ifndef NOTE_H
#define NOTE_H

#include <QString>
#include <QList>
#include <QDateTime>

class Note
{
public:
    Note();
    Note(const QString& noteContent,const QStringList& tags = QStringList(), const QString& status = "" );

    QString getNote() const;
    QStringList getTags() const;
    QString getStatus() const;
    QDateTime getTimestamp() const;

    QString toDailyNoteFormat();

private:
    QString noteContent;
    QStringList tags;
    QDateTime timeStamp;
    QString status;

};

#endif // NOTE_H
