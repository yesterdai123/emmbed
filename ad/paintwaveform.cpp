#include "paintwaveform.h"

#include <QPainter>

class QPaintWaveformPrivate
{
public:
    QPaintWaveformPrivate(QPaintWaveform * parent):
            q_ptr( parent )
    {}

    //  Members
    QPaintWaveform * const q_ptr;
    Q_DECLARE_PUBLIC(QPaintWaveform);

    // 串口能显示的点的集合
    QPolygon points;

    // 串口能显示的点数
    int pointsSize;
};

QPaintWaveform::QPaintWaveform(QWidget *parent) :
    QWidget(parent),
    d_ptr(new QPaintWaveformPrivate(this))
{
    Q_D(QPaintWaveform);
    d->points.clear();
    d->pointsSize = 0;
}

QPaintWaveform::~QPaintWaveform()
{
    delete d_ptr;
}

void QPaintWaveform::paintEvent(QPaintEvent *event)
{
    Q_D(const QPaintWaveform);

    QPainter painter(this);
    painter.setBrush(Qt::white);

    // 如果点的集合不空则显示
    if(!d->points.isEmpty())
        painter.drawPolyline(d->points);
}

void QPaintWaveform::checkDataValid()
{
    Q_D(QPaintWaveform);
    // 窗口只显示 d->pointsSize 个点，去掉前面多余的点
    if(d->pointsSize < d->points.size())
    {
        d->points.remove(0, d->points.size() - d->pointsSize);
        QPoint point;
        int index = 0;
        foreach(point, d->points)
        {
            point.setX(index * width() / d->pointsSize);
            d->points.setPoint(index++, point);
        }
    }
}

void QPaintWaveform::addData(const int data)
{
    Q_D(QPaintWaveform);
    QPoint point;

    point.setX(d->points.size());
    point.setY(data * (height()- 10) / 0x00ff + 10);

    addPoint(point);
}

void QPaintWaveform::addPoint(const QPoint &point)
{
    Q_D(QPaintWaveform);

    d->points.append(point);
    checkDataValid();

    update();
}

void QPaintWaveform::addDatas(const int *points, int datasCount)
{
    Q_D(QPaintWaveform);
    QPoint point;

    for(int i = 0 ; i < datasCount ; i++)
    {
        point.setX(d->points.size()* width() / d->pointsSize);
        point.setY(points[i] * (height()- 10) / 0x00ff + 10);

        d->points.append(point);
    }

    checkDataValid();

    update();
}

void QPaintWaveform::addPoints(const QPoint *points, int pointsCount)
{
    Q_D(QPaintWaveform);

    for(int i = 0 ; i < pointsCount ; i++)
        d->points.append(points[i]);

    checkDataValid();

    update();
}

void QPaintWaveform::addPoints(const QPolygon &points)
{
    Q_D(QPaintWaveform);
    QPoint point;
    foreach(point, points)
        d->points.append(point);

    checkDataValid();

    update();
}

void QPaintWaveform::setPointsSize(int size)
{
    Q_D(QPaintWaveform);
    d->pointsSize = size;
    if(d->pointsSize < d->points.size())
    {
        d->points.remove(0, d->points.size() - d->pointsSize);
    }

    update();
}

