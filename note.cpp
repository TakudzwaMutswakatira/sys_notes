#include "note.h"

Note::Note() : timeStamp(QDateTime::currentDateTime()) {}

Note::Note(const QString& noteContent,const QStringList& tags, const QString& status)
    : noteContent(noteContent),
    tags(tags),
    status(status){ }

QString Note::getNote() const
{
    return noteContent;
}

QStringList Note::getTags() const
{
    return tags;
}

QString Note::getStatus() const
{
    return status;
}

QDateTime Note::getTimestamp() const
{
    return timeStamp;
}

QString Note::toDailyNoteFormat()
{
    QString timeString = timeStamp.toString("HH:mm");
    QString tagString = "";
    QString status = "";
    for (const QString& tag : tags){
        tagString += "#" + tag + " ";
    }

    return QString("- **%1** | %2 %3")
        .arg(timeString)
        .arg(noteContent)
        .arg(tagString.trimmed());

}

