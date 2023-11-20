#include <QVBoxLayout>
#include <QLabel>
#include <QString>
#include <QPushButton>
#include <QVariant>
#include <QRadioButton>

#include "buttons-layout-builder.h"
#include "slider-widget.h"
#include "radio-button-widget.h"


ButtonsLayoutBuilder::ButtonsLayoutBuilder(QWidget *parent) : QWidget(parent)
{

    colorSlider1 = new SliderWidget(0, 200, 100, QString("R Channel"), true);
    colorSlider2 = new SliderWidget(0, 200, 100, QString("G Channel"), true);
    colorSlider3 = new SliderWidget(0, 200, 100, QString("B Channel"), true);

    balanceSlider1 = new SliderWidget(0, 1000, 100, QString("Contrast"), true);
    balanceSlider2 = new SliderWidget(-255, 255, 0, QString("Brightness"), true);

    convolutionRadioButton1 = new RadioButtonWidget({"Gaussian Blur", "Emboss effect", "Sharpening Effect", 
                                                        "Edge Enhancement Effect", "Edge Detection (Laplacian)", 
                                                            "Sobel Top", "Sobel Bot", "Sobel Right", "Sobel Left"}, "Convolution kernel", 1);

    crossSlider1 = new SliderWidget(0, 100, 5, QString("Radius"), false);
    crossSlider2 = new SliderWidget(0, 100, 5, QString("Center"), false);
    crossRadioButton3 = new RadioButtonWidget({"Cross", "V Line", "H Line"}, "Mode", 0);
    crossSlider4 = new SliderWidget(0, 100, 50, QString("Position"), false);

    bandSlider1 = new SliderWidget(0, 100, 25, QString("Min"), false);
    bandSlider2 = new SliderWidget(0, 100, 50, QString("Max"), false);
    bandRadioButton3 = new RadioButtonWidget({"Exclude", "Include"}, "Mode", 0);

    stainSlider1 = new SliderWidget(0, 100, 50, QString("R Threshold"), false);
    stainSlider2 = new SliderWidget(0, 100, 50, QString("G Threshold"), false);
    stainSlider3 = new SliderWidget(0, 100, 50, QString("B Threshold"), false);
    stainSlider4 = new SliderWidget(0, 10, 5, QString("Radius"), false);

    spinBox1 = new SpinBoxWidget("Width");
    spinBox2 = new SpinBoxWidget("Height");

    buttonsList << QVariant::fromValue(colorSlider1) << QVariant::fromValue(colorSlider2) << QVariant::fromValue(colorSlider3) 
                << QVariant::fromValue(balanceSlider1) << QVariant::fromValue(balanceSlider2)
                << QVariant::fromValue(convolutionRadioButton1)
                << QVariant::fromValue(crossSlider1) << QVariant::fromValue(crossSlider2) << QVariant::fromValue(crossRadioButton3) << QVariant::fromValue(crossSlider4)
                << QVariant::fromValue(bandSlider1) << QVariant::fromValue(bandSlider2) << QVariant::fromValue(bandRadioButton3)
                << QVariant::fromValue(stainSlider1) << QVariant::fromValue(stainSlider2) << QVariant::fromValue(stainSlider3) << QVariant::fromValue(stainSlider4)
                << QVariant::fromValue(spinBox1) << QVariant::fromValue(spinBox2); 
    
    QVBoxLayout *layout = new QVBoxLayout;

    QPushButton *resetButton = new QPushButton("Reset Parameters");
    connect(resetButton, &QPushButton::clicked, this, &ButtonsLayoutBuilder::resetSliderValues);

    layout->addWidget(resetButton);

    layout->addWidget(colorSlider1);
    layout->addWidget(colorSlider2);
    layout->addWidget(colorSlider3);

    layout->addWidget(balanceSlider1);
    layout->addWidget(balanceSlider2);
    layout->addWidget(convolutionRadioButton1);

    layout->addWidget(crossSlider1);
    layout->addWidget(crossSlider2);
    layout->addWidget(crossRadioButton3);
    layout->addWidget(crossSlider4);

    layout->addWidget(bandSlider1);
    layout->addWidget(bandSlider2);
    layout->addWidget(bandRadioButton3);

    layout->addWidget(stainSlider1);
    layout->addWidget(stainSlider2);
    layout->addWidget(stainSlider3);
    layout->addWidget(stainSlider4);

    layout->addWidget(spinBox1);
    layout->addWidget(spinBox2);

    this->updateButtonsLayout(0);

    setLayout(layout);

}

 void ButtonsLayoutBuilder::updateButtonsLayout(int mode){
    foreach(QVariant button, buttonsList){
        if(button.userType() == qMetaTypeId<SliderWidget *>()){
            SliderWidget *slider = button.value<SliderWidget *>();
            slider->setActive(false);
        }else if(button.userType() == qMetaTypeId<RadioButtonWidget *>()){
            RadioButtonWidget *radioGroup = button.value<RadioButtonWidget *>();
            radioGroup->setActive(false);
        }else{
            SpinBoxWidget *spin = button.value<SpinBoxWidget *>();
            spin->setActive(false);
        }
    }

    if(mode == 0){
        colorSlider1->setActive(true);
        colorSlider2->setActive(true);
        colorSlider3->setActive(true);
    }
    if(mode == 1){
        balanceSlider1->setActive(true);
        balanceSlider2->setActive(true);
    }
    if(mode == 2){
        convolutionRadioButton1->setActive(true);
    }
    if(mode == 3){
        crossSlider1->setActive(true);
        crossSlider2->setActive(true);
        crossRadioButton3->setActive(true);
        crossSlider4->setActive(true);
    }
    if(mode == 4){
        bandSlider1->setActive(true);
        bandSlider2->setActive(true);
        bandRadioButton3->setActive(true);
    }
    if(mode == 5){
        stainSlider1->setActive(true);
        stainSlider2->setActive(true);
        stainSlider3->setActive(true);
        stainSlider4->setActive(true);
    }
    if(mode == 6){
        spinBox1->setActive(true);
        spinBox2->setActive(true);
    }
}


QList<QVariant>  ButtonsLayoutBuilder::getButtonsList(){
    return buttonsList;
}

void ButtonsLayoutBuilder::resetSliderValues(){
    foreach(QVariant button, buttonsList){
        if(button.userType() == qMetaTypeId<SliderWidget *>()){
            SliderWidget *slider = button.value<SliderWidget *>();
            if(slider->isActive()){
                slider->resetValue();
            } 
        }
        if(button.userType() == qMetaTypeId<SpinBoxWidget *>()){
            SpinBoxWidget *spin = button.value<SpinBoxWidget *>();
            if(spin->isActive()){
                spin->resetValue();
            }
        }
        if(button.userType() == qMetaTypeId<RadioButtonWidget *>()){
            RadioButtonWidget *radio = button.value<RadioButtonWidget *>();
            radio->getRadioButtonsList()[0]->setChecked(true);
        }
    }
}
