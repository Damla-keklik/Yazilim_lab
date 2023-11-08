#include "MyWidget.h"
#include <QPainter>
#include <cmath>
#include <QMenuBar>
#include <QMenu>
#include <QtWidgets>
#include <QIcon>
#include <QMessageBox>

MyWidget::MyWidget(QWidget *parent) : QWidget(parent), drawing(false), shape(Shape::None) {
    shapeMenu = new QMenu("Shapes", this);



    shapeIcons[Shape::Line] = QIcon(":/icons/line_icon.ico");
    shapeIcons[Shape::Circle] = QIcon(":/icons/circle_icon.ico");
    shapeIcons[Shape::Triangle] = QIcon(":/icons/triangle_icon.ico");
    shapeIcons[Shape::Rectangle] = QIcon(":/icons/rectangle_icon.ico");
    shapeIcons[Shape::Ellipse] = QIcon(":/icons/ellipse_icon.ico");
    shapeIcons[Shape::Hexagon] = QIcon(":/icons/hexagon_icon.ico");
    shapeIcons[Shape::Star] = QIcon(":/icons/star_icon.ico");


    QAction *lineAction = new QAction(shapeIcons[Shape::Line], "Line", this);
    connect(lineAction, &QAction::triggered, this, &MyWidget::setShapeToLine);

    QAction *circleAction = new QAction(shapeIcons[Shape::Circle], "Circle", this);
    connect(circleAction, &QAction::triggered, this, &MyWidget::setShapeToCircle);

    QAction *triangleAction = new QAction(shapeIcons[Shape::Triangle], "Triangle", this);
    connect(triangleAction, &QAction::triggered, this, &MyWidget::setShapeToTriangle);

    QAction *rectangleAction = new QAction(shapeIcons[Shape::Rectangle], "Rectangle", this);
    connect(rectangleAction, &QAction::triggered, this, &MyWidget::setShapeToRectangle);

    QAction *ellipseAction = new QAction(shapeIcons[Shape::Ellipse], "Ellipse", this);
    connect(ellipseAction, &QAction::triggered, this, &MyWidget::setShapeToEllipse);

    QAction *hexagonAction = new QAction(shapeIcons[Shape::Hexagon], "Hexagon", this);
    connect(hexagonAction, &QAction::triggered, this, &MyWidget::setShapeToHexagon);

    QAction *starAction = new QAction(shapeIcons[Shape::Star], "Star", this);
    connect(starAction, &QAction::triggered, this, &MyWidget::setShapeToStar);

    shapeMenu->addAction(lineAction);
    shapeMenu->addAction(circleAction);
    shapeMenu->addAction(triangleAction);
    shapeMenu->addAction(rectangleAction);
    shapeMenu->addAction(ellipseAction);
    shapeMenu->addAction(hexagonAction);
    shapeMenu->addAction(starAction);


    QMenu *operationMenu = new QMenu("Operation", this);

    QIcon chooseIcon(":/icons/choose_icon.ico");
    QIcon deleteIcon(":/icons/delete_icon.ico");


    QAction *chooseAction = new QAction(chooseIcon, "Choose", this);
    connect(chooseAction, &QAction::triggered, this, &MyWidget::handleOption1);


    QAction *deleteAction = new QAction(deleteIcon, "Delete", this);
    connect(deleteAction, &QAction::triggered, this, &MyWidget::handleOption2);


    operationMenu->addAction(chooseAction);
    operationMenu->addAction(deleteAction);


    shapeMenu->addMenu(operationMenu);


    QMenuBar* menuBar = new QMenuBar(this);
    menuBar->addMenu(shapeMenu);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setMenuBar(menuBar);
}

void MyWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        startPoint = event->pos();
        drawing = true;
    }
}

void MyWidget::mouseMoveEvent(QMouseEvent *event) {
    if ((event->buttons() & Qt::LeftButton) && drawing) {
        endPoint = event->pos();
        update();
    }
}

void MyWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    if (drawing && shape != Shape::None)
    {
        QColor fillColor = shapeFillColors[shape];
        painter.setBrush(fillColor);

        if (shape == Shape::Line) {
            painter.drawLine(startPoint, endPoint);
        } else if (shape == Shape::Circle) {
            int radius = std::sqrt(std::pow(endPoint.x() - startPoint.x(), 2) +
                                   std::pow(endPoint.y() - startPoint.y(), 2));
            painter.drawEllipse(startPoint, radius, radius);
        } else if (shape == Shape::Triangle) {

            QPolygonF triangle;
            QPointF p1(startPoint.x(), startPoint.y() - 2 * (endPoint.y() - startPoint.y()));
            QPointF p2(startPoint.x() - (endPoint.x() - startPoint.x()),
                       startPoint.y() + (endPoint.y() - startPoint.y()));
            QPointF p3(startPoint.x() + (endPoint.x() - startPoint.x()),
                       startPoint.y() + (endPoint.y() - startPoint.y()));
            triangle << p1 << p2 << p3;
            painter.drawPolygon(triangle);
        } else if (shape == Shape::Rectangle) {
            QRectF rect(startPoint, endPoint);
            painter.drawRect(rect);
        } else if (shape == Shape::Ellipse) {
            QRectF rect(startPoint, endPoint);
            painter.drawEllipse(rect);
        }

        else if (shape == Shape::Star) {

            double angle = 2 * M_PI / 5;
            int outerRadius = std::min(std::abs(endPoint.x() - startPoint.x()), std::abs(endPoint.y() - startPoint.y()));
            int innerRadius = outerRadius / 2;

            QPolygonF starPolygon;
            for (int i = 0; i < 5; ++i) {
                double currentAngle = i * angle - M_PI / 2;
                double xOuter = startPoint.x() + outerRadius * std::cos(currentAngle);
                double yOuter = startPoint.y() + outerRadius * std::sin(currentAngle);
                double xInner = startPoint.x() + innerRadius * std::cos(currentAngle + angle / 2);
                double yInner = startPoint.y() + innerRadius * std::sin(currentAngle + angle / 2);

                starPolygon << QPointF(xOuter, yOuter) << QPointF(xInner, yInner);
            }

            painter.drawPolygon(starPolygon);


        }
        else if (shape == Shape::Hexagon) {

            double angle = 2 * M_PI / 6;
            int radius = std::min(std::abs(endPoint.x() - startPoint.x()), std::abs(endPoint.y() - startPoint.y()));

            QPolygonF hexagonPolygon;
            for (int i = 0; i < 6; ++i) {
                double currentAngle = i * angle;
                double x = startPoint.x() + radius * std::cos(currentAngle);
                double y = startPoint.y() + radius * std::sin(currentAngle);
                hexagonPolygon << QPointF(x, y);
            }

            painter.drawPolygon(hexagonPolygon);
        }

    }
}

void MyWidget::setShapeToRectangle() {
    shape = Shape::Rectangle;
    update();
}

void MyWidget::setShapeToEllipse() {
    shape = Shape::Ellipse;
    update();
}

void MyWidget::setShapeToLine() {
    shape = Shape::Line;
    update();
}

void MyWidget::setShapeToCircle() {
    shape = Shape::Circle;
    update();
}

void MyWidget::setShapeToTriangle() {
    shape = Shape::Triangle;
    update();
}
void MyWidget::setShapeToHexagon() {
    shape = Shape::Hexagon;
    update();
}

void MyWidget::setShapeToStar() {
    shape = Shape::Star;
    update();
}

QString MyWidget::shapeToString(Shape shape) {
    switch (shape) {
    case Shape::Line:
        return "Line";
    case Shape::Circle:
        return "Circle";
    case Shape::Triangle:
        return "Triangle";
    case Shape::Rectangle:
        return "Rectangle";
    case Shape::Ellipse:
        return "Ellipse";
    case Shape::Hexagon:
        return "Hexagon";
    case Shape::Star:
        return "Star";
    default:
        return "None";
    }
}

void MyWidget::handleOption1() {
    QColorDialog colorDialog(this);
    colorDialog.setWindowTitle("Choose Fill Color");
    QColor fillColor = colorDialog.getColor();

    if (fillColor.isValid()) {

        shapeFillColors[shape] = fillColor;


        update();
    } else {

        QMessageBox::information(this, "Fill Color Change Canceled", "Fill color change canceled");
    }
}

void MyWidget::handleOption2() {
    if (shape != Shape::None) {

        shape = Shape::None;


        QMessageBox::information(this, "Shape Deleted", "Selected shape has been deleted.");


        update();
    } else {

        QMessageBox::information(this, "No Shape Selected", "No shape selected to delete.");
    }
}
