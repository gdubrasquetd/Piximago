#include "viewer-app.h"
#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSplitter>
#include <QFrame>
#include <QWidget>
#include <QPushButton>
#include <QProcess>
#include <QDebug>
#include <QSlider>
#include <QMainWindow>
#include <QComboBox>
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QCoreApplication>


#include "buttons-layout-builder.h"
#include "image-manager.h"
#include "slider-widget.h"



ViewerApp::ViewerApp(QWidget *parent) : QMainWindow(parent)
{
    QWidget *widget = new QWidget();
    widget->resize(1000, 675);
    setCentralWidget(widget);

    QSplitter *splitterGlobal = new QSplitter(Qt::Horizontal);
    QSplitter *splitterRight = new QSplitter(Qt::Vertical);
    QSplitter *splitterLeft = new QSplitter(Qt::Vertical);

    originalImage = new ImageManager();
    resultImage = new ImageManager();

    QPushButton *saveButton = new QPushButton(QString("Confirm changes"));
    saveButton->setFixedHeight(40);

    mode = new QComboBox();
    mode->addItem("Colorimetry");
    mode->addItem("Balance");
    mode->addItem("Convolution");
    mode->addItem("Cross");
    mode->addItem("Band");
    mode->addItem("Stain");
    mode->addItem("Resize");
    mode->setFixedHeight(40);
    
    buttons = new ButtonsLayoutBuilder(this);

    connect(mode, SIGNAL(currentIndexChanged(int)), this, SLOT(updateButtonsLayout(int)));


    foreach(QVariant button, buttons->getButtonsList()) {
        if(button.userType() == qMetaTypeId<SliderWidget *>()){
            SliderWidget *slider = button.value<SliderWidget *>();
            connect(slider->getSlider(), &QSlider::valueChanged, this, &ViewerApp::onSliderValueChanged);
        }else if(button.userType() == qMetaTypeId<RadioButtonWidget *>()){
            RadioButtonWidget *radioGroup = button.value<RadioButtonWidget *>();
            foreach(QRadioButton *radioButton, radioGroup->getRadioButtonsList()){
                connect(radioButton, &QRadioButton::clicked, this, &ViewerApp::onSliderValueChanged);
            }
        }else if(button.userType() == qMetaTypeId<SpinBoxWidget *>()){
            SpinBoxWidget *spin = button.value<SpinBoxWidget *>();
            connect(spin->getSpinBox(), QOverload<int>::of(&QSpinBox::valueChanged), this, &ViewerApp::onSliderValueChanged);
        }
    }

    connect(saveButton, &QPushButton::clicked, this, &ViewerApp::confirmChanges);

    splitterLeft->setFrameShape(QFrame::Box);
    splitterLeft->setFrameShadow(QFrame::Plain);
    splitterLeft->setLineWidth(3);
    splitterLeft->setStyleSheet("background-color: lightGray;");

    splitterRight->addWidget(originalImage);
    splitterRight->addWidget(resultImage);

    splitterLeft->addWidget(mode);
    splitterLeft->addWidget(buttons);
    splitterLeft->addWidget(saveButton);


    splitterGlobal->addWidget(splitterLeft);
    splitterGlobal->addWidget(splitterRight);

    QList<int> ratioGlobal, ratioRight, ratioLeft;
    ratioGlobal << 50 << 300;
    ratioRight << 100 << 100;
    ratioLeft << 1 << 100 << 1;

    splitterGlobal->setSizes(ratioGlobal);
    splitterRight->setSizes(ratioRight);
    splitterLeft->setSizes(ratioLeft);
    splitterGlobal->setMinimumSize(700,450);


    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(splitterGlobal);

    QMenu *menu = menuBar()->addMenu("File");
    select_file_action = menu->addAction("Open");
    select_file_action->setShortcut(QKeySequence::Open);
    connect(select_file_action, SIGNAL(triggered()), this, SLOT(selectImgFile()));

    save_result_action = menu->addAction("Save");
    save_result_action->setShortcut(QKeySequence::Save);
    save_result_action->setEnabled(false);
    connect(save_result_action, SIGNAL(triggered()), this, SLOT(saveResultImg()));

    QMenu *menu_edit = menuBar()->addMenu("Edit");
    undo_action = menu_edit->addAction("Undo");
    undo_action->setShortcut(QKeySequence::Undo);
    undo_action->setEnabled(false);
    connect(undo_action, SIGNAL(triggered()), this, SLOT(undoAction()));

    redo_action = menu_edit->addAction("Redo");
    redo_action->setShortcut(QKeySequence::Redo);
    redo_action->setEnabled(false);
    connect(redo_action, SIGNAL(triggered()), this, SLOT(redoAction()));

    display_mask_action = menu_edit->addAction("Display Mask");
    display_mask_action->setEnabled(false);
    connect(display_mask_action, SIGNAL(triggered()), this, SLOT(displayMask()));

    display_spectrum_action = menu_edit->addAction("Display Spectrum");
    display_spectrum_action->setEnabled(false);
    connect(display_spectrum_action, SIGNAL(triggered()), this, SLOT(displaySpectrum()));

    widget->setLayout(layout);
}


