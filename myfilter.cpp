#include "myfilter.h"
#include <algorithm>
#include <queue>
#include <vector>

MyFilter::MyFilter() {
  extendimg = nullptr;
  newimg = nullptr;
  radius = 0;
  filterType = MEAN;
}

MyFilter::~MyFilter() {
  if (newimg) {
    delete[] newimg;
    newimg = nullptr;
  }

  /*
  if (extendimg) {
    for (int j = 0; j < h + (radius * 2); j++) {
      for (int k = 0; k < w + (radius * 2); k++) {
        delete[] extendimg[j][k];
      }
      delete[] extendimg[j];
    }
    delete extendimg;
    extendimg = nullptr;
  }
  */
}

MyFilter::MyFilter(MyFilter::FILTER type, int r, const QImage &img) {
  filterType = type;
  radius = r;
  w = img.width();
  h = img.height();
  switch (type) {
    case LAPLACE:
      filter = {{0, 1, 0}, {1, -4, 1}, {0, 1, 0}};
      r = 1;
      break;
    case MEDIAN:
    case MEAN:
      for (int i = 0; i < (r << 1) + 1; i++)
        filter.push_back(QVector<int>((r << 1) + 1, 1));
      break;
    default:
      break;
  }

  //初始化空的newimg
  newimg = new uchar[w * h * 4]{0};

  //初始化extendImg

  extendimg = new uchar **[h + (r * 2)];  //[0]:b [1]:g [2]:r
  for (int i = 0; i < h + (r * 2); i++) {
    extendimg[i] = new uchar *[w + (r * 2)];
    for (int j = 0; j < w + (r * 2); j++) {
      extendimg[i][j] = new uchar[3]{0};
    }
  }
  // extendImg的原图部分
  for (int j = 0; j < h; j++) {
    const uchar *originlpix = img.scanLine(j);
    for (int i = 0; i < w; i++) {
      extendimg[j + r][i + r][0] += *(originlpix + i * 4);
      extendimg[j + r][i + r][1] += *(originlpix + i * 4 + 1);
      extendimg[j + r][i + r][2] += *(originlpix + i * 4 + 2);
    }
  }
  // extendImg的扩展部分,扩展方式有多种,这里采用openCV中的BORDER_WRAP
  //先左右边界
  for (int i = 0, i_ = w - r; i < r; i++, i_++) {
    for (int j = 0; j < h; j++) {
      const uchar *originlpix = img.scanLine(j);
      extendimg[j][i][0] = *(originlpix + i * 4);
      extendimg[j][i][1] = *(originlpix + i * 4 + 1);
      extendimg[j][i][2] = *(originlpix + i * 4 + 2);
      extendimg[j][i_ + r][0] = *(originlpix + i_ * 4);
      extendimg[j][i_ + r][1] = *(originlpix + i_ * 4 + 1);
      extendimg[j][i_ + r][2] = *(originlpix + i_ * 4 + 2);
    }
  }
  //再上下边界
  for (int i = 0, i_ = h; i < r; i++, i_++)
    for (int j = 0; j < w + (r << 1); j++) {
      extendimg[i][j][0] = extendimg[i + r][j][0];
      extendimg[i][j][1] = extendimg[i + r][j][1];
      extendimg[i][j][2] = extendimg[i + r][j][2];
      extendimg[i_][j][0] = extendimg[i_ - r][j][0];
      extendimg[i_][j][1] = extendimg[i_ - r][j][1];
      extendimg[i_][j][2] = extendimg[i_ - r][j][2];
    }
}

QImage MyFilter::applyFilter() {
  switch (filterType) {
    case MEDIAN:
      for (int i = radius; i < h + radius; i++) {
        for (int j = radius; j < w + radius; j++) {
          std::vector<uchar> q_b, q_g, q_r;
          for (int i_ = -1 * radius; i_ <= radius; i_++)
            for (int j_ = -1 * radius; j_ <= radius; j_++) {
              q_b.push_back(extendimg[i + i_][j + j_][0]);
              q_g.push_back(extendimg[i + i_][j + j_][1]);
              q_r.push_back(extendimg[i + i_][j + j_][2]);
            }
          std::sort(q_b.begin(), q_b.end());
          std::sort(q_g.begin(), q_g.end());
          std::sort(q_r.begin(), q_r.end());

          newimg[((i - radius) * w + (j - radius)) << 2] = q_b[radius];
          newimg[(((i - radius) * w + (j - radius)) << 2) + 1] = q_g[radius];
          newimg[(((i - radius) * w + (j - radius)) << 2) + 2] = q_r[radius];
        }
      }
      break;
    case MEAN:
      for (int i = radius; i < h + radius; i++) {
        for (int j = radius; j < w + radius; j++) {
          int b_ = 0, g_ = 0, r_ = 0;
          for (int i_ = -1 * radius; i_ <= radius; i_++)
            for (int j_ = -1 * radius; j_ <= radius; j_++) {
              b_ += (extendimg[i + i_][j + j_][0]);
              g_ += (extendimg[i + i_][j + j_][1]);
              r_ += (extendimg[i + i_][j + j_][2]);
            }
          b_ /= radius * radius;
          g_ /= radius * radius;
          r_ /= radius * radius;
          newimg[(i - radius) * w * 4 + (j - radius) * 4] = b_;
          newimg[(i - radius) * w * 4 + (j - radius) * 4 + 1] = g_;
          newimg[(i - radius) * w * 4 + (j - radius) * 4 + 2] = r_;
        }
      }

      break;
    default:
      break;
  }
  return QImage(newimg, w, h, QImage::Format_RGB32);
}
