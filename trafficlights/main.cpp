#include <QtGui/QApplication>
#include "dialog.h"

#include "leds.h"
#include <QtGui>

#include <sys/signal.h>

void sig_handler(int sig)            //信号处理函数

{
    if(sig == SIGINT)
    {
        QApplication::quit();
    }

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;

    signal(SIGINT, sig_handler);

    //east
    QLeds trafficled_er(Qt::red, &w);
    QLeds trafficled_ey(Qt::yellow, &w);
    QLeds trafficled_eg(Qt::green, &w);
    QLabel label_e(&w);

    trafficled_er.setObjectName(QString::fromUtf8("east.r"));
    trafficled_ey.setObjectName(QString::fromUtf8("east.y"));
    trafficled_eg.setObjectName(QString::fromUtf8("east.g"));
    label_e.setText(QString::fromUtf8("E"));

    trafficled_er.setGeometry(QRect(220, 60, 24, 24));
    trafficled_ey.setGeometry(QRect(220, 90, 24, 24));
    trafficled_eg.setGeometry(QRect(220, 120, 24, 24));
    label_e.move(195,95);

    trafficled_er.off();
    trafficled_ey.off();
    trafficled_eg.off();

    system("echo 0,0,0 > /sys/devices/platform/davinci_ctr.2/traffic_lights/east");

    QObject::connect(&trafficled_er, SIGNAL(toggleEvent(QLeds *)), &w, SLOT(trafficled_light_clicked(QLeds *)));
    QObject::connect(&trafficled_ey, SIGNAL(toggleEvent(QLeds *)), &w, SLOT(trafficled_light_clicked(QLeds *)));
    QObject::connect(&trafficled_eg, SIGNAL(toggleEvent(QLeds *)), &w, SLOT(trafficled_light_clicked(QLeds *)));

    //sorth
    QLeds trafficled_sr(Qt::red, &w);
    QLeds trafficled_sy(Qt::yellow, &w);
    QLeds trafficled_sg(Qt::green, &w);
    QLabel label_s(&w);

    trafficled_sr.setObjectName(QString::fromUtf8("south.r"));
    trafficled_sy.setObjectName(QString::fromUtf8("south.y"));
    trafficled_sg.setObjectName(QString::fromUtf8("south.g"));
    label_s.setText(QString::fromUtf8("S"));

    trafficled_sr.setGeometry(QRect(170, 150, 24, 24));
    trafficled_sy.setGeometry(QRect(140, 150, 24, 24));
    trafficled_sg.setGeometry(QRect(110, 150, 24, 24));
    label_s.move(145,130);

    trafficled_sr.off();
    trafficled_sy.off();
    trafficled_sg.off();

    system("echo 0,0,0 > /sys/devices/platform/davinci_ctr.2/traffic_lights/south");

    QObject::connect(&trafficled_sr, SIGNAL(toggleEvent(QLeds *)), &w, SLOT(trafficled_light_clicked(QLeds *)));
    QObject::connect(&trafficled_sy, SIGNAL(toggleEvent(QLeds *)), &w, SLOT(trafficled_light_clicked(QLeds *)));
    QObject::connect(&trafficled_sg, SIGNAL(toggleEvent(QLeds *)), &w, SLOT(trafficled_light_clicked(QLeds *)));

    //west
    QLeds trafficled_wr(Qt::red, &w);
    QLeds trafficled_wy(Qt::yellow, &w);
    QLeds trafficled_wg(Qt::green, &w);
    QLabel label_w(&w);

    trafficled_wr.setObjectName(QString::fromUtf8("west.r"));
    trafficled_wy.setObjectName(QString::fromUtf8("west.y"));
    trafficled_wg.setObjectName(QString::fromUtf8("west.g"));
    label_w.setText(QString::fromUtf8("W"));

    trafficled_wr.setGeometry(QRect(60, 120, 24, 24));
    trafficled_wy.setGeometry(QRect(60, 90, 24, 24));
    trafficled_wg.setGeometry(QRect(60, 60, 24, 24));
    label_w.move(90,95);

    trafficled_wr.off();
    trafficled_wy.off();
    trafficled_wg.off();

    system("echo 0,0,0 > /sys/devices/platform/davinci_ctr.2/traffic_lights/west");

    QObject::connect(&trafficled_wr, SIGNAL(toggleEvent(QLeds *)), &w, SLOT(trafficled_light_clicked(QLeds *)));
    QObject::connect(&trafficled_wy, SIGNAL(toggleEvent(QLeds *)), &w, SLOT(trafficled_light_clicked(QLeds *)));
    QObject::connect(&trafficled_wg, SIGNAL(toggleEvent(QLeds *)), &w, SLOT(trafficled_light_clicked(QLeds *)));

    //north
    QLeds trafficled_nr(Qt::red, &w);
    QLeds trafficled_ny(Qt::yellow, &w);
    QLeds trafficled_ng(Qt::green, &w);
    QLabel label_n(&w);

    trafficled_nr.setObjectName(QString::fromUtf8("north.r"));
    trafficled_ny.setObjectName(QString::fromUtf8("north.y"));
    trafficled_ng.setObjectName(QString::fromUtf8("north.g"));
    label_n.setText(QString::fromUtf8("N"));

    trafficled_nr.setGeometry(QRect(110, 30, 24, 24));
    trafficled_ny.setGeometry(QRect(140, 30, 24, 24));
    trafficled_ng.setGeometry(QRect(170, 30, 24, 24));
    label_n.move(145,55);

    trafficled_nr.off();
    trafficled_ny.off();
    trafficled_ng.off();

    system("echo 0,0,0 > /sys/devices/platform/davinci_ctr.2/traffic_lights/north");

    QObject::connect(&trafficled_nr, SIGNAL(toggleEvent(QLeds *)), &w, SLOT(trafficled_light_clicked(QLeds *)));
    QObject::connect(&trafficled_ny, SIGNAL(toggleEvent(QLeds *)), &w, SLOT(trafficled_light_clicked(QLeds *)));
    QObject::connect(&trafficled_ng, SIGNAL(toggleEvent(QLeds *)), &w, SLOT(trafficled_light_clicked(QLeds *)));

    w.show();
    return a.exec();
}

