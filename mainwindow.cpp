#include "mainwindow.h"
#include <QColor>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QInputDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>
#include <QRectF>
#include <QScrollArea>
#include <QString>
#include <QStringList>
#include "mygraphicsview.h"
#include "myhistogram.h"
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
  ui->rightGraphicsView->setScene(rightScene);
  ui->statusBar->addPermanentWidget(sizeLabel);

  slidedlg = new SlideDlg(this);
  connect(slidedlg->getSliderP(), SIGNAL(valueChanged(int)), this,
          SLOT(on_slidebar_change()));
  setWindowTitle("DIP_demo");
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::cleanImage() {
  leftScene->clear();
  ui->leftGraphicsView->resetTransform();
  rightScene->clear();
  ui->rightGraphicsView->resetTransform();
  sizeLabel->clear();
}

void MainWindow::updateRighView(const QPixmap &newPixmap) {
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
  ui->rightGraphicsView->resetTransform();
  ui->leftGraphicsView->resetTransform();
}

void MainWindow::on_action_RGB2GRAY_triggered() {
  QImage img = leftPixmapItem->pixmap().toImage();
  RunTool::rgb2gray(img);
  updateRighView(QPixmap::fromImage(img));
}

void MainWindow::on_action_DEGARY_triggered() {
  if (slidedlg != nullptr) {
    category = DEGRAY;
    slidedlg->setRange(2, 100, 2);
    slidedlg->show();
  }
}

void MainWindow::on_action_DERESOLUTION_triggered() {
  if (slidedlg != nullptr) {
    category = DERESOLUTION;
    slidedlg->setRange(1, 100, 1);
    slidedlg->show();
  }
}

void MainWindow::on_slidebar_change() {
  if (slidedlg != nullptr) {
    int para = slidedlg->getPara();
    QImage img = leftPixmapItem->pixmap().toImage();
    QImage newimg;
    switch (category) {
      case DERESOLUTION:
        newimg = RunTool::deresolution(img, para);
        break;
      case DEGRAY:
        newimg = RunTool::regray(img, para);
        break;
      default:
        newimg = img;
        break;
    }
    updateRighView(QPixmap::fromImage(newimg));
  }
}

void MainWindow::on_action_HISTOGRAM_triggered() {
  QDialog *histogram_dlg = new QDialog(this);
  QScrollArea *scrollarea = new QScrollArea(histogram_dlg);
  MyHistogram *histogram = new MyHistogram(scrollarea);
  if (histogram == nullptr) return;
  histogram->setHistogram(rightPixmapItem->pixmap().toImage());
  scrollarea->setWidget(histogram);
  QHBoxLayout *hlayout = new QHBoxLayout(histogram_dlg);
  hlayout->addWidget(scrollarea);
  histogram_dlg->setLayout(hlayout);
  histogram->resize(800, 780);
  histogram_dlg->setFixedWidth(820);
  scrollarea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scrollarea->adjustSize();
  histogram_dlg->setWindowTitle("Histogram");
  histogram_dlg->show();
}

void MainWindow::on_action_ADDDENOISE_triggered() {
  QStringList fileslist;
  fileslist = QFileDialog::getOpenFileNames(
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
  if (!fileslist.isEmpty()) {
    QImage img = leftPixmapItem->pixmap().toImage();
    QImage newimg = RunTool::adddenoise(img, fileslist);
    updateRighView(QPixmap::fromImage(newimg));
  }
  QMessageBox::information(this,QString("Success!"),QString("operator compelet!"));
}