void ViewerApp::selectImgFile(){
    QString file_dir = QCoreApplication::applicationDirPath() + "/../Images";
    QString img_path = QFileDialog::getOpenFileName(this, tr("Select Image"), file_dir, tr("Images (*.jpg *.png *.jpeg)"));
    if(img_path.size() == 0){
        return;
    }

    cleanWorkPlace();

    QString file_name = img_path.split("/").last();
    qDebug() << file_name;

    originalImage->setImg(img_path, file_name);
    resultImage->setImg(img_path, QString("res-") + file_name);

    ImageManager *imgTmp = new ImageManager();
    QString newName = originalImage->getFileName();
    imgTmp->setImg(originalImage->getFilePath(),newName.insert(originalImage->getFileName().indexOf("."), QString::number(currentImageIndex)));
    imagesTmp.append(imgTmp);
    originalImage->changePixmap(resultImage->getPixmap());
    originalImage->updateImg();
    
    int i = 0;
    foreach(QVariant button,  buttons->getButtonsList()){
        if(button.userType() == qMetaTypeId<SpinBoxWidget *>()){
            if(i==0){
                SpinBoxWidget *spin1 = button.value<SpinBoxWidget *>();
                spin1->setDefaultValue(originalImage->getPixmap()->width());
            }
            if(i==1){
                SpinBoxWidget *spin2 = button.value<SpinBoxWidget *>();
                spin2->setDefaultValue(originalImage->getPixmap()->height());
            }
            i++;
        }
    }
}


void ViewerApp::saveResultImg(){
    QString file_dir = QCoreApplication::applicationDirPath() + "/../Images";
    QString file_name = QFileDialog::getSaveFileName(this, tr("Save result image"), file_dir + "/res_img.jpg", tr("Images (*.jpg *.png *.jpeg)"));
    if(file_name.size() == 0){
        return;
    }
    resultImage->saveImg(file_name);
    resultImage->setFilePath(file_name);
    resultImage->setSaved(true);
}

void ViewerApp::undoAction(){
    if(imagesTmp.size() >= 2 && currentImageIndex > 0){
        currentImageIndex --;
        imagesTmp.at(currentImageIndex)->updateImg();
        originalImage->changePixmap(imagesTmp.at(currentImageIndex)->getPixmap());
        resultImage->changePixmap(imagesTmp.at(currentImageIndex)->getPixmap());
        redo_action->setEnabled(true);
    }
    if(currentImageIndex <= 0){
        undo_action->setEnabled(false);
    }
}

void ViewerApp::redoAction(){
    if(imagesTmp.size() >= 2 && currentImageIndex + 1 < imagesTmp.size()){
        currentImageIndex ++;
        imagesTmp.at(currentImageIndex)->updateImg();
        originalImage->changePixmap(imagesTmp.at(currentImageIndex)->getPixmap());
        resultImage->changePixmap(imagesTmp.at(currentImageIndex)->getPixmap());
        undo_action->setEnabled(true);
    }
    if(currentImageIndex + 1 >= imagesTmp.size()){
        redo_action->setEnabled(false);
    }
}



void ViewerApp::updateButtonsLayout(int mode){

    buttons->updateButtonsLayout(mode);

}

