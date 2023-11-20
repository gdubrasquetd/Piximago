#include <QApplication>
#include <QWidget>
#include <QDebug>
#include <QSplashScreen>
#include <QTimer>
#include <QThread>
#include <QMovie>
#include <QDesktopWidget>
#include <QScreen>

#include "viewer-app.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QLabel label;
    QMovie *movie = new QMovie(QCoreApplication::applicationDirPath() + "/../Images/PiximagoAnimation.gif");
    label.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    label.setAttribute(Qt::WA_TranslucentBackground);
    label.setMovie(movie);

    label.show();
    movie->setSpeed(150);
    movie->start();
    label.setFixedSize(movie->frameRect().width(), movie->frameRect().height());

    int timer = 0;
    for (int i = 0; i < movie->frameCount(); ++i) {
        timer += movie->nextFrameDelay();
    }


    ViewerApp viewerApp;
    viewerApp.setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "/../Images/Piximago.png"));
    viewerApp.setWindowTitle("Piximago");
    QTimer::singleShot(timer, &viewerApp, SLOT(show()));
    QTimer::singleShot(timer, &label, &QLabel::close);
    //viewerApp.show();

    int screenWidth = QGuiApplication::primaryScreen()->geometry().width();
    int screenHeight = QGuiApplication::primaryScreen()->geometry().height();

    label.move((screenWidth - label.width()) / 2, (screenHeight - label.height()) / 2);
    viewerApp.move((screenWidth - viewerApp.width()) / 2, (screenHeight - viewerApp.height()) / 2);

    return app.exec();
}
