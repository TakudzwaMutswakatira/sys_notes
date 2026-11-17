#include "dbmanager.h"

dbManager::dbManager(QString instruction, Note *note) {


    if(instruction.toLower() == "add"){
        createNote(note);


    }else if(instruction.toLower() == "delete"){

    }else if(instruction.toLower() == "list"){
    }else if(instruction.toLower() == "clear"){

    }else if(instruction.toLower() == "help"){

    }

}


void dbManager::createNote(Note* note){
    QString filename = note->getTimestamp().toString() + ".md";

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Failed to create or open Markdown file:" << file.errorString();
        return;
    }
    QTextStream out(&file);
    out << note->getNote();

    file.close();
    qDebug() << "Success!";

}
