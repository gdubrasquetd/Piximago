#include <QRadioButton>
#include <QString>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QBoxLayout>
#include <QLabel>

#include "radio-button-widget.h"

RadioButtonWidget::RadioButtonWidget(QList<QString> radioNameList, QString name, int orientation, QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    setMaximumHeight(250);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setLayout(layout);

    QBoxLayout *buttonsLayout;

    if(orientation == 0){
        buttonsLayout = new QHBoxLayout;
    }else{
        buttonsLayout = new QVBoxLayout;
    }
    buttonsLayout->setContentsMargins(0, 0, 0, 0);
    buttonsLayout->setSpacing(0);
    nameLabel = new QLabel(name + " :");
    nameLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    nameLabel->setMaximumHeight(16);

    layout->addWidget(nameLabel);

    buttonGroup = new QButtonGroup();
    buttonGroup->setExclusive(true);

    for(int i = 0; i < radioNameList.size(); i++){
        QRadioButton *button = new QRadioButton(radioNameList.at(i));
        radioButtons.append(button);
        buttonGroup->addButton(button, i);

        if(orientation == 0){
            buttonsLayout->addWidget(button, 0, Qt::AlignHCenter);
        }else{
            buttonsLayout->addWidget(button);
        }
        
    }
    if(orientation == 0){
        layout->addLayout(buttonsLayout);
    }else{
        QWidget *centerWidget = new QWidget;
        centerWidget->setLayout(buttonsLayout);
        layout->addWidget(centerWidget, 0, Qt::AlignHCenter);
    }

}

bool RadioButtonWidget::isActive(){
    return this->active;
}

void RadioButtonWidget::setActive(bool status){
    if(status){
        this->buttonGroup->button(0)->setChecked(true);
        this->show();
    }else{
        this->hide();
    }
    this->active = status;
}

QList<QRadioButton *> RadioButtonWidget::getRadioButtonsList(){
    return radioButtons;
}

int RadioButtonWidget::getCurrentChecked(){
    return buttonGroup->checkedId();
}
