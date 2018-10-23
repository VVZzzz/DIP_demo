#ifndef MYFILTER_H
#define MYFILTER_H
#include <QImage>
#include <QVector>
//滤波器通用模板
class MyFilter {
 public:
  enum FILTER { MEDIAN, MEAN, LAPLACE };
  MyFilter();
  ~MyFilter();
  MyFilter(FILTER type, int r, const QImage &img);
  QImage applyFilter();

 private:
  int radius;                    //滤波器半径
  QVector<QVector<int>> filter;  //滤波器
  uchar ***extendimg;            //扩展后的img
  uchar *newimg;                 //处理后的img
  int w;                         //图像宽度
  int h;                         //图像高度
  FILTER filterType;             //滤波器类型
};

#endif  // MYFILTER_H
