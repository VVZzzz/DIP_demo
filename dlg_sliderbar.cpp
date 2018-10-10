#include "dlg_sliderbar.h"
#include "ui_dlg_sliderbar.h"

Dlg_Sliderbar::Dlg_Sliderbar(QWidget *parent)
    : QDialog(parent), ui(new Ui::Dlg_Sliderbar) {
  ui->setupUi(this);
}

Dlg_Sliderbar::~Dlg_Sliderbar() { delete ui; }
