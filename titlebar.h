#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>

class QPushButton;

// Windows-Terminal-style tab strip + window buttons (visual only)
class TitleBar : public QWidget
{
public:
    explicit TitleBar(const QString &title, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void syncMaximizeIcon();

    QWidget *tab = nullptr;
    QPushButton *maxBtn = nullptr;
    bool filterInstalled = false;
};

#endif // TITLEBAR_H
