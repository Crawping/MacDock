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
    ���˶����ͼ�������㷨��
    ÿ��ͼ���ڲ�����״̬�£������ǵ����ĵ���Ϊ��ʶ��
    �ֱ�������λ�õ�xֵ���������ĵ�xֵ�Ĳ
    ����ֵΪ0������������ͼ�����ĵ㴹ֱ�����ô�ͼ�����ŵ����
    ����ֵ>m_affectDist����ʱ������ͼ���Զ�ˣ��ô�ͼ�걣�ֳ�ʼ�ߴ磻
    ����ֵ���м����䣬����ݾ���ȱ�����ͼ�ꡣ
    ע�⣺��ʹͼ�걻�����ˣ����ǵ����ĵ�����������

    ͼ�������ˣ�ͼ�����ĳ���һ��Ҳ���仯��
    ��ʱ������ͼ�������е�ˮƽֵ���ֲ��䡣
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
        // ͼ��������ʱ��topֵ�����ж���������ٽ�
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

    // ��Ϊ��ʱ���ܿ죬�����������еĻ����ɲ����ٷ��ػ��¼���
    if (!m_jumpTimer->isActive())
    {
        repaint();
    }
}

void DockWidget::leaveEvent(QEvent* event)
{
    // ����뿪����ԭ
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

            // û�е�������ʼ
            if (!item.IsJumping())
            {
                item.PrepareJump();		// ������׼������
                m_jumpTimer->start();
                break;
            }
        }
    }
}

void DockWidget::OnJumpStep()
{
    int jumpIcons = 0;	// ͳ���м���ͼ���ڵ���

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

    // û��ͼ�굯����ֹͣ��ʱ��
    if (jumpIcons == 0)
    {
        m_jumpTimer->stop();
    }
}
