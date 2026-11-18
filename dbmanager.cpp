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
    QString filename = note->getTimestamp().toString("yyyy-MM-dd") + ".md";
    QString vaultPath = "C:\\Users\\mutsw\\OneDrive\\Obsidian\\Librarium\\01 Ephemeral Notes\\";

    QFile file(vaultPath + filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Failed to create or open Markdown file:" << file.errorString();
        return;
    }
    QStringList tags = note->getTags();
    QTextStream out(&file);
    out << note->getTimestamp().toString("HH:mm") << "\n\n";
    out << "Status: " << note->getStatus() << "\n\n";
    out << "Tags: ";
    for(const QString& tag : tags){
        out << "[[" << tag << "]] ";
    }
    out << "\n\n";
    out << note->getNote();

    file.close();
    qDebug() << "Success!";

}
