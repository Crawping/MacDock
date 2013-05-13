#pragma once
#include <QPixmap>
#include <QString>
#include <QSize>
#include <QRect>
#include <QPainter>
#include <QFont>

class IconItem
{
public:
    IconItem(const QPixmap& icon, const QString& tip, const QSize& size, int baseBottom);
    ~IconItem();

public:
    QRect GetRect();
    QSize GetSize();

    void MakeMirrorIcon();
    void PrepareTip();

    void SetCurrentIcon(bool current);
    bool IsCurrentIcon();

    // 鼠标移动时，用以下函数改变图标大小和位置
    void Resize(const QSize& size);
    void Resize(int width, int height);
    void MoveLeft(int left);

    // 绘制图标和文字
    void Paint(QPainter* painter);

    bool IsJumping();	// 是否正在弹跳
    void PrepareJump();	// 准备弹跳，预置参数
    void JumpStep();	// 每个计数周期所调用的函数

protected:
    QPixmap m_icon;		// 图标
    QPixmap m_mirror;	// 镜像图标
    int m_mirrorDist;	// 原图标底部与镜像图标顶部的距离

    QString m_tip;		// 说明文字
    QFont m_tipFont;	// 说明文字字体
    QSize m_tipSize;	// 计算文字尺寸
    int m_tipHeight;	// 文字底部到图标顶部的距离

    QColor m_tipTextColor;	// 文字颜色
    QColor m_tipBgColor;	// 文字背景颜色

    QRect m_rect;		// 不弹跳时的区域
    QSize m_initSize;	// 图标初始尺寸

    int m_ticksWhenTop;	// 向上弹跳时，到此计数则回落，如此反复
    int m_baseBottom;	// 记录图标不弹跳时所在水平线
    bool m_isCurrent;	// 鼠标移动时，是否停留在当前图标

    int m_jumpHeight;	// 弹跳高度，弹跳时将不断变化
    int m_jumpTicks;	// 弹跳计数
    bool m_isJumping;	// 是否正在弹跳
    int m_jumpMaxCount;	// 弹跳周期上限，超过则停止弹跳
    int m_jumpCount;	// 已弹跳周期数

};
