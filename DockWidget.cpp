#include "DockWidget.h"
#include "IconItem.h"
#include <QPainter>
#include <QDebug>

DockWidget::DockWidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setMouseTracking(true);

    m_macStyle = true;

    m_initIconSize = QSize(50, 50);
    m_maxIconScale = 2.0f;

    m_iconSpacing = 0;
    m_dockLeft = 50;
    m_dockTop = 150;
    m_dockRight = 50;
    m_dockBottom = 20;

    m_iconBaseBottom = m_dockTop + m_initIconSize.height();

    m_affectDist = 2 * (m_initIconSize.width() + m_iconSpacing);

    m_jumpTimer = new QTimer(this);
    m_jumpTimer->setInterval(15);

    connect(m_jumpTimer, SIGNAL(timeout()), this, SLOT(OnJumpStep()));
}

DockWidget::~DockWidget()
{
}

void DockWidget::SetMacStyle(bool macStyle)
{
    m_macStyle = macStyle;
}

bool DockWidget::IsMacStyle()
{
    return m_macStyle;
}

void DockWidget::AddIcon(const QPixmap& icon, const QString& tip)
{
    IconItem item(icon, tip, m_initIconSize, m_iconBaseBottom);
    if (m_iconList.isEmpty())
    {
        item.MoveLeft(m_dockLeft);
    }

    m_iconList << item;

    Resize();
    Rearrange();
}

void DockWidget::Resize()
{
    int w = m_iconList.size() * m_initIconSize.width() +
            (m_iconList.size() - 1) * m_iconSpacing +
            m_dockLeft + m_dockRight;

    int h = m_initIconSize.height() + m_dockTop + m_dockBottom;

    setFixedSize(w, h);
}

void DockWidget::Restore()
{
    int x = m_dockLeft;

    for (int i = 0; i < m_iconList.size(); i++)
    {
        IconItem& item = m_iconList[i];
        item.SetCurrentIcon(false);
        item.Resize(m_initIconSize);
        item.MoveLeft(x);

        x += item.GetSize().width() + m_iconSpacing;
    }
}

void DockWidget::Rearrange()
{
    int x = m_iconList[0].GetRect().x();

    for (int i = 0; i < m_iconList.size(); i++)
    {
        IconItem& item = m_iconList[i];
        item.MoveLeft(x);
        x += item.GetSize().width() + m_iconSpacing;
    }
}

void DockWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    for (int i = 0; i < m_iconList.size(); i++)
    {
        IconItem& item = m_iconList[i];
        item.Paint(&painter);
    }
}

/**
    鄙人定义的图标缩放算法：
    每个图标在不缩放状态下，以它们的中心点作为标识，
    分别计算鼠标位置的x值和它们中心点x值的差。
    若差值为0，表明鼠标与此图标中心点垂直，则让此图标缩放到最大；
    若差值>m_affectDist，表时鼠标离此图标很远了，让此图标保持初始尺寸；
    若差值在中间区间，则根据距离等比缩放图标。
    注意：即使图标被缩放了，它们的中心点仍视作不变

    图标缩放了，图标链的长度一般也将变化，
    此时需让新图标链的中点水平值保持不变。
**/

void DockWidget::TouchIcons(QPoint pos)
{
    int newLength = 0;
    for (int i = 0; i < m_iconList.size(); i++)
    {
        int x, dist;

        x = m_dockLeft + 0.5f * m_initIconSize.width() + i * (m_initIconSize.width() + m_iconSpacing);
        dist = qAbs(pos.x() - x);

        IconItem& item = m_iconList[i];
        if (dist <= m_affectDist)
        {
            float newWidth = (1 - m_maxIconScale) * m_initIconSize.width() / m_affectDist * dist + m_maxIconScale * m_initIconSize.width();
            float newHeight = (1 - m_maxIconScale) * m_initIconSize.height() / m_affectDist * dist + m_maxIconScale * m_initIconSize.height();
            item.Resize((int)(newWidth + 0.5f), (int)(newHeight + 0.5f));
        }
        else
        {
            item.Resize(m_initIconSize);
        }

        newLength += item.GetSize().width();
    }

    newLength += m_iconSpacing * (m_iconList.size() - 1);
    int fixedLength = m_initIconSize.width() * m_iconList.size() + m_iconSpacing * (m_iconList.size() - 1);

    int newX = (fixedLength - newLength) / 2 + m_dockLeft;
    m_iconList[0].MoveLeft(newX);

    Rearrange();
}

void DockWidget::mouseMoveEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();

    if (m_macStyle)
    {
        // 图标最大比例时的top值，是判断鼠标进入的临界
        if (pos.y() >= (height() - m_dockBottom - m_maxIconScale * m_initIconSize.height()))
        {
            TouchIcons(pos);
        }
        else
        {
            Restore();
        }
    }

    for (int i = 0; i < m_iconList.size(); i++)
    {
        QRect rect = m_iconList[i].GetRect();
        if (rect.contains(pos))
        {
            m_iconList[i].SetCurrentIcon(true);
        }
        else
        {
            m_iconList[i].SetCurrentIcon(false);
        }
    }

    // 因为计时器很快，所以若在运行的话，可不用再发重绘事件了
    if (!m_jumpTimer->isActive())
    {
        repaint();
    }
}

void DockWidget::leaveEvent(QEvent* event)
{
    // 鼠标离开，还原
    Restore();
    repaint();
}

void DockWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton)
    {
        return;
    }

    QPoint pos = event->pos();

    for (int i = 0; i < m_iconList.size(); i++)
    {
        IconItem& item = m_iconList[i];
        QRect rect = item.GetRect();
        if (rect.contains(pos))
        {
            emit IconClicked(i);

            // 没有弹跳，则开始
            if (!item.IsJumping())
            {
                item.PrepareJump();		// 必须做准备工作
                m_jumpTimer->start();
                break;
            }
        }
    }
}

void DockWidget::OnJumpStep()
{
    int jumpIcons = 0;	// 统计有几个图标在弹跳

    for (int i = 0; i < m_iconList.size(); i++)
    {
        IconItem& item = m_iconList[i];
        if (item.IsJumping())
        {
            item.JumpStep();
            jumpIcons++;
        }
    }

    repaint();

    // 没有图标弹跳，停止计时器
    if (jumpIcons == 0)
    {
        m_jumpTimer->stop();
    }
}
