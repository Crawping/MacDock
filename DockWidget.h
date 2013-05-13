#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QWidget>
#include "ui_DockWidget.h"
#include <QPixmap>
#include <QSize>
#include <QList>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QTimer>

#include "IconItem.h"


class DockWidget : public QWidget
{
    Q_OBJECT

public:
    DockWidget(QWidget *parent = 0);
    ~DockWidget();

public:
    void AddIcon(const QPixmap& icon, const QString& tip);
    void Resize();

    void Restore();		// ����ͼ��λ�úʹ�С��ԭ
    void Rearrange();	// ����ͼ���ˮƽλ��

    void TouchIcons(QPoint pos);

    // �������Ƿ���ҪMac Dock������Ч��
    void SetMacStyle(bool macStyle);
    bool IsMacStyle();

signals:
    void IconClicked(int iconIndex);

protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void leaveEvent(QEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);

protected slots:
    void OnJumpStep();

private:
    Ui::DockWidget ui;

    QList<IconItem> m_iconList;	// ͼ�꼯��

    QSize m_initIconSize;	// ͼ���ʼ��С
    float m_maxIconScale;	// ͼ��������ű���
    int m_iconSpacing;		// ͼ����
    int m_iconBaseBottom;	// ͼ�겻����ʱ��ˮƽ��

    // �����ͼ�꼯���������ҵ�Ԥ��λ��
    int m_dockLeft;
    int m_dockTop;
    int m_dockRight;
    int m_dockBottom;

    QTimer* m_jumpTimer;	// ������ʱ��

    int m_affectDist;		// �����Ӱ������ͼ��ߴ����Զ���루���ѽ��ͣ�
    bool m_macStyle;		// �Ƿ���Macͼ������Ч��
};

#endif // DOCKWIDGET_H
