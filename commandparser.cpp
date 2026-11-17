#include "commandparser.h"
#include <QDebug>
#include <QRegularExpression>
#include "note.h"
#include "dbmanager.h"
CommandParser::CommandParser() {}

void CommandParser::parse(QString &cmd)
{

    this->cmd = cmd;

    QStringList cmdTokens;
    QRegularExpression re(R"((\"[^\"]*\")|(\'[^\']*\')|(--?[a-zA-Z0-9_-]+)|(\S+))");
    QRegularExpressionMatchIterator i = re.globalMatch(cmd);

    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        QString token = match.captured(0);

        token.remove(QRegularExpression("^\"|\"$|^'|'$"));
        cmdTokens << token;

    } // add -t aws udacity "This is a testing note"


    QString instruction = cmdTokens[0];

    QString noteContent = cmdTokens.last();

    int tagIndex = cmdTokens.indexOf("-t");
    QStringList tags;

    if (tagIndex != -1 && tagIndex + 1 < cmdTokens.size()){
        for(int i = tagIndex + 1; i < cmdTokens.size() -1; i++){
            tags << cmdTokens[i];
            qDebug() << tags;
        }

    }

    Note note(noteContent,tags);
    dbManager(instruction, &note);



    qDebug().noquote() << tags ;

}
