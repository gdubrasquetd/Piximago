#ifndef BUTTONSLAYOUTBUILDER_H
#define BUTTONSLAYOUTBUILDER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVariant>

#include "slider-widget.h"
#include "radio-button-widget.h"
#include "spin-box-widget.h"


class ButtonsLayoutBuilder : public QWidget {
    Q_OBJECT

public:
    ButtonsLayoutBuilder(QWidget *parent = nullptr);
    QList<QVariant> getButtonsList();


public slots:
    void updateButtonsLayout(int mode);
    void resetSliderValues();

private:
    QList<QVariant> buttonsList;

    SliderWidget *colorSlider1;
    SliderWidget *colorSlider2;
    SliderWidget *colorSlider3;

    SliderWidget *balanceSlider1;
    SliderWidget *balanceSlider2;

    RadioButtonWidget *convolutionRadioButton1;

    SliderWidget *crossSlider1;
    SliderWidget *crossSlider2;
    RadioButtonWidget *crossRadioButton3;
    SliderWidget *crossSlider4;

    SliderWidget *bandSlider1;
    SliderWidget *bandSlider2;
    RadioButtonWidget *bandRadioButton3;

    SliderWidget *stainSlider1;
    SliderWidget *stainSlider2;
    SliderWidget *stainSlider3;
    SliderWidget *stainSlider4;

    SpinBoxWidget *spinBox1;
    SpinBoxWidget *spinBox2;


signals:
};

/**
 * @fn QList<QVariant> ButtonsLayoutBuilder::getButtonsList()
 * @brief the button list contained only QVariant variables
 * @return the list of all the buttons used in the interface
*/

/**
 * @fn void ButtonsLayoutBuilder::updateButtonsLayout(int mode)
 * @brief modify the interface display by hiding some buttons depending on the mode currently selected by the user
 * @details depending to the origin type of these buttons, this action is performed by modifying the active type of each of them by true or false
 * @param mode [in] know the mode chosen by the user
*/

/**
 * @fn void ButtonsLayoutBuilder::resetSliderValues()
 * @brief for each button, reset the default value as at the opening of the application
 * @details reset the value for active buttons only, i.e. those displayed on the interface
*/

#endif // BUTTONSLAYOUTBUILDER_H
