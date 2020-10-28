#include "lcdpainter.h"

LCDPainter::LCDPainter()
{
    lcd = NULL;
}

LCDPainter::~LCDPainter()
{
    points.clear();
}

void LCDPainter::drawWave()
{
    QPoint prePoint;
    QPoint point;

    int index = 0;
    if(!lcd) return;

    lcd->clearBuffer();
    foreach(point, points)
    {
        if(index)
        {
            point.setX(index * lcd->width / pointsSize);
            lcd->LCDDrawLine(prePoint.x(), prePoint.y(), point.x(), point.y(), 1);
        }
        prePoint = point;
        index++;
    }
    lcd->flush();
}

void LCDPainter::addData(const int data)
{
    if(!lcd) return;

    QPoint point;

    point.setX(points.size());
    point.setY(data * (lcd->height - 8) / 0x00ff);

    addPoint(point);
}

void LCDPainter::addDatas(const int *points, int datasCount)
{
    if(!lcd) return;

    QPoint point;

    for(int i = 0 ; i < datasCount ; i++)
    {
        point.setX(this->points.size()*lcd->width / pointsSize);
        point.setY(points[i] * (lcd->height - 8) / 0x00ff);

        this->points.append(point);
    }

    checkDataValid();
}

void LCDPainter::setPointsSize(int size)
{
    pointsSize = size;
}

void LCDPainter::setLcd(CTR_lcd *lcd)
{
    this->lcd = lcd;
}

void LCDPainter::checkDataValid()
{
    if(!lcd) return;

    // 窗口只显示 d->pointsSize 个点，去掉前面多余的点
    if(pointsSize < points.size())
    {
        points.remove(0, points.size() - pointsSize);
        QPoint point;
        int index = 0;
        foreach(point, points)
        {
            point.setX(index * lcd->width / pointsSize);
            points.setPoint(index++, point);
        }
    }
}

void LCDPainter::addPoint(const QPoint &point)
{
    points.append(point);
    checkDataValid();
}
