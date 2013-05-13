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

    // ����ƶ�ʱ�������º����ı�ͼ���С��λ��
    void Resize(const QSize& size);
    void Resize(int width, int height);
    void MoveLeft(int left);

    // ����ͼ�������
    void Paint(QPainter* painter);

    bool IsJumping();	// �Ƿ����ڵ���
    void PrepareJump();	// ׼��������Ԥ�ò���
    void JumpStep();	// ÿ���������������õĺ���

protected:
    QPixmap m_icon;		// ͼ��
    QPixmap m_mirror;	// ����ͼ��
    int m_mirrorDist;	// ԭͼ��ײ��뾵��ͼ�궥���ľ���

    QString m_tip;		// ˵������
    QFont m_tipFont;	// ˵����������
    QSize m_tipSize;	// �������ֳߴ�
    int m_tipHeight;	// ���ֵײ���ͼ�궥���ľ���

    QColor m_tipTextColor;	// ������ɫ
    QColor m_tipBgColor;	// ���ֱ�����ɫ

    QRect m_rect;		// ������ʱ������
    QSize m_initSize;	// ͼ���ʼ�ߴ�

    int m_ticksWhenTop;	// ���ϵ���ʱ�����˼�������䣬��˷���
    int m_baseBottom;	// ��¼ͼ�겻����ʱ����ˮƽ��
    bool m_isCurrent;	// ����ƶ�ʱ���Ƿ�ͣ���ڵ�ǰͼ��

    int m_jumpHeight;	// �����߶ȣ�����ʱ�����ϱ仯
    int m_jumpTicks;	// ��������
    bool m_isJumping;	// �Ƿ����ڵ���
    int m_jumpMaxCount;	// �����������ޣ�������ֹͣ����
    int m_jumpCount;	// �ѵ���������

};
