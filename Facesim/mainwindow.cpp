#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>


#include <QtWidgets>
#ifndef QT_NO_PRINTER
#include <QPrintDialog>
#endif

#include "facer.h"
#include <QPixmap>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scenem = new QGraphicsScene(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::loadFile(const QString &fileName)
{
        QImageReader reader(fileName);



        reader.setAutoTransform(true);
        const QImage newImage = reader.read();
        if (newImage.isNull()) {
            QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                     tr("Cannot load %1: %2")
                                     .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
            return false;
        }

        setImage(newImage);

        setWindowFilePath(fileName);

        const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
            .arg(QDir::toNativeSeparators(fileName)).arg(image.width()).arg(image.height()).arg(image.depth());
        statusBar()->showMessage(message);
        return true;

}

void MainWindow::setImage(const QImage &newImage)
{
    image = newImage;
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));

    /*scaleFactor = 1.0;

    scrollArea->setVisible(true);
    printAct->setEnabled(true);
    fitToWindowAct->setEnabled(true);
    updateActions();

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();*/
}


static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    foreach (const QByteArray &mimeTypeName, supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    //dialog.setFileMode(QFileDialog::AnyFile);
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}


void MainWindow::on_load_button_clicked()
{
        QFileDialog dialog(this, tr("Open File"));
        initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

        while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}

void MainWindow::on_search_button_clicked()
{
    if(windowFilePath() != "") {
        indexar();
        auto res = comparation(windowFilePath().toStdString(), ui->textEdit->toPlainText().toInt());
        QImage imagea;
        QGraphicsPixmapItem* item;
        ui->graphicsView->setScene(scenem);

        for(auto i = 0; i < res.size(); i++) {
            QString qstr = QString::fromStdString(res[i]);
            imagea.load(qstr);
            item = new QGraphicsPixmapItem(QPixmap::fromImage(imagea));

            item->setFlag(QGraphicsItem::ItemIsMovable);
            item->setPos(0,i*imagea.height()+20);
            scenem->addItem(item);
        }

         ui->graphicsView->show();
    }

}


