#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

class DrawingArea  : public QWidget
{
    Q_OBJECT

public:
    DrawingArea (QWidget *parent = 0);


    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);


    bool isChanged() const { return changed; }
    QColor penColor() const { return myPColor; }
    int penWidth() const { return myPWidth; }

public slots:


    void clearImage();
    void drawText(const QString &text, const QPoint &position);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;


    void paintEvent(QPaintEvent *event) override;


    void resizeEvent(QResizeEvent *event) override;

private:
    void drawLine(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);


    bool changed;


    bool drawing;


    int myPWidth;
    QColor myPColor;


    QImage image;


    QPoint lastPoint;
};

#endif
