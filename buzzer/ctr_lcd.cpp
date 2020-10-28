#include "ctr_lcd.h"

#include <QtGlobal>

CTR_lcd::CTR_lcd()
{
    width = 128;
    height = 64;

    m_fbdevice = new QFile();
    m_fbdevice->setFileName(QString::fromUtf8("/dev/fb1"));
    if(!m_fbdevice->open(QIODevice::WriteOnly))
    {
        delete m_fbdevice;
        m_fbdevice = NULL;
    }

    frameBufferSize = width * (height + 7)/8;
    frameBuffer = new unsigned char[frameBufferSize];
}

CTR_lcd::~CTR_lcd()
{
    //if(frameBuffer) delete[] frameBuffer;

    if(m_fbdevice)
    {
        if(m_fbdevice->isOpen())
        {
            clearBuffer();
            flush();

            m_fbdevice->close();
        }

        //delete m_fbdevice;
     }
}

void CTR_lcd::setShowBitsFile(QString fileName)
{
    QFile *bmp = new QFile();
    bmp->setFileName(fileName);
    if(bmp->open(QIODevice::ReadOnly))
    {
        bmp->read((char *)frameBuffer, frameBufferSize);
        bmp->close();
    }
    delete bmp;

    if(m_fbdevice && m_fbdevice->isOpen())
    {
        flush();
    }
}

void CTR_lcd::setPixel(unsigned int x, unsigned int y, unsigned int color)
{
    unsigned int targetByteIndex;
    const unsigned char bitsMask[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

    if((x >= width) || (y >= height)) return;

    targetByteIndex = width * (y / 8) + x;
    if(color)
    {
        frameBuffer[targetByteIndex] |= bitsMask[y % 8];
    }
    else
    {
        frameBuffer[targetByteIndex] &= ~bitsMask[y % 8];
    }
}

void CTR_lcd::LCDDrawLine(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned char color)
{
    unsigned int x, y;
    int dx, dy, t, error, xd, yd;

    if ( (x0 == x1) && (y0 == y1) ) //点
    {
        setPixel(x0, y0, color);
    }
    else if ( y0 == y1 ) //横线
    {
        if ( x0 > x1 )
        {
            t=x0; x0=x1; x1=t;
        }
        for ( x = x0 ; x <= x1 ; x++ )
            setPixel(x, y0, color);
    }
    else if ( x0 == x1 ) //竖线
    {
        if ( y0 > y1 )
        {
            t=y0; y0=y1; y1=t;
        }
        for ( y = y0 ; y <= y1 ; y++ )
            setPixel(x0, y, color);
    }
    else //斜线
    {
        xd = x1 - x0; yd = y1 - y0;
        xd = qAbs(xd); yd = qAbs(yd);
        if ( xd >= yd )
        {
            if ( x0 > x1 )
            {
                t=x0; x0=x1; x1=t; t=y0; y0=y1; y1=t;
            }
            error = (yd << 1) - (xd);
            setPixel(x0, y0, color);
            x=x0; y=y0;
            dy = (y1 > y0) ? 1 : (-1);
            while ( x < x1 )
            {
                x++;
                if ( error < 0 )
                    error += (yd << 1);
                else
                {
                    error += ((yd - xd) << 1);
                    y += dy;
                }
                setPixel(x, y, color);
            }
        }
        else
        {
            if ( y0 > y1 )
            {
                t=x0; x0=x1; x1=t; t=y0; y0=y1; y1=t;
            }
            error = (xd << 1) - (yd);
            setPixel(x0, y0, color);
            x=x0; y=y0;
            dx = (x1 > x0) ? 1 : (-1);
            while ( y < y1 )
            {
                y++;
                if ( error < 0 )
                    error += (xd << 1);
                else
                {
                    error += ((xd - yd) << 1);
                    x += dx;
                }
                setPixel(x, y, color);
            }
        }
    }

}

void CTR_lcd::LCDDrawRect(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned char color)
{
    LCDDrawLine(x0, y0, x1, y0, color);
    LCDDrawLine(x1, y0, x1, y1, color);
    LCDDrawLine(x1, y1, x0, y1, color);
    LCDDrawLine(x0, y1, x0, y0, color);
}

// 八对称性
void CTR_lcd::drawCircle8(int xc, int yc, int x, int y, unsigned long color)
{
    setPixel(xc + x, yc + y, color);
    setPixel(xc - x, yc + y, color);
    setPixel(xc + x, yc - y, color);
    setPixel(xc - x, yc - y, color);
    setPixel(xc + y, yc + x, color);
    setPixel(xc - y, yc + x, color);
    setPixel(xc + y, yc - x, color);
    setPixel(xc - y, yc - x, color);
}

//Bresenham's circle algorithm
void CTR_lcd::LCDDrawCircle(int x, int y, int r, int fill, unsigned long color)
{

    // (x, y) 为圆心，r 为半径
    // fill 为是否填充
    // 如果圆在图片可见区域外，直接退出
    if ((x + r) < 0 || (x - r) >= (int)width ||
            (y + r) < 0 || (y - r) >= (int)height) return;

    int xt = 0, yt = r, yi, d;

    d = 3 - 2 * r;

    if (fill) {
        // 如果填充（画实心圆）
        while (xt <= yt)
        {

            for (yi = xt; yi <= yt; yi ++)
                drawCircle8(x, y, xt, yi, color);

            if (d < 0)
            {
                d = d + 4 * xt + 6;
            }
            else
            {
                d = d + 4 * (xt - yt) + 10;
                yt --;
            }
            xt++;
        }
    }
    else
    {
        // 如果不填充（画空心圆）
        while (xt <= yt)
        {

            drawCircle8(x, y, xt, yt, color);

            if (d < 0)
            {
                d = d + 4 * xt + 6;
            }
            else
            {
                d = d + 4 * (xt - yt) + 10;
                yt --;
            }
            xt ++;
        }
    }
}

void CTR_lcd::flush()
{
    if(m_fbdevice && m_fbdevice->isOpen())
    {
        m_fbdevice->reset();
        m_fbdevice->write((char *)frameBuffer, frameBufferSize);
        m_fbdevice->flush();
    }
}

void CTR_lcd::clearBuffer()
{
    if(frameBuffer)
        memset(frameBuffer, 0,  frameBufferSize);
}
