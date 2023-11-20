#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>


class ImageManager : public QLabel {
    Q_OBJECT

public:
    ImageManager(QWidget *parent = nullptr);

public slots:
    void saveImg(QString file_name);
    void setImg(QString file_name, QString img_name);
    void changePixmap(QPixmap * pixmap);
    QPixmap * getPixmap();
    void updateImg();
    bool isLoaded();
    QString getFilePath();
    QString getFileName();
    void setFilePath(QString file_path);
    bool isSaved();
    void setSaved(bool save);
    void resizeEvent(QResizeEvent *event);



private:
    QString file_path = NULL;
    QString file_name;
    bool imgLoaded = false;
    QPixmap * image;
    bool saved = false;


signals:
};

/**
 * @fn void ImageManager::saveImg(QString file_name)
 * @brief convert the result image to QImage to save it in a local folder chosen by the user
 * @param file_name [in] name given by the user to the image at the time of saving
*/

/**
 * @fn void ImageManager::setImg(QString image_path, QString img_name)
 * @brief load an image as a QPixmap and create a copy in a temporary folder
 * @param image_path [in] local path to the original image
 * @param img_name [in] original image name extract from the path
*/

/**
 * @fn void ImageManager::updateImg()
 * @brief reload the image to be resized to the window size 
*/

/**
 * @fn bool ImageManager::isLoaded()
 * @brief indicate whether the image is loaded and displayed on the interface
 * @return true if the image is displayed on the interface, false otherwise
*/

/**
 * @fn QString ImageManager::getFilePath()
 * @brief retrieve the path of the image
 * @return the local path of image as QString
*/

/**
 * @fn QString ImageManager::getFileName()
 * @brief retrieve the name of the image
 * @return the name of image as QString
*/

/**
 * @fn void ImageManager::changePixmap(QPixmap * pixmap)
 * @brief change the image representation on the interface
 * @details call the function 'saveImg' to save modifications in the local 'tmp' folder
 * @param pixmap [in] the new image representation to set
*/

/**
 * @fn QPixmap * ImageManager::getPixmap()
 * @brief retrieve the current image representation 
 * @return a QPixmap of the image representation
*/

/**
 * @fn void ImageManager::setFilePath(QString file_path)
 * @brief set a new filepath for the image 
 * @param file_path [in] the new file path to be set
*/

/**
 * @fn bool ImageManager::isSaved()
 * @brief test if the image is already saved by the user
 * @details this function is used when the user leaves the application : it allows him to save it before leaving
 * @return true if the image is already saved, false otherwise
*/

/**
 * @fn void ImageManager::setSaved(bool save)
 * @brief indicated if the image is saved or not
 * @param save [in] set the variable 'saved' with true or false
*/

/**
 * @fn void ImageManager::resizeEvent(QResizeEvent *event)
 * @brief keep image proportions when the window is resized
 * @details if the image is loaded, the image is reloaded et resized according to the window size
 * @param event [in] event to know when the window is resized
*/


#endif // IMAGEMANAGER_H
