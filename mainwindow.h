#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileInfo>
#include <QMainWindow>
#include <QString>
#include "mygraphicsview.h"
#include "runtool.h"
#include "slidedlg.h"

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

  void on_action_DEGARY_triggered();

  void on_action_DERESOLUTION_triggered();

  void on_slidebar_change();


  void on_action_HISTOGRAM_triggered();

  void on_action_ADDDENOISE_triggered();

private:
  enum PROCESS_CATEGORY { DERESOLUTION, DEGRAY };
  void cleanImage();
  void updateRighView(const QPixmap &newPixmap);

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

  SlideDlg *slidedlg;
  PROCESS_CATEGORY category;
};

#endif  // MAINWINDOW_H
