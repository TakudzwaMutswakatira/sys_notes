#include "titlebar.h"

#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>

// Segoe MDL2 Assets glyphs (built into Windows 10/11)
static QString glyph(ushort code) { return QString(QChar(code)); }

static QPushButton *makeButton(const QString &text, const QString &name,
                               int w, int h, QWidget *parent)
{
    auto *b = new QPushButton(text, parent);
    b->setObjectName(name);
    b->setFixedSize(w, h);
    b->setFocusPolicy(Qt::NoFocus);   // never steal keyboard focus from the terminal
    return b;
}

TitleBar::TitleBar(const QString &title, QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(40);

    // ---- the tab ----
    tab = new QWidget(this);
    tab->setFixedSize(200, 34);

    auto *icon = new QLabel(">_", tab);
    icon->setObjectName("tabIcon");
    icon->setAlignment(Qt::AlignCenter);
    icon->setFixedSize(18, 14);

    auto *label = new QLabel(title, tab);
    label->setObjectName("tabTitle");

    auto *tabClose = makeButton(glyph(0xE711), "tabClose", 22, 22, tab);

    auto *tabLayout = new QHBoxLayout(tab);
    tabLayout->setContentsMargins(12, 0, 6, 0);
    tabLayout->setSpacing(8);
    tabLayout->addWidget(icon);
    tabLayout->addWidget(label, 1);
    tabLayout->addWidget(tabClose);

    // ---- "+" and chevron ----
    auto *plusBtn = makeButton(glyph(0xE710), "plusBtn", 32, 28, this);
    auto *chevBtn = makeButton(glyph(0xE70D), "chevBtn", 28, 28, this);

    auto *left = new QWidget(this);
    auto *leftLayout = new QHBoxLayout(left);
    leftLayout->setContentsMargins(8, 6, 0, 0);   // tab sits flush with the bottom edge
    leftLayout->setSpacing(0);
    leftLayout->addWidget(tab);
    leftLayout->addSpacing(4);
    leftLayout->addWidget(plusBtn, 0, Qt::AlignVCenter);
    leftLayout->addWidget(chevBtn, 0, Qt::AlignVCenter);
    leftLayout->addStretch();

    // ---- window buttons ----
    auto *minBtn   = makeButton(glyph(0xE921), "minBtn",   46, 32, this);
    maxBtn         = makeButton(glyph(0xE922), "maxBtn",   46, 32, this);
    auto *closeBtn = makeButton(glyph(0xE8BB), "closeBtn", 46, 32, this);

    auto *root = new QHBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);
    root->addWidget(left, 1);
    root->addWidget(minBtn,   0, Qt::AlignTop);
    root->addWidget(maxBtn,   0, Qt::AlignTop);
    root->addWidget(closeBtn, 0, Qt::AlignTop);

    connect(minBtn, &QPushButton::clicked, this, [this] { window()->showMinimized(); });
    connect(maxBtn, &QPushButton::clicked, this, [this] {
        if (window()->isMaximized()) window()->showNormal();
        else                         window()->showMaximized();
    });
    connect(closeBtn, &QPushButton::clicked, this, [this] { window()->close(); });
    connect(tabClose, &QPushButton::clicked, this, [this] { window()->close(); });

    setStyleSheet(R"CSS(
        QPushButton {
            background: transparent; border: none; color: #ffffff;
            font-family: "Segoe MDL2 Assets"; font-size: 10px;
        }
        QPushButton:hover { background: rgba(255, 255, 255, 25); }
        QPushButton#closeBtn:hover { background: #c42b1c; }
        QPushButton#tabClose, QPushButton#plusBtn, QPushButton#chevBtn {
            border-radius: 4px; font-size: 9px;
        }
        QLabel { background: transparent; }
        QLabel#tabTitle { color: #ffffff; font: 9pt "Segoe UI"; }
        QLabel#tabIcon {
            color: #cccccc; border: 1px solid #cccccc; border-radius: 2px;
            font: bold 7pt "Consolas";
        }
    )CSS");
}

// Paints the strip and the tab as ONE shape so the tab color matches the
// terminal below it exactly (no double-alpha darkening over the glass).
void TitleBar::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QPainterPath bar;
    bar.addRect(rect());

    QRectF tabRect(tab->mapTo(this, QPoint(0, 0)), QSizeF(tab->size()));
    QPainterPath tabPath;
    tabPath.addRoundedRect(tabRect.adjusted(0, 0, 0, 8), 8, 8); // round the top corners only
    tabPath = tabPath.intersected(bar);

    p.fillPath(bar.subtracted(tabPath), QColor(32, 32, 32, 150)); // strip
    p.fillPath(tabPath, QColor(12, 12, 12, 90));                  // tab = terminal color
}

void TitleBar::showEvent(QShowEvent *e)
{
    if (!filterInstalled) {
        window()->installEventFilter(this);   // watch for maximize/restore
        filterInstalled = true;
    }
    syncMaximizeIcon();
    QWidget::showEvent(e);
}

bool TitleBar::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == window() && event->type() == QEvent::WindowStateChange)
        syncMaximizeIcon();
    return QWidget::eventFilter(watched, event);
}

void TitleBar::syncMaximizeIcon()
{
    maxBtn->setText(glyph(window()->isMaximized() ? 0xE923 : 0xE922)); // restore / maximize
}
