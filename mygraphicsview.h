#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H
#include <QGraphicsView>
#include <QWheelEvent>

//重载一些事件函数,包括鼠标滚轮等
class MyGraphicsView : public QGraphicsView {
  Q_OBJECT
 private:
  int para;

 public:
  MyGraphicsView(QWidget *parent = nullptr);

 protected:
  virtual void wheelEvent(QWheelEvent *e);
};

#endif  // MYGRAPHICSVIEW_H
