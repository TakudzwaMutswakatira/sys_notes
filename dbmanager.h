#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QSqldatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include "note.h"

class dbManager
{
public:
    dbManager(QString instruction, Note *note);
    void createNote(Note *note);
};

#endif // DBMANAGER_H
