#ifndef RADIOBUTTONWIDGET_H
#define RADIOBUTTONWIDGET_H

#include <QWidget>
#include <QObject>
#include <QButtonGroup>
#include <QRadioButton>
#include <QLabel>


class RadioButtonWidget : public QWidget
{
    Q_OBJECT

public:
    RadioButtonWidget(QList<QString> radioNameList, QString name, int orientation, QWidget *parent = nullptr);
    bool isActive();
    void setActive(bool status);
    QList<QRadioButton *> getRadioButtonsList();
    int getCurrentChecked();

private:
    QButtonGroup *buttonGroup;
    bool active = true;
    QList<QRadioButton *> radioButtons;
    QLabel *nameLabel;
};

/**
 * @fn bool RadioButtonWidget::isActive()
 * @brief return the status of radio button list, i.e. whether or not it is displayed on the interface
 * @return true is the element is active (displayed), false otherwise (hidden)
*/

/**
 * @fn void RadioButtonWidget::setActive(bool status)
 * @brief define whether the radio button list is displayed, or not, on the interface depending on the status
 * @param status [in] define if the button list should be displayed according to the selected mode
*/

/**
 * @fn QList<QRadioButton *> RadioButtonWidget::getRadioButtonsList()
 * @brief get the radio buttons list created in the constructor
 * @return a QList of QRadioButton
*/

/**
 * @fn int RadioButtonWidget::getCurrentChecked()
 * @brief obtain the identifier of the radio button currently selected among the proposals
 * @return an integer of the index
*/

#endif // RADIOBUTTONWIDGET_H
