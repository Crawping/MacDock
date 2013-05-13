#include "MacDock.h"

MacDock::MacDock(QWidget *parent, Qt::WFlags flags)
    : QDialog(parent, flags)
{
    ui.setupUi(this);

    setStyleSheet("MacDock { border-image:url(./grass.png); }");

    connect(ui.m_dockWidget, SIGNAL(IconClicked(int)), this, SLOT(OnItemClicked(int)));

    ui.m_dockWidget->AddIcon(QPixmap(":/img/house.png"), "House");
    ui.m_dockWidget->AddIcon(QPixmap(":/img/cup.png"), "Cup");
    ui.m_dockWidget->AddIcon(QPixmap(":/img/clock.png"), "Clock");
    ui.m_dockWidget->AddIcon(QPixmap(":/img/diary.png"), "Diary");
    ui.m_dockWidget->AddIcon(QPixmap(":/img/calendar.png"), "Calendar");
    ui.m_dockWidget->AddIcon(QPixmap(":/img/phone.png"), "Phone");
    ui.m_dockWidget->AddIcon(QPixmap(":/img/soccer.png"), "Soccer");
    ui.m_dockWidget->AddIcon(QPixmap(":/img/firefox.png"), "Firefox");

}

MacDock::~MacDock()
{
}

void MacDock::OnItemClicked(int iconIndex)
{
    QString title = "MacDock - you clicked icon %1";
    title = title.arg(iconIndex + 1);
    setWindowTitle(title);
}
