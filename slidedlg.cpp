#include "slidedlg.h"
#include <QString>
#include "ui_slidedlg.h"

SlideDlg::SlideDlg(QWidget *parent, int min, int max)
    : QDialog(parent), ui(new Ui::SlideDlg) {
  ui->setupUi(this);
  ui->horizontalSlider->setMinimum(min);
  ui->horizontalSlider->setMaximum(max);
  ui->horizontalSlider->setValue(min);
  ui->label->setText(QString::number(ui->horizontalSlider->value()));
  setWindowTitle("Slidebar Dialog");
  connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this,
          SLOT(setlabel()));
}

SlideDlg::~SlideDlg() { delete ui; }

int SlideDlg::getPara() const { return ui->horizontalSlider->value(); }

void SlideDlg::setRange(int min, int max, int val) {
  ui->horizontalSlider->setMinimum(min);
  ui->horizontalSlider->setMaximum(max);
  ui->horizontalSlider->setValue(val);
}

const QSlider *SlideDlg::getSliderP() const {return ui->horizontalSlider;}

void SlideDlg::setlabel() {
  ui->label->setText(QString::number(ui->horizontalSlider->value()));
}
