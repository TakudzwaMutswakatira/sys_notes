#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QWidget>
#include <QVBoxLayout>
#include <QKeyEvent>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void readCmd();
    void prompt();
    void newLine();
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;


private:
    QPlainTextEdit *terminal;
    QWidget *central;
    QVBoxLayout *layout;
    int currentDeletelim;
};
#endif // MAINWINDOW_H
