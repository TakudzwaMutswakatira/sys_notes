#include "mainwindow.h"
#include "commandparser.h"
#include <QTextBlock>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    central = new QWidget; // Make a container
    setCentralWidget(central); // Set it as the main area

    layout = new QVBoxLayout(central); // create a layout
    terminal = new QPlainTextEdit(this); // create the terminal

    layout->addWidget(terminal);// add the terminal to the layout
    layout->setContentsMargins(0, 0, 0, 0);

    terminal->installEventFilter(this);

    prompt();
    terminal->moveCursor(QTextCursor::End);
    QString cmd = terminal->textCursor().block().text();
    terminal->appendPlainText(cmd);

    currentDeletelim = terminal->textCursor().position();

}


bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    // Isolate messages belonging exclusively to your target text edit widget
    if (watched == terminal && event->type() == QEvent::KeyPress) {

        // Cast the generic event to access keyboard properties safely
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        int cursorPosition = terminal->textCursor().position();

        // Target destructive keys when the cursor resides inside the protected zone (0-30)
        if (cursorPosition <= currentDeletelim && (keyEvent->key() == Qt::Key_Backspace || keyEvent->key() == Qt::Key_Delete)) {
            return true; // Returns true to kill the event; Qt drops the keystroke completely
        }
        if(keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return){
            if (keyEvent->modifiers() & Qt::ShiftModifier){
                newLine();
                return true;
            }else{
                readCmd();
                prompt();
                currentDeletelim = terminal->textCursor().position();
                return true;
            }
        }
    }

    // Pass all other non-matching events (like regular typing or arrow keys) back to standard processing
    return QMainWindow::eventFilter(watched, event);
}

// This is the command prompt
void MainWindow::prompt()
{
    terminal->appendPlainText("$.Note > ");
}

void MainWindow::newLine()
{
    terminal->appendPlainText("             > ");
    currentDeletelim = terminal->textCursor().position();
}

// This function reads the command from the use
void MainWindow::readCmd()
{
    QTextCursor curser(terminal->document());
    curser.setPosition(currentDeletelim, QTextCursor::MoveAnchor);
    curser.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);

    QString cmd = curser.selectedText();
    //Testing the output system
    terminal->appendPlainText(cmd);

    CommandParser Parser;
    Parser.parse(cmd);
}




MainWindow::~MainWindow() {}
