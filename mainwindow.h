#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QWidget>
#include <QVBoxLayout>
#include <QKeyEvent>

class TitleBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void readCmd();
    void prompt();
    void newLine();
    void applyRoundedCorners();
    void applyTerminalStyle();
    void applyTitleBar();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

#ifdef Q_OS_WIN
protected:
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;
#endif

private:
    QPlainTextEdit *terminal;
    QWidget *central;
    QVBoxLayout *layout;
    int currentDeletelim;
    TitleBar *titleBar = nullptr;
};
#endif // MAINWINDOW_H
