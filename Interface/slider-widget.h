#ifndef SLIDERWIDGET_H
#define SLIDERWIDGET_H

#include <QWidget>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>


class SliderWidget : public QWidget
{
    Q_OBJECT

public:
    SliderWidget(int minValue, int maxValue, int defaultValue, const QString &name, bool live, QWidget *parent = nullptr);
    void updateValue(int value);
    int getValue();
    QSlider* getSlider();
    bool isActive();
    void setActive(bool status);
    void resetValue();

private:
    int defaultValue;
    int minValue;
    int maxValue;
    int value = 0;
    bool active = true;
    bool live;
    QLabel *valueLabel;
    QSlider *slider;
    QLabel *labelLeft;
    QLabel *labelRight;
    QLabel *nameLabel;
};

/**
 * @fn void SliderWidget::updateValue(int newValue)
 * @brief update with a new value and modify the display text above the slider
 * @param newValue [in] new value to be set
*/

/**
 * @fn int SliderWidget::getValue()
 * @brief get the current value of the slider 
 * @return an integer corresponding to the value
*/

/**
 * @fn QSlider* SliderWidget::getSlider()
 * @brief obtain a reference to a created object of type QSlider
 * @return a pointer of type QSlider
*/

/**
 * @fn bool SliderWidget::isActive()
 * @brief return the status of the slider, i.e. whether or not it is displayed on the interface
 * @return true is the element is active (displayed), false otherwise (hidden)
*/

/**
 * @fn void SliderWidget::setActive(bool status)
 * @brief define whether the slider is displayed, or not, on the interface depending on the status
 * @param status [in] define if the slider should be displayed according to the selected mode
*/

/**
 * @fn void SliderWidget::resetValue()
 * @brief reset slider value with the default value of the slider 
 * @details used when the slider is displayed again for example
*/

#endif // SLIDERWIDGET_H
