#ifndef MACDOCK_H
#define MACDOCK_H

#include <QtGui/QDialog>
#include "ui_MacDock.h"
#include "DockWidget.h"

class MacDock : public QDialog
{
    Q_OBJECT

public:
    MacDock(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~MacDock();

public slots:
    void OnItemClicked(int iconIndex);

private:
    Ui::MacDockClass ui;
};

#endif // MACDOCK_H
