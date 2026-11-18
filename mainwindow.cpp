#include "mainwindow.h"
#include "commandparser.h"
#include <QTextBlock>
#include <QFont>
#include <QFontMetrics>
#ifdef Q_OS_WIN
#include <windows.h>
#include <dwmapi.h>
#endif
#include "titlebar.h"
#include <QAbstractButton>
#include <QCursor>

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

    applyTerminalStyle();
    applyRoundedCorners();
    applyTitleBar();

    prompt();
    terminal->moveCursor(QTextCursor::End);
    //QString cmd = terminal->textCursor().block().text();
    //terminal->appendPlainText(cmd);

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

    CommandParser Parser;
    Parser.parse(cmd);
}

void MainWindow::applyTerminalStyle()
{
    resize(960, 600);
    setWindowTitle("C:\\sys_note.exe");
    setWindowOpacity(0.90);

    QFont font;
    font.setFamilies({"Cascadia Mono", "Cascadia Code", "Consolas", "Courier New"});
    font.setStyleHint(QFont::Monospace);
    font.setPointSize(12);
    terminal->setFont(font);

    terminal->setFrameShape(QFrame::NoFrame);
    terminal->setCursorWidth(3);
    terminal->setTabStopDistance(QFontMetricsF(font).horizontalAdvance(' ') * 4);

    // Dark acrylic-like colors
    setStyleSheet("QMainWindow { background-color: #0c0c0c; }");

    terminal->setStyleSheet(R"(
        QPlainTextEdit {
            background-color: rgba(12, 12, 12, 215);
            color: #f2f2f2;
            border: none;
            padding: 8px 10px;
            selection-background-color: #3a96dd;
            selection-color: #ffffff;
        }
        QScrollBar:vertical {
            background: transparent;
            width: 10px;
            margin: 0;
        }
        QScrollBar::handle:vertical {
            background: rgba(255, 255, 255, 70);
            min-height: 30px;
            border-radius: 2px;
        }
        QScrollBar::handle:vertical:hover {
            background: rgba(255, 255, 255, 120);
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical,
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
            background: none;
            height: 0;
        }
    )");
}

void MainWindow::applyTitleBar()
{
    // Remove the native title bar, keep the other default window hints
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

    titleBar = new TitleBar("$.Note", central);
    layout->insertWidget(0, titleBar);   // above the terminal
    layout->setSpacing(0);
}

#ifdef Q_OS_WIN
// Native drag + resize for the frameless window (Snap, double-click maximize, etc.)
bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
{
    if (eventType == "windows_generic_MSG") {
        MSG *msg = static_cast<MSG *>(message);

        if (msg->message == WM_NCHITTEST) {
            const QPoint globalPos = QCursor::pos();
            const QPoint p = mapFromGlobal(globalPos);

            // 1) Resize borders (not while maximized)
            if (!isMaximized()) {
                const int b = 6;
                const bool left   = p.x() < b;
                const bool right  = p.x() >= width()  - b;
                const bool top    = p.y() < b;
                const bool bottom = p.y() >= height() - b;

                if (top && left)     { *result = HTTOPLEFT;     return true; }
                if (top && right)    { *result = HTTOPRIGHT;    return true; }
                if (bottom && left)  { *result = HTBOTTOMLEFT;  return true; }
                if (bottom && right) { *result = HTBOTTOMRIGHT; return true; }
                if (left)            { *result = HTLEFT;        return true; }
                if (right)           { *result = HTRIGHT;       return true; }
                if (top)             { *result = HTTOP;         return true; }
                if (bottom)          { *result = HTBOTTOM;      return true; }
            }

            // 2) Dragging by the title bar (anywhere that isn't a button)
            if (titleBar && p.y() < titleBar->height()) {
                QWidget *child = titleBar->childAt(titleBar->mapFromGlobal(globalPos));
                if (!qobject_cast<QAbstractButton *>(child)) {
                    *result = HTCAPTION;
                    return true;
                }
            }
        }
    }
    return QMainWindow::nativeEvent(eventType, message, result);
}
#endif

void MainWindow::applyRoundedCorners()
{
#ifdef Q_OS_WIN
    HWND hwnd = reinterpret_cast<HWND>(winId());
    int corner = 2; // DWMWCP_ROUND
    DwmSetWindowAttribute(hwnd, 33 /*DWMWA_WINDOW_CORNER_PREFERENCE*/, &corner, sizeof(corner));
#endif
}

MainWindow::~MainWindow() {}
