#include "IconItem.h"
#include <QWidget>

IconItem::IconItem(const QPixmap& icon, const QString& tip, const QSize& size, int baseBottom)
{
    m_rect = QRect(0, 0, 0, 0);
    m_baseBottom = baseBottom;

    m_icon = icon;
    MakeMirrorIcon();
    m_mirrorDist = 0;

    m_tip = tip;
    m_initSize = size;
    m_rect.setSize(size);
    m_isCurrent = false;
    m_isJumping = false;

    m_jumpHeight = 0;
    m_jumpTicks = 0;
    m_ticksWhenTop = 30;
    m_jumpMaxCount = 3;

    PrepareTip();
}

IconItem::~IconItem()
{
}

QSize IconItem::GetSize()
{
    return m_rect.size();
}

QRect IconItem::GetRect()
{
    QRect rect = m_rect;
    rect.moveBottom(m_baseBottom + m_jumpHeight);
    return rect;
}

void IconItem::SetCurrentIcon(bool current)
{
    m_isCurrent = current;
}

bool IconItem::IsCurrentIcon()
{
    return m_isCurrent;
}

bool IconItem::IsJumping()
{
    return m_isJumping;
}

void IconItem::PrepareJump()
{
    m_jumpHeight = 0;
    m_jumpTicks = 0;
    m_jumpCount = 0;
    m_isJumping = true;
}

/**
    无初速度时，时间和位移公式：s = 0.5 * g * (t*t)
    一个弹跳周期内，t的变化规律可视为：
    0, 1, 2, ... tm, tm-1, tm-2, ..., 0, ...  (tm为处在最高点的时刻)
**/
void IconItem::JumpStep()
{
    if (!m_isJumping)
    {
        return;
    }

    // 已回落到水平线，一个弹跳周期
    if (m_jumpTicks == 2 * m_ticksWhenTop)
    {
        m_jumpTicks = 0;

        m_jumpCount++;
        if (m_jumpCount == m_jumpMaxCount)
        {
            m_jumpCount = 0;
            m_isJumping = false;
        }
    }

    m_jumpTicks++;
    int t;

    int div = m_jumpTicks / m_ticksWhenTop;
    if (div % 2 == 0)
    {
        t = m_jumpTicks - div * m_ticksWhenTop;
    }
    else
    {
        t = m_ticksWhenTop - (m_jumpTicks - div * m_ticksWhenTop);
    }

    int jumpOffset = 0.5f * 9.8f * (m_ticksWhenTop * m_ticksWhenTop - (m_ticksWhenTop - t) * (m_ticksWhenTop - t));
    m_jumpHeight = jumpOffset / 150.0f;		// 适当定义一个常数，使弹跳效果看起来更自然
}

void IconItem::Resize(const QSize& size)
{
    m_rect.setSize(size);
    m_rect.moveBottom(m_baseBottom);
}

void IconItem::Resize(int width, int height)
{
    Resize(QSize(width, height));
}

void IconItem::MoveLeft(int left)
{
    m_rect.moveLeft(left);
}

void IconItem::Paint(QPainter* painter)
{
    int scaleJumpHeight = m_jumpHeight * (1.0f * m_rect.size().width() / m_initSize.width());
    QRect rectWithHeight = m_rect;

    // 弹跳时，若图标有缩放，则弹跳高度也按此比例缩放
    rectWithHeight.moveBottom(m_baseBottom - scaleJumpHeight);

    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter->drawPixmap(rectWithHeight, m_icon);

    QRect mirrorRect = rectWithHeight;
    mirrorRect.moveTop(rectWithHeight.bottom() + 2 * scaleJumpHeight + m_mirrorDist);
    painter->drawPixmap(mirrorRect, m_mirror);

    // 绘制文字
    if (m_isCurrent)
    {
        QRect tipRect;

        tipRect.setSize(m_tipSize);
        tipRect.moveLeft(m_rect.left() + (m_rect.width() - m_tipSize.width()) / 2);
        tipRect.moveBottom(rectWithHeight.top() - m_tipHeight);

        QPen framePen(m_tipBgColor);
        QBrush bgBrush(m_tipBgColor);

        painter->setPen(framePen);
        painter->setBrush(bgBrush);
        painter->drawRoundedRect(tipRect, 5, 5);

        painter->setFont(m_tipFont);
        QPen textPen(m_tipTextColor);
        painter->setPen(textPen);
        painter->drawText(tipRect, Qt::AlignCenter, m_tip);
    }
}

void IconItem::MakeMirrorIcon()
{
    QImage image = m_icon.toImage().mirrored(false, true);
    m_mirror = QPixmap::fromImage(image);

    QPixmap tempMirror(m_mirror.size());
    tempMirror.fill(Qt::transparent);
    QPainter p(&tempMirror);
    p.setCompositionMode(QPainter::CompositionMode_Source);
    p.drawPixmap(0, 0, m_mirror);
    p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p.fillRect(tempMirror.rect(), QColor(0, 0, 0, int(0.3 * 255)));

    m_mirror = tempMirror;
}

void IconItem::PrepareTip()
{
    m_tipHeight = 20;
    m_tipFont.setFamily("Arial");
    m_tipFont.setPixelSize(20);

    QPixmap p(100, 100);
    QPainter tipPainter(&p);
    tipPainter.setFont(m_tipFont);
    QRect tipRect = tipPainter.boundingRect(0, 0, 100, 100, Qt::AlignLeft, m_tip);
    m_tipSize = tipRect.size() + QSize(4, 4);

    m_tipTextColor = QColor(255, 255, 255);
    m_tipBgColor = QColor(0, 0, 0, 160);
}
