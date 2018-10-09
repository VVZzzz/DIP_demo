#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFileInfo>
#include "runtool.h"

class QGraphicsView;
class QGraphicsScene;
class QImage;
class QLabel;
class QGraphicsPixmapItem;


#define WINDOW_ERROR "error!"
#define WINDOW_WARNING "warning!"
#define WINDOW_ABOUT "about"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
    void on_action_OPEN_triggered();

    void on_action_RESTORE_triggered();

    void on_action_RGB2GRAY_triggered();

private:
  void cleanImage();
  void updateRighView(QPixmap &newPixmap);

 private:
  Ui::MainWindow *ui;
  QGraphicsScene *leftScene;
  QGraphicsScene *rightScene;
  QGraphicsPixmapItem *leftPixmapItem;
  QGraphicsPixmapItem *rightPixmapItem;
  QImage *leftQImage;
  QImage *rightQImage;

  QLabel *sizeLabel;
  QString filePath;
  QFileInfo fileInfo;

};

#endif  // MAINWINDOW_H
