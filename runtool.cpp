#include "runtool.h"

namespace RunTool {

void rgb2gray(QImage &img) {
  unsigned w = img.width(), h = img.height();
  for (int y = 0; y < h; y++) {
    uchar *lpix = img.scanLine(y);
    for (int x = 0; x < w; x++) {
      int tmppix = ((*(lpix + x * 4 + 2)) * 19595 +
                    (*(lpix + x * 4 + 1)) * 38469 + (*(lpix + x * 4)) * 7472) >>
                   16;
      *(lpix + x * 4) = tmppix;
      *(lpix + x * 4 + 1) = tmppix;
      *(lpix + x * 4 + 2) = tmppix;
    }
  }
}

QImage regray(QImage &img, int n) {
  unsigned w = img.width(), h = img.height();
  RunTool::rgb2gray(img);
  QImage newimg(w, h, QImage::Format_RGB32);
  int dis = 256 / n;
  int i = 0;
  for (int y = 0; y < h; y++) {
    uchar *originlpix = img.scanLine(y);
    uchar *newlpix = newimg.scanLine(y);
    for (int x = 0; x < w; x++) {
      i = 0;
      while (i * dis < *(originlpix + 4 * x)) i++;
      i = ((i - 1) < 0 ? 0 : i - 1) * dis;
      *(newlpix + x * 4) = i;
      *(newlpix + x * 4 + 1) = i;
      *(newlpix + x * 4 + 2) = i;
    }
  }
  return newimg;
}

/*
 * 降低分辨率,w*h->(w/n)*(h/n)
 * */
QImage deresolution(QImage &img, int n) {
  unsigned w = img.width(), h = img.height();
  QImage newimg(w, h, QImage::Format_RGB32);
  for (int j = 0; j * n < h; j++) {
    uchar *originlpix = img.scanLine(n * j);
    for (int i = 0; i * n < w; i++) {
      uchar r = *(originlpix + i * n * 4);
      uchar g = *(originlpix + i * n * 4 + 1);
      uchar b = *(originlpix + i * n * 4 + 2);
      for (int j_ = 0; (j_ < n) && ((n * j + j_) < h); j_++) {
        uchar *newlpix = newimg.scanLine(n * j + j_);
        for (int k_ = 0; (k_ < n) && ((n * i + k_) < w); k_++) {
          *(newlpix + (n * i + k_) * 4) = r;
          *(newlpix + (n * i + k_) * 4 + 1) = g;
          *(newlpix + (n * i + k_) * 4 + 2) = b;
        }
      }
    }
  }
  return newimg;
}

}  // namespace RunTool
