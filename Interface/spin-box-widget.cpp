#include "spin-box-widget.h"

#include <QSpinBox>
#include <QVBoxLayout>
#include <QLabel>


SpinBoxWidget::SpinBoxWidget(QString name, QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    QLabel *nameLabel = new QLabel(name + " :");
    nameLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    nameLabel->setMaximumHeight(20);

    layout->addWidget(nameLabel);

    spin_box = new QSpinBox(this);
    spin_box->setRange(0, 9999);
    spin_box->setValue(1);
    spin_box->setSuffix(" pixels");

    layout->addWidget(spin_box);
}

bool SpinBoxWidget::isActive(){
    return this->active;
}

void SpinBoxWidget::setActive(bool status){
    if(status){
        spin_box->setValue(this->defaultValue);
        this->show();
    }else{
        this->hide();
    }
    this->active = status;
}

void SpinBoxWidget::setValueByRatio(int ratio){
   this->spin_box->setValue(this->defaultValue*ratio);
}

int SpinBoxWidget::getValue(){
    return this->spin_box->value();
}

QSpinBox* SpinBoxWidget::getSpinBox(){
    return this->spin_box;
}

void SpinBoxWidget::resetValue(){
    this->spin_box->setValue(this->defaultValue);
}

void SpinBoxWidget::setDefaultValue(int default_value){
    this->defaultValue = default_value;
}

int SpinBoxWidget::getDefaultValue(){
    return this->defaultValue;
}
