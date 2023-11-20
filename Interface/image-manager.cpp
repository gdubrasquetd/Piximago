#include<QWidget>
#include<QString>
#include<QPixmap>
#include<QLabel>
#include<QDir>
#include<QImage>
#include<QDebug>
#include<QCoreApplication>

#include "image-manager.h"

ImageManager::ImageManager(QWidget *parent) : QLabel(parent)
{
    setAlignment(Qt::AlignCenter);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    setFrameShape(QFrame::Box);
    setLineWidth(3);
}

void ImageManager::setImg(QString image_path, QString img_name){

    image = new QPixmap(image_path);

    setPixmap(image->scaled(size(), Qt::KeepAspectRatio));
    this->imgLoaded = true;

    QString currentPath = QCoreApplication::applicationDirPath();
    QDir dir(currentPath);
    dir.mkdir("tmp");
    
    QString copy_file = dir.path() + "/tmp/" + img_name;
    qDebug() << "fichier copié : " << copy_file;
    saveImg(copy_file);

    this->file_path = copy_file;
    this->file_name = img_name;
}

void ImageManager::saveImg(QString file_name){
    QImage img = image->toImage();
    if(!img.save(file_name)){
        qDebug() << "Save error" << file_name;
    }
}

void ImageManager::updateImg(){
    QString currentPath = QCoreApplication::applicationDirPath();
    image->load(currentPath + "/tmp/" + this->file_name);
    setPixmap(image->scaled(size(), Qt::KeepAspectRatio));

}

bool ImageManager::isLoaded(){
    return this->imgLoaded;
}

QString ImageManager::getFilePath(){
    return this->file_path;
}

QString ImageManager::getFileName(){
    return this->file_name;
}

void ImageManager::changePixmap(QPixmap * pixmap){
    this->image = pixmap;
    //this->setPixmap(*image);
    this->setPixmap(image->scaled(size(), Qt::KeepAspectRatio));
    saveImg(this->file_path);
}

QPixmap * ImageManager::getPixmap(){
   return image;
}

void ImageManager::setFilePath(QString file_path){
    this->file_path = file_path;
}

bool ImageManager::isSaved(){
    return this->saved;
}

void ImageManager::setSaved(bool save){
    this->saved = save;
}

void ImageManager::resizeEvent(QResizeEvent *event) {
    QLabel::resizeEvent(event);
    if (imgLoaded) {
        QPixmap *img = new QPixmap(this->file_path);
        setPixmap(img->scaled(size(), Qt::KeepAspectRatio));
    }
}