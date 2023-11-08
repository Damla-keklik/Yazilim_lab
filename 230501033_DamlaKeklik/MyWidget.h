#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QMenu>
#include <QMap>

class MyWidget : public QWidget {
    Q_OBJECT

public:
    enum class Shape {
        Line,
        Circle,
        Triangle,
        Rectangle,
        Ellipse,
        Hexagon,
        Star,
        None
    };

    MyWidget(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QPoint startPoint;
    QPoint endPoint;
    bool drawing;
    Shape shape;
    QMenu *shapeMenu;
    QMap<Shape, QColor> shapeFillColors;
    QMap<Shape, QIcon> shapeIcons;


private slots:
    void setShapeToLine();
    void setShapeToCircle();
    void setShapeToTriangle();
    void setShapeToRectangle();
    void setShapeToEllipse();
    void setShapeToHexagon();
    void setShapeToStar();
    void handleOption1();
    void handleOption2();
    QString shapeToString(Shape shape);
};

#endif
