#include "myhistogram.h"
#include <QPainter>
#include "runtool.h"

MyHistogram::MyHistogram(QWidget *parent) : QLabel(parent) {
  grayHistogram[256] = redHistogram[256] = greenHistogram[256] =
      blueHistogram[256] = 256;
  grayHistogram[257] = redHistogram[257] = greenHistogram[257] =
      blueHistogram[257] = -1;
  grayHistogram[258] = redHistogram[258] = greenHistogram[258] =
      blueHistogram[258] = -1;
  width = 0;
  height = 0;
}

void MyHistogram::setHistogram(const QImage &img) {
  QImage grayImg(img);
  RunTool::rgb2gray(grayImg);
  unsigned w = img.width(), h = img.height();
  width = w;
  height = h;
  for (int y = 0; y < h; y++) {
    const uchar *lpix = img.scanLine(y);
    const uchar *lpix2 = grayImg.scanLine(y);
    for (int x = 0; x < w; x++) {
      uchar red = *(lpix + x * 4 + 2);
      uchar green = *(lpix + x * 4 + 1);
      uchar blue = *(lpix + x * 4);
      uchar gray = *(lpix2 + x * 4);
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
  for (int i = 0; i < 256; i++) {
    if (grayHistogram[i] > grayHistogram[258])
      grayHistogram[258] = grayHistogram[i];
    if (redHistogram[i] > redHistogram[258])
      redHistogram[258] = redHistogram[i];
    if (greenHistogram[i] > greenHistogram[258])
      greenHistogram[258] = greenHistogram[i];
    if (blueHistogram[i] > blueHistogram[258])
      blueHistogram[258] = blueHistogram[i];
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
      painter.drawText(xBase + 40, yBase - height - 10,
                       tr("GRAY LEVELS HISTOGRAM"));
      painter.drawText(xBase + 100, yBase + 15, tr("Intensity"));
      painter.drawText(xBase - 84, yBase - height / 2 + 5, tr("Pixels count"));
      break;
    case 1:
      painter.setPen(Qt::red);
      text = "Red Histogram";
      temp = redHistogram;
      painter.drawText(xBase + 25, yBase - height - 10,
                       tr("RED COMPONENT HISTOGRAM"));
      painter.drawText(xBase + 100, yBase + 15, tr("Intensity"));
      painter.drawText(xBase - 84, yBase - height / 2 + 5, tr("Pixels count"));
      break;
    case 2:
      painter.setPen(Qt::green);
      text = "Green Histogram";
      temp = greenHistogram;
      painter.drawText(xBase + 15, yBase - height - 10,
                       tr("GREEN COMPONENT HISTOGRAM"));

      painter.drawText(xBase + 100, yBase + 15, tr("Intensity"));
      painter.drawText(xBase - 84, yBase - height / 2 + 5, tr("Pixels count"));
      break;
    case 3:
      painter.setPen(Qt::blue);
      text = "Blue Histogram";
      temp = blueHistogram;
      painter.drawText(xBase + 20, yBase - height - 10,
                       tr("BLUE COMPONENT HISTOGRAM"));
      painter.drawText(xBase + 100, yBase + 15, tr("Intensity"));
      painter.drawText(xBase - 84, yBase - height / 2 + 5, tr("Pixels count"));
      break;
    default:
      break;
  }
  float max_ = max(max(grayHistogram[258], redHistogram[258]),
                   max(greenHistogram[258], blueHistogram[258]));
  for (int i = 0; i < 256; i++) {
    painter.drawLine(xBase + 1 + i, yBase, xBase + 1 + i,
                     yBase - (float)(256. / max_) * (float)(*(temp + i)));
  }

  painter.drawText(xBase, yBase + 25, tr("dark"));
  painter.drawText(xBase + 225, yBase + 25, tr("light"));

  painter.setPen(Qt::black);

  /*
  painter.drawText(xBase + 25, yBase - height - 10, text);

  painter.drawText(xBase + 100, yBase + 15, tr("Intensity"));
  painter.drawText(xBase - 84, yBase - height / 2 + 5, tr("Pixels count"));
  */

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

QImage MyHistogram::histEqual(const QImage &img) {
  QImage newimg(width, height, QImage::Format_RGB32);
  uchar B_Transfer[256] = {0};  //定义一个Tansfer数组，用来保存转换后的灰度值
  uchar G_Transfer[256] = {0};
  uchar R_Transfer[256] = {0};

  for (int i = 0; i < 255; i++) {
    int R_sum = 0;
    int G_sum = 0;
    int B_sum = 0;
    for (int j = 0; j <= i; j++) {
      B_sum += blueHistogram[j];
      G_sum += greenHistogram[j];
      R_sum += redHistogram[j];
    }
    B_Transfer[i] = (int)(B_sum * 255 / (width * height) +
                          0.5);  //即int(a+0.5)是对a进行四舍五入
    G_Transfer[i] = (int)(G_sum * 255 / (width * height) + 0.5);
    R_Transfer[i] = (int)(R_sum * 255 / (width * height) + 0.5);
  }

  for (int j = 0; j < height; j++) {
    const uchar *originlpix = img.scanLine(j);
    uchar *newlpix = newimg.scanLine(j);
    for (int k = 0; k < width; k++) {
      *(newlpix + k * 4) = B_Transfer[*(originlpix + k * 4)];
      *(newlpix + k * 4 + 1) = G_Transfer[*(originlpix + k * 4 + 1)];
      *(newlpix + k * 4 + 2) = R_Transfer[*(originlpix + k * 4 + 2)];
    }
  }
  return newimg;
}
