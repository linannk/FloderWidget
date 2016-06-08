#include "pixelfloderwidget.h"
#include <QBoxLayout>
#include <QPaintEvent>
#include <QMap>
#include <QMouseEvent>
#include <QPainter>
#include <QCoreApplication>
#include <QStyle>
#include <QStyleOptionButton>
#include <QStyleOptionComplex>
#include <QRadialGradient>
#include <QFontMetrics>
#include <QDebug>

class FolderTitleWidget
        : public QWidget
{
    Q_OBJECT
public:
    explicit FolderTitleWidget(QWidget* item , QWidget* parent = 0)
        : QWidget(parent)
        , m_relatedWidget(item)
    {
        Q_ASSERT(item != nullptr);

        //! [0]设置空白，左边留出的位置是为了绘制三角形时不与被的元素重合
        QMargins margins;
        margins.setLeft(25);
        margins.setTop(0);
        margins.setRight(0);
        margins.setBottom(0);
        setContentsMargins(margins);

        //! [1]设置StyleSheet的目的是，让图标不会与三角形重合，当空间的过窄时。
        setStyleSheet("FolderTitleWidget {padding-left:25;padding-right:15;}");
        setSizePolicy(QSizePolicy::MinimumExpanding , QSizePolicy::Fixed);
        setFocusPolicy(Qt::StrongFocus);
    }

    //! 设置想要显示的图标
    void setIcon(const QIcon& icon)
    {
        m_icon = icon;
        update();
    }

    /*!
     * \brief setText 设置想要显示的文本
     * \param title
     */
    void setText(const QString& title)
    {
        if (m_title != title)
        {
            m_title = title;
            update();
        }
    }

    /*!
     * \brief setIconText 同时修改图表和文本
     * \param text
     * \param icon
     */
    void setIconText(const QString& text , const QIcon& icon)
    {
        m_title = text;
        m_icon = icon;
        update();
    }

    /*!
     * \brief addWidget 在标题栏上添加一个控件
     * \param titleWidget
     * \param stretch
     * \param alignment
     */
    void addWidget(QWidget *titleWidget , int stretch = 0, Qt::Alignment alignment = 0)
    {
        if (!layout())
        {
            QHBoxLayout* lyt = new QHBoxLayout;
            lyt->addWidget(titleWidget , stretch , alignment);
            lyt->setMargin(0);
            setLayout(lyt);
        }
        else
        {
            static_cast<QHBoxLayout*>(layout())->addWidget(titleWidget , stretch , alignment);
        }
    }

    /*!
     * \brief sizeHint 返回标题栏的最合适的尺寸，默认为7个字符宽外加两个图标的宽度以及多余的留白的宽度
     * \return
     */
    QSize sizeHint() const
    {
        int preferedWidth = 20;
        if (!m_icon.isNull())
            preferedWidth += 21;

        QFontMetrics metrics = fontMetrics();
        preferedWidth += 7 * metrics.width(QChar('W'));

        return QSize(preferedWidth , 24);
    }

protected:

    void enterEvent(QEvent*) override
    {
        update();
    }

    void leaveEvent(QEvent*) override
    {
        update();
    }

    void mouseReleaseEvent(QMouseEvent* e)
    {
        Q_ASSERT(m_relatedWidget != nullptr);
        if (e->button() == Qt::LeftButton)
        {
            if (m_relatedWidget->isVisible())
            {
                m_relatedWidget->setVisible(false);
                QCoreApplication::sendEvent(m_relatedWidget , &PixelFloderEvent(true));
            }
            else
            {
                m_relatedWidget->setVisible(true);
                QCoreApplication::sendEvent(m_relatedWidget , &PixelFloderEvent(false));
            }
        }
        update();
    }

    void paintEvent(QPaintEvent* e)
    {
        QPainter painter(this);
        painter.setClipRect(e->rect());
        painter.setPen(Qt::NoPen);
        painter.setRenderHint(QPainter::Antialiasing , true);

        const int availableWidth = width() - 20 - (m_icon.isNull() ? 0 : 21);
        const int theMaxNumOfChar = availableWidth / fontMetrics().width(QChar('w'));
        QStyleOptionButton optionButton;
        optionButton.initFrom(this);
        optionButton.icon = m_icon;
        optionButton.iconSize = QSize(16 , 16);
        if (m_title.length() > theMaxNumOfChar)
        {
            const QString tmp = m_title.left((theMaxNumOfChar - 3) / 2) + "..." + m_title.right((theMaxNumOfChar - 3) / 2);
            optionButton.text = tmp;
        }
        else
        {
            optionButton.text = m_title;
        }
        optionButton.rect = rect();
        optionButton.state = 0;
        if (isEnabled())
        {
            optionButton.state |= QStyle::State_Enabled;
        }
        if (this->underMouse())
        {
            optionButton.state |= QStyle::State_MouseOver;
        }
        if (this->hasFocus())
        {
            optionButton.state |= QStyle::State_HasFocus;
        }

        if (!layout() || layout()->count() == 0)
        {
            style()->drawControl(QStyle::CE_PushButton , &optionButton , &painter , this);
        }
        else
        {
            optionButton.text = QString();
            optionButton.icon = QIcon();
            style()->drawControl(QStyle::CE_PushButton , &optionButton , &painter , this);
        }

        QPoint pts[3] = {
            {8  , 8} ,
            {16 , 8} ,
            {12 , 16}
        };

        QPoint pts2[3] = {
            {8  , 8} ,
            {8 , 16} ,
            {16 , 12}
        };

        if (m_relatedWidget->isVisible())
        {
            painter.setPen(Qt::NoPen);
            QRadialGradient gra(12 , 12 , 8  , 12 , 12);
            gra.setColorAt(0.0 , QColor(55 , 55 , 55));
            gra.setColorAt(0.9 , QColor(55 , 55 , 55));
            gra.setColorAt(1.0 , Qt::white);
            painter.setBrush(gra);
            painter.drawPolygon(pts , 3);
        }
        else
        {
            painter.setPen(Qt::darkGray);
            painter.setBrush(Qt::white);
            painter.drawPolygon(pts2 , 3);
        }
    }

private:
    /*!
     * \brief m_relatedWidget 与标题栏相关联的Widget，标题栏会控制其显示与隐藏
     */
    QWidget* m_relatedWidget;

    /*!
     * \brief m_icon 将要绘制的图标
     */
    QIcon m_icon;

    /*!
     * \brief m_title 显示的标题
     */
    QString m_title;
};

