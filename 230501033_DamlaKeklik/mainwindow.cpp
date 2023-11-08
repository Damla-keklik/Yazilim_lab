#include <QtWidgets>
#include "mainwindow.h"
#include "drawingarea.h"

MainWindow::MainWindow()
{
    drawingArea  = new DrawingArea ;
    setCentralWidget(drawingArea );


    createActions();
    createMenus();

    setWindowTitle(tr("Draw"));
    resize(500, 500);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open File"), QDir::currentPath());

        if (!fileName.isEmpty())
            drawingArea ->openImage(fileName);
    }
}

void MainWindow::save()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void MainWindow::penColor()
{
    QColor newColor = QColorDialog::getColor(drawingArea ->penColor());
    if (newColor.isValid())
        drawingArea ->setPenColor(newColor);
}

void MainWindow::penWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Draw"),
                                        tr("Select pen width:"),
                                        drawingArea ->penWidth(),
                                        1, 30, 1, &ok);
    if (ok)
        drawingArea ->setPenWidth(newWidth);
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());
        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(save()));
        saveDuplicateAct.append(action);
    }

    exitAct = new QAction(tr("E&xit"), this);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    penWidthAct = new QAction(tr("Pen &Width..."), this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    clearScreenAct = new QAction(tr("&Clear Screen"), this);
    connect(clearScreenAct, SIGNAL(triggered()), drawingArea, SLOT(clearImage()));
}

void MainWindow::createMenus()
{
    saveDuplicateMenu = new QMenu(tr("&Save Duplicate"), this);
    foreach (QAction *action, saveDuplicateAct)
        saveDuplicateMenu->addAction(action);

    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveDuplicateMenu);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addSeparator();
    optionMenu->addAction(clearScreenAct);


    QAction *addTextAction = new QAction(tr("Add Text"), this);
    connect(addTextAction, &QAction::triggered, this, [=]() {
        QString text = QInputDialog::getText(this, tr("Add Text"), tr("Text:"));
        if (!text.isEmpty()) {
            bool okX, okY;
            int x = QInputDialog::getInt(this, tr("X Coordinate:"), tr("X Coordinate:"), 0, -1000, 1000, 1, &okX);
            int y = QInputDialog::getInt(this, tr("Y Coordinate:"), tr("Y Coordinate:"), 0, -1000, 1000, 1, &okY);
            if (okX && okY) {
                drawingArea->drawText(text, QPoint(x, y));
            } else {
                // Kullanıcı iptal etti veya geçersiz bir değer girdi.
            }
        }
    });
    optionMenu->addAction(addTextAction);
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);

}

bool MainWindow::maybeSave()
{
    if (drawingArea->isChanged()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Draw"),
                                   tr("Changes have been made to the drawing.\n"
                                      "Would you like to save the changes?"),
                                   QMessageBox::Save | QMessageBox::Ignore
                                       | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            return saveFile("png");
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/unnamed." + fileFormat;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Duplicate"),
                                                    initialPath);
    if (fileName.isEmpty()) {
        return false;
    } else {
        return drawingArea ->saveImage(fileName, fileFormat.constData());
    }
}
