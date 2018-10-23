#include "runtool.h"
#include "myfilter.h"

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
      uchar b = *(originlpix + i * n * 4);
      uchar g = *(originlpix + i * n * 4 + 1);
      uchar r = *(originlpix + i * n * 4 + 2);
      for (int j_ = 0; (j_ < n) && ((n * j + j_) < h); j_++) {
        uchar *newlpix = newimg.scanLine(n * j + j_);
        for (int k_ = 0; (k_ < n) && ((n * i + k_) < w); k_++) {
          *(newlpix + (n * i + k_) * 4) = b;
          *(newlpix + (n * i + k_) * 4 + 1) = g;
          *(newlpix + (n * i + k_) * 4 + 2) = r;
        }
      }
    }
  }
  return newimg;
}

QImage adddenoise(QImage &img, QStringList &fileslist) {
  unsigned w = img.width(), h = img.height();
  QImage newimg(w, h, QImage::Format_RGB32);

  int ***pVal = new int **[h];  //[0]:b [1]:g [2]:r
  for (int i = 0; i < h; i++) {
    pVal[i] = new int *[w];
    for (int j = 0; j < w; j++) {
      pVal[i][j] = new int[3]{0};
    }
  }

  QStringList::const_iterator constIterator;
  for (constIterator = fileslist.constBegin();
       constIterator != fileslist.constEnd(); constIterator++) {
    QImage *pImg = new QImage(*constIterator);
    for (int j = 0; j < h; j++) {
      uchar *originlpix = pImg->scanLine(j);
      for (int i = 0; i < w; i++) {
        pVal[j][i][0] += *(originlpix + i * 4);
        pVal[j][i][1] += *(originlpix + i * 4 + 1);
        pVal[j][i][2] += *(originlpix + i * 4 + 2);
      }
    }
    delete pImg;
  }

  for (int j = 0; j < h; j++) {
    uchar *originlpix = img.scanLine(j);
    uchar *newlpix = newimg.scanLine(j);
    for (int i = 0; i < w; i++) {
      pVal[j][i][0] += *(originlpix + i * 4);
      *(newlpix + i * 4) = pVal[j][i][0] / (fileslist.size() + 1);

      pVal[j][i][1] += *(originlpix + i * 4 + 1);
      *(newlpix + i * 4 + 1) = pVal[j][i][1] / (fileslist.size() + 1);
      pVal[j][i][2] += *(originlpix + i * 4 + 2);
      *(newlpix + i * 4 + 2) = pVal[j][i][2] / (fileslist.size() + 1);
    }
  }
  for (int j = 0; j < h; j++) {
    for (int k = 0; k < w; k++) {
      delete[] pVal[j][k];
    }
    delete[] pVal[j];
  }
  delete pVal;

  return newimg;
}

QImage medianFilter(QImage &img, int radius) {
  MyFilter medianfilter(MyFilter::FILTER::MEDIAN, radius, img);
  return medianfilter.applyFilter();
}

}  // namespace RunTool
