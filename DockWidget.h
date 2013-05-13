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

    void Restore();		// 所有图标位置和大小还原
    void Rearrange();	// 重排图标的水平位置

    void TouchIcons(QPoint pos);

    // 可设置是否需要Mac Dock的缩放效果
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

    QList<IconItem> m_iconList;	// 图标集合

    QSize m_initIconSize;	// 图标初始大小
    float m_maxIconScale;	// 图标最大缩放比例
    int m_iconSpacing;		// 图标间距
    int m_iconBaseBottom;	// 图标不弹跳时的水平线

    // 窗体给图标集合上下左右的预留位置
    int m_dockLeft;
    int m_dockTop;
    int m_dockRight;
    int m_dockBottom;

    QTimer* m_jumpTimer;	// 弹跳计时器

    int m_affectDist;		// 鼠标所影响相邻图标尺寸的最远距离（较难解释）
    bool m_macStyle;		// 是否有Mac图标缩放效果
};

#endif // DOCKWIDGET_H