void ViewerApp::onSliderValueChanged() {
    QStringList arg_list;
    QString currentPath = QCoreApplication::applicationDirPath();
    if(originalImage->isLoaded() && resultImage->isLoaded()){
        arg_list << currentPath + "/tmp/" + originalImage->getFileName();
        arg_list << currentPath + "/tmp/" + resultImage->getFileName();
    }

    foreach(QVariant button, buttons->getButtonsList()) {
        if(button.userType() == qMetaTypeId<SliderWidget *>()){
            SliderWidget *slider = button.value<SliderWidget *>();
            if(slider->isActive()){
                arg_list << QString::number(slider->getValue());
            }
        }else if(button.userType() == qMetaTypeId<RadioButtonWidget *>()){
            RadioButtonWidget *radioGroup = button.value<RadioButtonWidget *>();
            if(radioGroup->isActive()){
                arg_list << QString::number(radioGroup->getCurrentChecked());
            }
        }else{
            SpinBoxWidget *spin = button.value<SpinBoxWidget *>();
            if(spin->isActive()){
                arg_list << QString::number(spin->getValue());
            }
        }
    }

    executeCommand(arg_list);

    if(resultImage->isLoaded()){
        resultImage->updateImg();
    }
    foreach(ImageManager *spectrum, spectrumTmp){
        spectrum->updateImg();
    }
    foreach(ImageManager *mask, maskTmp){
        mask->updateImg();
    }
}

void ViewerApp::executeCommand(QStringList args) {
    QProcess process;
    QString currentPath = QCoreApplication::applicationDirPath();
    process.setWorkingDirectory(currentPath);
    if (mode->currentIndex() == 0) {
        qDebug() << "./colors" << args;
        process.start("./colors", args);
        display_spectrum_action->setEnabled(false);
        display_mask_action->setEnabled(false);
    } else if (mode->currentIndex() == 1) {
        qDebug() << "./balance" << args;
        process.start("./balance", args);
        display_spectrum_action->setEnabled(false);
        display_mask_action->setEnabled(false);
    } else if (mode->currentIndex() == 2) {
        qDebug() << "./convolution" << args;
        process.start("./convolution", args);
        display_spectrum_action->setEnabled(false);
        display_mask_action->setEnabled(false);
    } else if (mode->currentIndex() == 3) {
        qDebug() << "./cross" << args;
        process.start("./cross", args);
        display_spectrum_action->setEnabled(true);
        display_mask_action->setEnabled(true);
    } else if (mode->currentIndex() == 4) {
        qDebug() << "./band" << args;
        process.start("./band", args);
        display_spectrum_action->setEnabled(true);
        display_mask_action->setEnabled(true);
    } else if (mode->currentIndex() == 5) {
        qDebug() << "./stain" << args;
        process.start("./stain", args);
        display_spectrum_action->setEnabled(true);
        display_mask_action->setEnabled(true);
    } else if (mode->currentIndex() == 6) {
        qDebug() << "./resize" << args;
        process.start("./resize", args);
        display_spectrum_action->setEnabled(false);
        display_mask_action->setEnabled(false);
    } else {
        qDebug() << "Error command on mode : " << mode->currentIndex() << args;
    }

    process.waitForFinished();
    QByteArray output = process.readAllStandardOutput();
    QByteArray error = process.readAllStandardError();
}

void ViewerApp::confirmChanges() {
    originalImage->changePixmap(resultImage->getPixmap());
    originalImage->updateImg();
    for(int i = imagesTmp.size() - 1; i > currentImageIndex; i--){
        imagesTmp.removeAt(i);
    }    
    ImageManager *imgTmp = new ImageManager();
    currentImageIndex ++;
    QString newName = originalImage->getFileName();
    imgTmp->setImg(resultImage->getFilePath(),newName.insert(originalImage->getFileName().indexOf("."), QString::number(currentImageIndex)));
    imagesTmp.append(imgTmp);

    save_result_action->setEnabled(true);
    undo_action->setEnabled(true);
}

