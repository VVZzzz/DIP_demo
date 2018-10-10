#ifndef DLG_SLIDERBAR_H
#define DLG_SLIDERBAR_H

#include <QDialog>

namespace Ui {
class Dlg_Sliderbar;
}

class Dlg_Sliderbar : public QDialog {
  Q_OBJECT

 public:
  explicit Dlg_Sliderbar(QWidget *parent = nullptr);
  ~Dlg_Sliderbar();

 private:
  Ui::Dlg_Sliderbar *ui;
};

#endif  // DLG_SLIDERBAR_H
