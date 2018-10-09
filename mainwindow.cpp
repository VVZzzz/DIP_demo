#include "mainwindow.h"
#include <QColor>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>
#include <QRectF>
#include <QString>
#include "runtool.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  leftScene = new QGraphicsScene(this);
  rightScene = new QGraphicsScene(this);
  sizeLabel = new QLabel(this);

  leftScene->setBackgroundBrush(QColor::fromRgb(224, 224, 224));
  ui->leftGraphicsView->setScene(leftScene);
  rightScene->setBackgroundBrush(QColor::fromRgb(224, 224, 224));
  ui->rightGrahpicsView->setScene(rightScene);
  ui->statusBar->addPermanentWidget(sizeLabel);

  setWindowTitle("DIP_demo");
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::cleanImage() {
  leftScene->clear();
  ui->leftGraphicsView->resetTransform();
  rightScene->clear();
  ui->rightGrahpicsView->resetTransform();
  sizeLabel->clear();
}

void MainWindow::updateRighView(QPixmap &newPixmap) {
  rightPixmapItem->setPixmap(
      newPixmap);  //如果直接setPixmap,旧的Qpixmap会被收回。
}

void MainWindow::on_action_OPEN_triggered() {
  filePath = QFileDialog::getOpenFileName(
      this, tr("Open image"), "C:/Users",
      tr("All Files (*);;"
         "All Images (*.bpm *.gif *.jpg *.jpeg *.png *.ppm *.xbm *.xpm);;"
         "Image BPM (*.bpm);;"
         "Image GIF (*.gif);;"
         "Image JPG (*.jpg);;"
         "Image JPEG (*.jpeg);;"
         "Image PNG (*.png);;"
         "Image PPM (*.ppm);;"
         "Image XBM (*.xbm);;"
         "Image XPM (*.xpm);;"));
  if (!filePath.isEmpty()) {
    if (!QFile(filePath).open(QIODevice::ReadOnly)) {
      QMessageBox::critical(this, WINDOW_ERROR, "Unable to open image!");
      return;
    }
    cleanImage();
    fileInfo = QFileInfo(filePath);
    QPixmap tempPixmap(filePath);
    leftPixmapItem = leftScene->addPixmap(tempPixmap);
    rightPixmapItem = rightScene->addPixmap(tempPixmap);
    leftScene->setSceneRect(QRectF(tempPixmap.rect()));
    rightScene->setSceneRect(QRectF(tempPixmap.rect()));

    setWindowTitle(fileInfo.fileName() + "-DIP_demo");
    sizeLabel->setText(fileInfo.baseName() + " : " +
                       QString::number(tempPixmap.width()) + "x" +
                       QString::number(tempPixmap.height()));
  }
}

void MainWindow::on_action_RESTORE_triggered() {
  updateRighView(leftPixmapItem->pixmap());
  ui->rightGrahpicsView->resetTransform();
}

void MainWindow::on_action_RGB2GRAY_triggered() {
  QImage img = leftPixmapItem->pixmap().toImage();
  RunTool::rgb2gray();
  updateRighView(QPixmap::fromImage(img));
}
