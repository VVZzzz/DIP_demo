#ifndef SLIDEDLG_H
#define SLIDEDLG_H

#include <QDialog>
#include <QSlider>

namespace Ui {
class SlideDlg;
}

class SlideDlg : public QDialog {
  Q_OBJECT

 public:
  explicit SlideDlg(QWidget *parent = nullptr, int min = 1, int max = 100);
  ~SlideDlg();
  int getPara() const;
  void setRange(int min, int max, int val);
  const QSlider *getSliderP() const;
 private slots:
  void setlabel();

 private:
  Ui::SlideDlg *ui;
};

#endif  // SLIDEDLG_H
