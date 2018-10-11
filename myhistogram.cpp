#include "myhistogram.h"
#include <QPainter>
#include "runtool.h"

MyHistogram::MyHistogram(QWidget *parent) : QLabel(parent) {
  grayHistogram[256] = redHistogram[256] = greenHistogram[256] =
      blueHistogram[256] = 256;
  grayHistogram[257] = redHistogram[257] = greenHistogram[257] =
      blueHistogram[257] = -1;
}

void MyHistogram::setHistogram(const QImage &img) {
  QImage grayImg(img);
  RunTool::rgb2gray(grayImg);
  unsigned w = img.width(), h = img.height();
  for (int y = 0; y < h; y++) {
    const uchar *lpix = img.scanLine(y);
    const uchar *lpix2 = grayImg.scanLine(y);
    for (int x = 0; x < w; x++) {
      uchar red = *(lpix + int(x >> 2) + 2);
      uchar green = *(lpix + int(x >> 2) + 1);
      uchar blue = *(lpix + int(x >> 2));
      uchar gray = *(lpix2 + int(x >> 2));
      redHistogram[red]++;
      greenHistogram[green]++;
      blueHistogram[blue]++;
      grayHistogram[gray]++;
      if (red > redHistogram[257])
        redHistogram[257] = red;
      else if (red < redHistogram[256])
        redHistogram[256] = red;
      if (green > greenHistogram[257])
        greenHistogram[257] = green;
      else if (green < greenHistogram[256])
        greenHistogram[256] = green;
      if (blue > blueHistogram[257])
        blueHistogram[257] = blue;
      else if (blue < blueHistogram[256])
        blueHistogram[256] = blue;
      if (gray > grayHistogram[257])
        grayHistogram[257] = gray;
      else if (gray < grayHistogram[256])
        grayHistogram[256] = gray;
    }
  }
}

void MyHistogram::paintEvent(QPaintEvent *e) {
  Q_UNUSED(e)

  int step = 100;               // distance between histograms
  int height = 255 + 10;        // histograms height
  int xBase = 99;               // x coordinate of the first histogram origin
  int yBase = 30 + height + 1;  // y coordinate of the first histogram origin

  drawHistogram(xBase, yBase, height, 0);
  drawHistogram(xBase + step + height, yBase, height, 1);
  drawHistogram(xBase, yBase + step + height + 1, height, 2);
  drawHistogram(xBase + step + height, yBase + step + height + 1, height, 3);
}

void MyHistogram::drawHistogram(int xBase, int yBase, int height, int flag) {
  QPainter painter(this);
  QString text;
  int *temp;
  switch (flag) {
    case 0:
      painter.setPen(Qt::black);
      text = "Gray Histogram";
      temp = grayHistogram;
      break;
    case 1:
      painter.setPen(Qt::red);
      text = "Red Histogram";
      temp = redHistogram;
      break;
    case 2:
      painter.setPen(Qt::green);
      text = "Green Histogram";
      temp = greenHistogram;
      break;
    case 3:
      painter.setPen(Qt::blue);
      text = "Blue Histogram";
      temp = blueHistogram;
      break;
    default:
      break;
  }
  float max_ = max(max(grayHistogram[257], redHistogram[257]),
                   max(greenHistogram[257], blueHistogram[257]));
  for (int i = 0; i < 256; i++) {
    painter.drawLine(xBase + 1 + i, yBase, xBase + 1 + i,
                     yBase - (float)(256. / max_) * (float)temp[i]);
  }

  painter.drawText(xBase, yBase + 25, tr("dark"));
  painter.drawText(xBase + 225, yBase + 25, tr("light"));

  painter.setPen(Qt::black);

  painter.drawText(xBase + 25, yBase - height - 10, text);

  painter.drawText(xBase + 100, yBase + 15, tr("Intensity"));
  painter.drawText(xBase - 84, yBase - height / 2 + 5, tr("Pixels count"));

  // abscissa
  painter.drawLine(xBase, yBase, xBase + 256 + 1, yBase);
  painter.drawLine(xBase, yBase + 1, xBase + 256 + 1, yBase + 1);

  // left ordinate
  painter.drawLine(xBase, yBase, xBase, yBase - height);
  painter.drawLine(xBase - 1, yBase, xBase - 1, yBase - height);

  // right ordinate
  painter.drawLine(xBase + 256 + 1, yBase, xBase + 256 + 1, yBase - height);
  painter.drawLine(xBase + 256 + 2, yBase, xBase + 256 + 2, yBase - height);

  // left ordinate arrow
  painter.drawLine(xBase, yBase - height, xBase + 4, yBase - height + 7);
  painter.drawLine(xBase - 1, yBase - height, xBase - 1 - 4,
                   yBase - height + 7);

  // right ordinate arrow
  painter.drawLine(xBase + 256 + 1, yBase - height, xBase + 256 + 1 - 4,
                   yBase - height + 7);
  painter.drawLine(xBase + 256 + 2, yBase - height, xBase + 256 + 2 + 4,
                   yBase - height + 7);
}
