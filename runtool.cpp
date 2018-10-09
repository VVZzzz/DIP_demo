#include "runtool.h"

namespace RunTool {

void rgb2gray(/*QImage &img*/) {
    /*
  unsigned w = img.width(), h = img.height();
  for (int y = 0; y < h; y++) {
    QRgb *lpix = img.scanLine(y);
    for (int x = 0; x < w; x++) {
      int tmppix = (qRed(*(lpix + x)) * 19595 + qGreen(*(lpix + x)) * 38469 +
                    qBlue(*(lpix + x)) * 7472) >>
                   16;
      *(lpix + x) = qRgb(tmppix, tmppix, tmppix);
    }
  }
  return;
  */
}

QImage regray(QImage &img, int n) {
  unsigned w = img.width(), h = img.height();
  RunTool::rgb2gray();
  QImage newimg(w, h, QImage::Format_ARGB32);
  int dis = 256 / n;
  int i = 0;
  for (int x = 0; x < w; x++) {
    QRgb *originlpix = img.scanLine(x);
    QRgb *newlpix = newimg.scanLine(x);
    for (int y = 0; y < h; y++) {
      i = 0;
      while (i * dis < qRed(*(originlpix + y)))
        ;
      i = ((i - 1) < 0 ? 0, i - 1)*dis;
      *(newlpix + y) = qRgb(i, i, i);
    }
  }
}

}  // namespace RunTool
