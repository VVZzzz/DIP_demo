#include "mygraphicsview.h"
#include <QDebug>
#include <QPoint>
#include <QtMath>

MyGraphicsView::MyGraphicsView(QWidget *parent)
    : QGraphicsView(parent), para(0) {}

void MyGraphicsView::wheelEvent(QWheelEvent *e) {
  QPoint numDegrees = e->angleDelta() / 8;
  QPoint numSteps = numDegrees / 15;
  qreal val = pow(1.2, numSteps.y());
  this->scale(val, val);  //放大缩小
}
