﻿#include "myfilter.h"
#include <QDebug>
#include <algorithm>
#include <queue>
#include <set>
#include <vector>
#include "runtool.h"

MyFilter::MyFilter() {
  extendimg = nullptr;
  radius = 0;
  filterType = MEAN;
}

MyFilter::~MyFilter() {
  if (extendimg != nullptr) {
    for (int j = 0; j < h + (radius * 2); j++) {
      for (int k = 0; k < w + (radius * 2); k++) {
        delete[] extendimg[j][k];
      }
      delete[] extendimg[j];
    }
    delete extendimg;
    extendimg = nullptr;
  }
}

MyFilter::MyFilter(MyFilter::FILTER type, int r, const QImage &img)
    : newimg(img.width(), img.height(), QImage::Format_RGB32) {
  // : newimg(img) {
  // RunTool::rgb2gray(newimg);
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

  //初始化extendImg

  extendimg = new uchar **[h + (r << 1)];  //[0]:b [1]:g [2]:r
  for (int i = 0; i < h + (r << 1); i++) {
    extendimg[i] = new uchar *[w + (r << 1)];
    for (int j = 0; j < w + (r << 1); j++) {
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
      extendimg[j + r][i][0] = *(originlpix + i * 4);
      extendimg[j + r][i][1] = *(originlpix + i * 4 + 1);
      extendimg[j + r][i][2] = *(originlpix + i * 4 + 2);
      extendimg[j + r][i_ + 2 * r][0] = *(originlpix + i_ * 4);
      extendimg[j + r][i_ + 2 * r][1] = *(originlpix + i_ * 4 + 1);
      extendimg[j + r][i_ + 2 * r][2] = *(originlpix + i_ * 4 + 2);
      /*
      extendimg[j + r][i][0] = 0;
      extendimg[j + r][i][1] = 0;
      extendimg[j + r][i][2] = 0;
      extendimg[j + r][i_ + 2 * r][0] = 0;
      extendimg[j + r][i_ + 2 * r][1] = 0;
      extendimg[j + r][i_ + 2 * r][2] = 0;
    */
    }
  }
  //再上下边界
  for (int i = 0, i_ = h; i < r; i++, i_++)
    for (int j = 0; j < w + (r << 1); j++) {
      extendimg[i][j][0] = extendimg[i + r][j][0];
      extendimg[i][j][1] = extendimg[i + r][j][1];
      extendimg[i][j][2] = extendimg[i + r][j][2];
      extendimg[i_ + r][j][0] = extendimg[i_][j][0];
      extendimg[i_ + r][j][1] = extendimg[i_][j][1];
      extendimg[i_ + r][j][2] = extendimg[i_][j][2];
      /*
      extendimg[i][j][0] = 0;
      extendimg[i][j][1] = 0;
      extendimg[i][j][2] = 0;
      extendimg[i_ + r][j][0] = 0;
      extendimg[i_ + r][j][1] = 0;
      extendimg[i_ + r][j][2] = 0;
    */
    }
}

QImage MyFilter::applyFilter() {
  std::set<uchar> q_g, q_b, q_r;
  switch (filterType) {
    case MEDIAN:
      for (int j = radius; j < h + radius; j++) {
        uchar *originlpix = newimg.scanLine(j - radius);
        for (int i = radius; i < w + radius; i++) {
          q_b.clear();
          q_g.clear();
          q_r.clear();
          for (int i_ = -1 * radius; i_ <= radius; i_++)
            for (int j_ = -1 * radius; j_ <= radius; j_++) {
              q_b.insert(extendimg[j + j_][i + i_][0]);
              q_g.insert(extendimg[j + j_][i + i_][1]);
              q_r.insert(extendimg[j + j_][i + i_][2]);
            }
          auto b_itr = q_b.begin();
          auto g_itr = q_g.begin();
          auto r_itr = q_r.begin();
          int c = 0, d = 0, e = 0;
          while (c++ <= (q_b.size() >> 1)) b_itr++;
          while (d++ <= (q_g.size() >> 1)) g_itr++;
          while (e++ <= (q_r.size() >> 1)) r_itr++;

          *(originlpix + (i - radius) * 4) = *(b_itr);
          *(originlpix + (i - radius) * 4 + 1) = *(g_itr);
          *(originlpix + (i - radius) * 4 + 2) = *(r_itr);
        }
      }
      break;
    case MEAN:
      for (int i = radius; i < h + radius; i++) {
        uchar *originlpix = newimg.scanLine(i - radius);
        for (int j = radius; j < w + radius; j++) {
          int b_ = 0, g_ = 0, r_ = 0;
          for (int i_ = -1 * radius; i_ <= radius; i_++)
            for (int j_ = -1 * radius; j_ <= radius; j_++) {
              b_ += (extendimg[i + i_][j + j_][0]);
              g_ += (extendimg[i + i_][j + j_][1]);
              r_ += (extendimg[i + i_][j + j_][2]);
            }
          b_ /= (2 * radius + 1) * (2 * radius + 1);
          g_ /= (2 * radius + 1) * (2 * radius + 1);
          r_ /= (2 * radius + 1) * (2 * radius + 1);

          *(originlpix + (j - radius) * 4) = b_;
          *(originlpix + (j - radius) * 4 + 1) = g_;
          *(originlpix + (j - radius) * 4 + 2) = r_;
        }
      }

      break;
    case LAPLACE:
      for (int i = radius; i < h + radius; i++) {
        uchar *originlpix = newimg.scanLine(i - radius);
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

          *(originlpix + (j - radius) * 4) = 0;
          *(originlpix + (j - radius) * 4 + 1) = 0;
          *(originlpix + (j - radius) * 4 + 2) = 0;
        }
      }
      break;
    default:
      break;
  }
  return newimg;
}
