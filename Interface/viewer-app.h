#ifndef VIEWER_APP_H
#define VIEWER_APP_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QSplitter>
#include <QMainWindow>
#include <QList>
#include <QComboBox>


#include "buttons-layout-builder.h"
#include "image-manager.h"

class ViewerApp : public QMainWindow{
    Q_OBJECT

public:
    ViewerApp(QWidget *parent = 0);
    

public slots:
    void selectImgFile();
    void updateButtonsLayout(int mode);
    void saveResultImg();
    void undoAction();
    void redoAction();
    void onSliderValueChanged();
    void executeCommand(QStringList args);
    void confirmChanges();
    void closeEvent(QCloseEvent *event);
    void cleanWorkPlace();
    void displayMask();
    void displaySpectrum();

private:
    ButtonsLayoutBuilder *buttons;
    ImageManager *originalImage;
    ImageManager *resultImage;
    QList<ImageManager*> imagesTmp;
    QList<ImageManager*> spectrumTmp;
    QList<ImageManager*> maskTmp;
    QComboBox *mode;
    int currentImageIndex = 0;

    QAction *select_file_action;
    QAction *save_result_action;
    QAction *undo_action;
    QAction *redo_action;
    QAction *display_mask_action;
    QAction *display_spectrum_action;

};

/**
 * @fn void ViewerApp::selectImgFile()
 * @brief Display the image chosen by the user, from a dialog box, twice on the interface : first is the current image, while the second is the image resulting from processing
 * @details Create a local copy of this image to save a record of changes and filed an ImageManager QList 'imgTmp' containing the different generated images to undo or redo a step
*/

/**
 * @fn void ViewerApp::updateButtonsLayout(int mode)
 * @brief Call updateButtonsLayout in the class ButtonsLayoutBuilder to manage the display of buttons according to the selected mode
 * @param mode [in] indicate the mode selected by the user
*/

/**
 * @fn void ViewerApp::saveResultImg()
 * @brief save the final resulting image with a name chosen by the user from a dialog box
*/

/**
 * @fn void ViewerApp::undoAction()
 * @brief Enable to return and display the previously generated image : future processing will be based on this image
*/

/**
 * @fn void ViewerApp::redoAction()
 * @brief Enable to recover and display an image that had previously cancelled : future processing will be based on this image
*/

/**
 * @fn void ViewerApp::onSliderValueChanged()
 * @brief This function is called when one of the interface elements is modified : slider, spin box or radio button.
 * The resulting image, and eventually corresponding spectrums or masks, are modified in real time
 * @details Retrieve the values indicated by the elements to set the command to be executed and update result image, spectrums and masks
*/

/**
 * @fn void ViewerApp::executeCommand(QStringList args) {
 * @brief Execute the appropriate command depending on the current mode and previously retrieved values to generate a result image
 * @param args [in] gather values retrieved from interface elements
*/

/**
 * @fn void ViewerApp::confirmChanges()
 * @brief Apply the result image representation to the original image and create a local copy
 * @details add image to the QList<ImageManager *> 'imgTmp' to save progress steps
 * 'save' and 'undo' actions are now enabled
*/

/**
 * @fn void ViewerApp::closeEvent(QCloseEvent *event)
 * @brief Display a message with several options before exiting the application : save, quit, cancel
 * @details save : save the result image before leaving the application.
 * cancel : cancel leaving the application
 * quit : exit the application without saving the result image
 * @param event [in] event to signal when the user wants to leave the application
*/

/**
 * @fn void ViewerApp::cleanWorkPlace()
 * @brief reset all application elements
 * @details  
 * - disabled 'save', 'undo' and 'redo' actions 
 * - deletes locally saved image copies, spectrums and masks, also clear the corresponding lists
 * - rest the mode to 0
*/

/**
 * @fn void ViewerApp::displayMask()
 * @brief Create a new window to display the masks produced from the Fourier transform
 * @details action only availablefor modes 3, 4 and 5
*/

/**
 * @fn void ViewerApp::displaySpectrum()
 * @brief Create a new window to display the spectrums produced from the Fourier transform
 * @details action only availablefor modes 3, 4 and 5
*/

#endif // VIEWER_APP_H