#ifndef PIXELFLODERWIDGET_H
#define PIXELFLODERWIDGET_H

#include <QScrollArea>
#include <QIcon>
#include <QEvent>

const int PixelFloderEventType = QEvent::User + 1001;

class PixelFloderEvent
        : public QEvent
{
public:
    explicit PixelFloderEvent(bool hidden = true);
    ~PixelFloderEvent();

    void setHidden(bool hidden = true) { m_isHidden = hidden; }
    bool isHidden() const { return m_isHidden; }
private:
    bool m_isHidden;
};

class FolderContentWidget;

class PixelFloderWidget
        : public QScrollArea
{
    Q_OBJECT
public:
    explicit PixelFloderWidget(QWidget* parent = 0);
    ~PixelFloderWidget();
    void setWidget(QWidget *widget) = delete;

    //! 以下函数，以控件的指针为索引
    void addWidget(QWidget *wgt , const QString& title , const QIcon& icon = QIcon());
    void addTitleWidget(QWidget* wgt , QWidget* titleWidget , int stretch = 0, Qt::Alignment alignment = 0);
    void setTitle(QWidget *wgt , const QString& text);
    void setIcon(QWidget *wgt , const QIcon& icon);
    void setIconAndTitle(QWidget *wgt, const QString& title , const QIcon& icon);

    void insertWidget(int index , QWidget* wgt , const QString& title , const QIcon& icon = QIcon());

    QWidget* removeWidget(int index);
    void removeWidget(QWidget* wgt);

    int count() const;
private:
    FolderContentWidget* m_centralWidget;
};

#endif // PIXELFLODERWIDGET_H
