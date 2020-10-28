#ifndef CTR_LCD_H
#define CTR_LCD_H

#include <QObject>
#include <QFile>

class CTR_lcd : public QObject
{

public:
    CTR_lcd();
    ~CTR_lcd();

public:
    void setShowBitsFile(QString fileName);

    void setPixel(unsigned int x, unsigned int y, unsigned int color);

    void LCDDrawLine(unsigned int x0, unsigned int y0,
                     unsigned int x1, unsigned int y1,
                     unsigned char color);

    void LCDDrawRect(unsigned int x0, unsigned int y0,
                     unsigned int x1, unsigned int y1,
                     unsigned char color);

    void LCDDrawCircle(int xc, int yc, int r, int fill, unsigned long color);

    void flush();
    void clearBuffer();

private:
void drawCircle8(int xc, int yc, int x, int y, unsigned long color);

public:
    unsigned int     width;
    unsigned int     height;

private:
    unsigned char   *frameBuffer;
    unsigned int     frameBufferSize;
    QFile           *m_fbdevice;
};

#endif // CTR_LCD_H
