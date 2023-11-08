#include <QtWidgets>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "drawingarea.h"

DrawingArea::DrawingArea (QWidget *parent)
    : QWidget(parent)
{

    setAttribute(Qt::WA_StaticContents);


    changed = false;
    drawing = false;
    myPWidth = 1;
    myPColor = Qt::black;

}
void DrawingArea::drawText(const QString &text, const QPoint &position)
{
    QPainter painter(&image);
    painter.setPen(QPen(myPColor, myPWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawText(position, text);
    changed = true;
    update();
}

bool DrawingArea ::openImage(const QString &fileName)
{

    QImage loadedImage;


    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    changed = false;
    update();
    return true;
}


bool DrawingArea ::saveImage(const QString &fileName, const char *fileFormat)
{

    QImage visibleImage = image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        changed = false;
        return true;
    } else {
        return false;
    }
}


void DrawingArea ::setPenColor(const QColor &newColor)
{
    myPColor = newColor;
}


void DrawingArea ::setPenWidth(int newWidth)
{
    myPWidth = newWidth;
}


void DrawingArea ::clearImage()
{
    image.fill(qRgb(255, 255, 255));
    changed = true;
    update();
}


void DrawingArea ::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        drawing = true;
    }
}


void DrawingArea ::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && drawing)
        drawLine(event->pos());
}


void DrawingArea ::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && drawing) {
        drawLine(event->pos());
        drawing = false;
    }
}

void DrawingArea ::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);


    QRect nastyRect = event->rect();


    painter.drawImage(nastyRect, image, nastyRect);
}

void DrawingArea ::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}


void DrawingArea ::drawLine(const QPoint &endPoint)
{

    QPainter painter(&image);

    painter.setPen(QPen(myPColor, myPWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));


    painter.drawLine(lastPoint, endPoint);


    changed = true;

    int rad = (myPWidth / 2) + 2;


    update(QRect(lastPoint, endPoint).normalized()
               .adjusted(-rad, -rad, +rad, +rad));


    lastPoint = endPoint;
}


void DrawingArea ::resizeImage(QImage *image, const QSize &newSize)
{

    if (image->size() == newSize)
        return;


    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));


    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}
