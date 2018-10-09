#ifndef RUNTOOL_H
#define RUNTOOL_H
#include <QColor>
#include <QImage>
#include <QRgb>

namespace RunTool {
//void rgb2gray(QImage& img);
void rgb2gray();
QImage regray(QImage& img, int n);

}  // namespace RunTool

#endif  // RUNTOOL_H
