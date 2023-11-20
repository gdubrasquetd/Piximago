#ifndef SPINWIDGET_H
#define SPINWIDGET_H

#include <QWidget>
#include <QObject>
#include <QSpinBox>

class SpinBoxWidget : public QWidget
{
    Q_OBJECT

public:
    SpinBoxWidget(QString name, QWidget *parent = nullptr);
    bool isActive();
    void setActive(bool status);
    void setValueByRatio(int ratio);
    QSpinBox* getSpinBox();
    int getValue();
    void resetValue();
    void setDefaultValue(int default_value);
    int getDefaultValue();

private:
    bool active = true;
    QSpinBox *spin_box;
    int defaultValue;
};

/**
 * @fn bool SpinBoxWidget::isActive()
 * @brief return the status of the spin box, i.e. whether or not it is displayed on the interface
 * @return true is the element is active (displayed), false otherwise (hidden)
*/

/**
 * @fn void SpinBoxWidget::setActive(bool status)
 * @brief define whether the box is displayed, or not, on the interface depending on the status
 * @param status [in] define if the box should be displayed according to the selected mode
*/

/**
 * @fn void SpinBoxWidget::setValueByRatio(int ratio)
 * @brief keep the image rationnally correct
 * @details width or height of the image are defined one from the other according to the ratio
 * @param ratio [in] give the ratio that the image must respect
*/

/**
 * @fn QSpinBox* SpinBoxWidget::getSpinBox()
 * @brief obtain a reference to a created object of type QSpinBox
 * @return a pointer of type QSpinBox
*/

/**
 * @fn int SpinBoxWidget::getValue()
 * @brief Allow to get the current value indicated by the spin box 
 * @return an integer corresponding to the value
*/

/**
 * @fn void SpinBoxWidget::resetValue()
 * @brief reset spin box value with the default value 
 * @details used when the box is displayed again for example
*/

/**
 * @fn void SpinBoxWidget::setDefaultValue(int default_value)
 * @brief set the value displayed in the spin box
 * @details this value is set according to the width or height of the original image
 * @param default_value [in] the value of the spin box to be set
*/

/**
 * @fn int SpinBoxWidget::getDefaultValue()
 * @brief get the default value of the spin box
 * @return an integer corresponding to the default value set for the spin box
*/



#endif // SPINWIDGET_H