class FolderContentWidget
        : public QWidget
{
    Q_OBJECT
public:
    explicit FolderContentWidget(QWidget* parent = 0)
        : QWidget(parent)
        , m_vLyt(new QVBoxLayout)
    {
        m_vLyt->addStretch(0);
        m_vLyt->setSpacing(0);
        m_vLyt->setMargin(0);
        setContentsMargins(0 , 0 , 0 , 0);
        setLayout(m_vLyt);
    }

    ~FolderContentWidget()
    { }

    void addWidget(QWidget* wgt , const QString& title , const QIcon& icon = QIcon())
    {
        Q_ASSERT(m_items[wgt] == nullptr);
        FolderTitleWidget* titleWgt = new FolderTitleWidget(wgt , this);
        titleWgt->setText(title);
        titleWgt->setIcon(icon);
        m_items[wgt] = titleWgt;
        const int cnt = m_vLyt->count() - 1;
        m_vLyt->insertWidget(cnt , titleWgt , 0);
        m_vLyt->insertWidget(cnt + 1 , wgt , 0);
    }

    void setTitle(QWidget* wgt , const QString& text)
    {
        Q_ASSERT(m_items[wgt] != nullptr);
        m_items[wgt]->setText(text);
    }

    void setIcon(QWidget* wgt , const QIcon& icon)
    {
        Q_ASSERT(m_items[wgt] != nullptr);
        m_items[wgt]->setIcon(icon);
    }

    void setIconAndTitle(QWidget* wgt , const QString& text , const QIcon& icon)
    {
        Q_ASSERT(m_items[wgt] != nullptr);
        m_items[wgt]->setIconText(text , icon);
    }

    void addTitleWidget(QWidget *wgt, QWidget *titleWidget , int stretch = 0, Qt::Alignment alignment = 0)
    {
        Q_ASSERT(m_items[wgt] != nullptr);
        m_items[wgt]->addWidget(titleWidget , stretch , alignment);
    }

    int count() const
    {
        return m_items.count();
    }
protected:

private:
    QVBoxLayout* m_vLyt;
    QMap<QWidget* , FolderTitleWidget*> m_items;
};

#include "pixelfloderwidget.moc"

PixelFloderWidget::PixelFloderWidget(QWidget *parent)
    : QScrollArea(parent)
    , m_centralWidget(new FolderContentWidget(this))
{
    QScrollArea::setWidget(m_centralWidget);
    setWidgetResizable(true);
}

PixelFloderWidget::~PixelFloderWidget()
{ }

void PixelFloderWidget::addWidget(QWidget *wgt, const QString &title, const QIcon &icon)
{
    m_centralWidget->addWidget(wgt , title , icon);
}

void PixelFloderWidget::addTitleWidget(QWidget *wgt, QWidget *titleWidget, int stretch, Qt::Alignment alignment)
{
    m_centralWidget->addTitleWidget(wgt , titleWidget , stretch , alignment);
}

void PixelFloderWidget::setTitle(QWidget *wgt, const QString &text)
{
    m_centralWidget->setTitle(wgt , text);
}

void PixelFloderWidget::setIcon(QWidget *wgt, const QIcon &icon)
{
    m_centralWidget->setIcon(wgt , icon);
}

void PixelFloderWidget::setIconAndTitle(QWidget* wgt , const QString &title, const QIcon &icon)
{
    m_centralWidget->setIconAndTitle(wgt , title , icon);
}

int PixelFloderWidget::count() const
{
    return m_centralWidget->count();
}

PixelFloderEvent::PixelFloderEvent(bool hidden)
    : QEvent(static_cast<Type>(PixelFloderEventType))
    , m_isHidden(hidden)
{

}

PixelFloderEvent::~PixelFloderEvent()
{

}
