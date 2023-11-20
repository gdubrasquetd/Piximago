#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "slider-widget.h"


SliderWidget::SliderWidget(int minValue, int maxValue, int defaultValue, const QString &name, bool live, QWidget *parent)
    : QWidget(parent)
{
    this->minValue = minValue;
    this->maxValue = maxValue;
    this->defaultValue = defaultValue;
    this->live = live;
    this->setMaximumHeight(80);

    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    nameLabel = new QLabel(name + " :");
    nameLabel->setStyleSheet("font-weight: bold; font-size: 16px;");

    valueLabel = new QLabel(QString::number(this->defaultValue));
    valueLabel->setAlignment(Qt::AlignHCenter);

    layout->addWidget(nameLabel);
    layout->addWidget(valueLabel);

    // layout->addStretch(1);

    slider = new QSlider(Qt::Horizontal);
    slider->setRange(minValue, maxValue);
    slider->setValue(this->defaultValue);

    QHBoxLayout *sliderLayout = new QHBoxLayout;
    labelLeft = new QLabel(QString::number(minValue));
    labelRight = new QLabel(QString::number(maxValue));

    sliderLayout->addWidget(labelLeft);
    sliderLayout->addWidget(slider);
    sliderLayout->addWidget(labelRight);

    this->updateValue(defaultValue);

    layout->addLayout(sliderLayout);

    connect(slider, &QSlider::valueChanged, this, &SliderWidget::updateValue);

}

void SliderWidget::updateValue(int newValue)
{
    this->value = newValue;
    this->valueLabel->setText(QString::number(newValue));
}

int SliderWidget::getValue(){
    return this->value;
}

QSlider* SliderWidget::getSlider(){
    return this->slider;
}

bool SliderWidget::isActive(){
    return this->active;
}

void SliderWidget::setActive(bool status){
    if(status){
        this->resetValue();
        this->show();
    }else{
        this->hide();
    }
    this->active = status;
}

void SliderWidget::resetValue(){
    updateValue(this->defaultValue);
    this->slider->setValue(this->defaultValue);
}