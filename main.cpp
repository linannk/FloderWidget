#include <QApplication>
#include <QTimer>
#include <QIcon>
#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QComboBox>
#include "pixelfloderwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PixelFloderWidget pfw;
    QWidget* ws[5] = {0};
    QIcon icos[5];
    icos[0].addFile(":/0.ico");
    icos[1].addFile(":/1.ico");
    icos[2].addFile(":/2.ico");
    icos[3].addFile(":/3.ico");
    icos[4].addFile(":/4.ico");
    for(int i = 0; i < 4; ++i)
    {
        QWidget* w = new QWidget;
        w->setMinimumHeight(100);
        w->setStyleSheet("background-color:red;");
        pfw.addWidget(w , "test");
        ws[i] = w;
    }

    QTimer timer;
    timer.setInterval(500);
    timer.start();
    QObject::connect(&timer , &QTimer::timeout , [&pfw , &ws , &icos](){
        for(int i = 0; i < 4; ++i)
        {
            pfw.setIconAndTitle(ws[i] , "wwwwwbackground-color:cyan;background-color:cyan;" + QString::number(qrand()) + "xxx" , icos[qrand() % 5]);
        }
    });

    QWidget* www = new QWidget;
    QGridLayout* lyt = new QGridLayout;
    QLabel* fr = new QLabel("Language");
    QComboBox* cbx = new QComboBox;
    lyt->addWidget(fr , 0 , 0 , 1 , 1);
    lyt->addWidget(cbx , 0 , 1 , 1 , 1);
    QLabel* n = new QLabel("name:");
    QLineEdit* e = new QLineEdit;
    lyt->addWidget(n , 1 , 0 , 1 , 1);
    lyt->addWidget(e , 1 , 1  , 1 , 1);
    QRadioButton* r = new QRadioButton("REd");
    QRadioButton* g = new QRadioButton("Green");
    QPushButton* o = new QPushButton("Ok");
    QPushButton* c = new QPushButton("Cancel");
    QPushButton* c1 = new QPushButton();
    //c1->setMinimumHeight(500);

    lyt->addWidget(r , 2 , 0 , 1 , 1);
    lyt->addWidget(g , 2 , 1 , 1 , 1);


    lyt->addWidget(o , 3 , 0 , 1 , 1);
    lyt->addWidget(c , 3 , 1 , 1 , 1);
    //lyt->addWidget(c1 , 4 , 1 , 1 , 1);

    www->setLayout(lyt);
    pfw.addWidget(www , "wwwwwbackground-color:cyan;background-color:cyan;" , icos[1]);
    pfw.addWidget(c1 , "123424123423");
    QComboBox *ccb = new QComboBox;
    ccb->addItem(QStringLiteral("中文"));
    ccb->addItem(QStringLiteral("English"));
    ccb->addItem(QStringLiteral("你好吗？"));
    ccb->addItem(QStringLiteral("大黄蜂"));
    ccb->addItem(QStringLiteral("擎天柱"));
    pfw.addTitleWidget(www , new QLineEdit , 2);
    pfw.addTitleWidget(www , ccb , 1);
    QWidget* w = new QWidget;
    w->setMinimumHeight(100);
    w->setStyleSheet("background-color:cyan;");
    pfw.addWidget(w , "test" , icos[2]);
    QComboBox* cc = new QComboBox;
    cc->addItem("123d412");
    cc->addItem("1234f12");
    cc->addItem("1234f12");
    cc->addItem("1234f12");
    pfw.addTitleWidget(w , cc);
    pfw.show();
    return a.exec();
}
