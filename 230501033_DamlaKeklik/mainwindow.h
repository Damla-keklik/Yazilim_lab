#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>


class DrawingArea ;

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    MainWindow();


protected:

    void closeEvent(QCloseEvent *event) override;


private slots:
    void open();
    void save();
    void penColor();
    void penWidth();


private:

    void createActions();
    void createMenus();


    bool maybeSave();



    bool saveFile(const QByteArray &fileFormat);
    DrawingArea  *drawingArea ;


    QMenu *saveDuplicateMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;


    QAction *openAct;


    QList<QAction *> saveDuplicateAct;
    QAction *exitAct;
    QAction *penColorAct;
    QAction *penWidthAct;
    QAction *clearScreenAct;
};

#endif
