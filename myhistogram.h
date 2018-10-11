#ifndef MYHISTOGRAM_H
#define MYHISTOGRAM_H
#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QPaintEvent>

class MyHistogram : public QLabel {
  Q_OBJECT
 public:
  MyHistogram(QWidget *parent = 0);
  void setHistogram(const QImage &img);
  void paintEvent(QPaintEvent *e);
  void drawHistogram(int xBase, int yBase, int height, int flag);

 private:
  //[256]:最小灰度值[257]:最大灰度值
  int grayHistogram[258] = {0};
  int redHistogram[258] = {0};
  int greenHistogram[258] = {0};
  int blueHistogram[258] = {0};
};

#endif  // MYHISTOGRAM_H
