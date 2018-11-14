#ifndef RUNTOOL_H
#define RUNTOOL_H
#include <QColor>
#include <QImage>
#include <QRgb>
#include <QStringList>

namespace RunTool {
#define max(a, b) a > b ? a : b
#define min(a, b) a < b ? a : b
void rgb2gray(QImage& img);
QImage regray(QImage& img, int n);
QImage deresolution(QImage& img, int n);
QImage adddenoise(QImage& img, QStringList& fileslist);
QImage medianFilter(QImage& img, int radius);
QImage meanFilter(QImage& img, int radius);
QImage diff(QImage& img, QImage& secondPic);
QImage laplaceFilter(QImage& img);
}  // namespace RunTool

#endif  // RUNTOOL_H
