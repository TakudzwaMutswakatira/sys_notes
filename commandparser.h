#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <QString>

class CommandParser
{
public:
    CommandParser();

    void parse(QString &cmd);
private:
    QString cmd;
};

#endif // COMMANDPARSER_H