void ViewerApp::closeEvent(QCloseEvent *event) {
    if(!resultImage->isSaved() && resultImage->isLoaded() ){
        QMessageBox *msgBox = new QMessageBox(this);
        msgBox->setText("Are you sure you want to quit the application without saving changes ?");

        QPushButton *saveButton = msgBox->addButton("Save", QMessageBox::ApplyRole);
        QPushButton *quitButton = msgBox->addButton("Quit without saving", QMessageBox::NoRole);
        QPushButton *cancelButton = msgBox->addButton("Cancel", QMessageBox::RejectRole);

        saveButton->setIcon(QIcon::fromTheme("document-save"));
        quitButton->setIcon(QIcon::fromTheme("window-close"));
        cancelButton->setIcon(QIcon::fromTheme("edit-undo"));

        msgBox->exec();
        if (msgBox->clickedButton() == saveButton) {
            event->ignore();
            saveResultImg();
        } else if (msgBox->clickedButton() == cancelButton) {
            event->ignore(); 
        } else if (msgBox->clickedButton() == quitButton) {
            event->accept();
        cleanWorkPlace();
        }
    }else{
        if(resultImage->isLoaded() ){
            resultImage->saveImg(resultImage->getFilePath());
        }

        QMainWindow::closeEvent(event);
        cleanWorkPlace();
    }

}

void ViewerApp::cleanWorkPlace(){
    // Clear files list
    imagesTmp.clear();
    currentImageIndex = 0;

    // Remove files in tmp
    QString currentPath = QCoreApplication::applicationDirPath();
    QDir tmpDir(currentPath + "/tmp/");
    QFileInfoList tmpDirFiles = tmpDir.entryInfoList(QDir::Files);
    for(auto file : tmpDirFiles){
        QFile::remove(file.absoluteFilePath());
    }

    tmpDir.setPath(currentPath + "/tmp/mask/");
    tmpDirFiles.clear();
    tmpDirFiles = tmpDir.entryInfoList(QDir::Files);
    for(auto file : tmpDirFiles){
        QFile::remove(file.absoluteFilePath());
    }

    tmpDir.setPath(currentPath + "/tmp/spectrum/");
    tmpDirFiles.clear();
    tmpDirFiles = tmpDir.entryInfoList(QDir::Files);
    for(auto file : tmpDirFiles){
        QFile::remove(file.absoluteFilePath());
    }

    mode->setCurrentIndex(0);

    save_result_action->setEnabled(false);
    undo_action->setEnabled(false);
    redo_action->setEnabled(false);
}

void ViewerApp::displayMask(){
    QWidget *dislayWindow = new QWidget();
    dislayWindow->resize(750, 250);
    dislayWindow->setWindowTitle("Mask");
    QHBoxLayout* layout = new QHBoxLayout();
    QDir maskDir(QCoreApplication::applicationDirPath() + "/tmp/mask/");
    QFileInfoList maskDirFiles = maskDir.entryInfoList(QDir::Files);
    for(auto file : maskDirFiles){
        QVBoxLayout* vlayout = new QVBoxLayout();
        QLabel *name = new QLabel(file.fileName() + " :");
        name->setStyleSheet("font-size: 16px;");
        name->setMaximumHeight(18);
        ImageManager *img = new ImageManager();
        img->setImg(file.absoluteFilePath(), "mask/" + file.fileName());
        maskTmp.append(img);
        vlayout->setMargin(0);
        vlayout->setSpacing(2);
        vlayout->addWidget(name, 0, Qt::AlignHCenter);
        vlayout->addWidget(img);
        layout->addItem(vlayout);
    }
    dislayWindow->setLayout(layout);
    dislayWindow->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    dislayWindow->show();
}

void ViewerApp::displaySpectrum(){
    QWidget *dislayWindow = new QWidget();
    dislayWindow->resize(750, 250);
    dislayWindow->setWindowTitle("Spectrum");
    QHBoxLayout* layout = new QHBoxLayout();
    QDir maskDir(QCoreApplication::applicationDirPath() + "/tmp/spectrum/");
    QFileInfoList maskDirFiles = maskDir.entryInfoList(QDir::Files);
    for(auto file : maskDirFiles){
        QVBoxLayout* vlayout = new QVBoxLayout();
        QLabel *name = new QLabel(file.fileName() + " :");
        name->setStyleSheet("font-size: 16px;");
        name->setMaximumHeight(18);
        ImageManager *img = new ImageManager();
        img->setImg(file.absoluteFilePath(), "spectrum/" + file.fileName());
        spectrumTmp.append(img);
        vlayout->setMargin(0);
        vlayout->setSpacing(2);
        vlayout->addWidget(name, 0, Qt::AlignHCenter);
        vlayout->addWidget(img);
        layout->addItem(vlayout);
    }
    dislayWindow->setLayout(layout);
    dislayWindow->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    dislayWindow->show();
}
