/*
 *
 *    ______
 *   /_  __/___  ____ ___  ___  ____
 *    / / / __ \/ __ `__ \/ _ \/ __ \
 *   / / / /_/ / / / / / /  __/ /_/ /
 *  /_/  \____/_/ /_/ /_/\___/\____/
 *              video for sports enthusiasts...
 *
 *  2811 cw3 : twak
 */

#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include "the_player.h"
#include "the_button.h"
#include<QPushButton>
#include<QSize>
#include<QLabel>
#include<QMovie>

using namespace std;

// read in videos and thumbnails to this directory
vector<TheButtonInfo> getInfoIn (string loc) {

    vector<TheButtonInfo> out =  vector<TheButtonInfo>();
    QDir dir(QString::fromStdString(loc) );
    QDirIterator it(dir);

    while (it.hasNext()) { // for all files

        QString f = it.next();
            if (f.contains("."))

#if defined(_WIN32)
            if (f.contains(".wmv"))  { // windows
#else
            if (f.contains(".mp4"))  { // mac/linux
#endif

            QString thumb = f.left( f .length() - 4) +".png";
            if (QFile(thumb).exists()) { // if a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
                    if (!sprite.isNull()) {
                        QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                        QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                        out . push_back(TheButtonInfo( url , ico  ) ); // add to the output list
                    }
                    else
                        qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb << endl;
            }
            else
                qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb << endl;
        }
    }

    return out;
}

vector<QIcon> geticon(string loc){
    vector<QIcon> icon = vector<QIcon>();
    QDir dir(QString::fromStdString(loc) );
    QDirIterator it(dir);

    while (it.hasNext()) {
        QString f = it.next();

        if(f.contains("play")||f.contains("pause")||f.contains("fast")||f.contains("back")||f.contains("sound")) {
            QImageReader *im = new QImageReader(f);
            QImage ima = im->read();
            if(!ima.isNull()){
                QIcon *ic = new QIcon(QPixmap::fromImage(ima));
                icon . push_back(*ic);
            }
        }

    }

    return icon;
}

void play_icon(QPushButton *q1, QPushButton *q2){
    q1->setEnabled(false);
    q1->setVisible(false);
    q2->setEnabled(true);
    q1->setVisible(false);
}

void pause_icon(QPushButton *q1, QPushButton *q2){

}

int main(int argc, char *argv[]) {

    QSize size(50,50);

    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application
    QApplication app(argc, argv);

    // collect all the videos in the folder
    vector<TheButtonInfo> videos;

    if (argc == 2)
        videos = getInfoIn( string(argv[1]) );

    vector<QIcon> icon = geticon(argv[1]);

    if (videos.size() == 0) {

        const int result = QMessageBox::question(
                    NULL,
                    QString("Tomeo"),
                    QString("no videos found! download, unzip, and add command line argument to \"quoted\" file location. Download videos from Tom's OneDrive?"),
                    QMessageBox::Yes |
                    QMessageBox::No );

        switch( result )
        {
        case QMessageBox::Yes:
          QDesktopServices::openUrl(QUrl("https://leeds365-my.sharepoint.com/:u:/g/personal/scstke_leeds_ac_uk/EcGntcL-K3JOiaZF4T_uaA4BHn6USbq2E55kF_BTfdpPag?e=n1qfuN"));
          break;
        default:
            break;
        }
        exit(-1);
    }

    // the widget that will show the video
    QVideoWidget *videoWidget = new QVideoWidget;

    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;

    player->setNotifyInterval(29);
    player->setVideoOutput(videoWidget);

    // a row of buttons
    QWidget *buttonWidget = new QWidget();
    // a list of the buttons
    vector<TheButton*> buttons;
    // the buttons are arranged horizontally
    QHBoxLayout *layout = new QHBoxLayout();
    buttonWidget->setLayout(layout);


    // create the four buttons
    for ( int i = 0; i < 5; i++ ) {
        TheButton *button = new TheButton(buttonWidget);
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo* ))); // when clicked, tell the player to play.
        buttons.push_back(button);
        layout->addWidget(button);
        button->init(&videos.at(i));
    }

    QWidget* frame = new QWidget();
    QHBoxLayout *f_la = new QHBoxLayout();
    f_la->addWidget(player->return_frame());
    f_la->addWidget(player->return_show());
    frame->setLayout(f_la);

    QWidget* control = new QWidget();
    QHBoxLayout *c_la = new QHBoxLayout();
    control->setLayout(c_la);

    player->setIc(buttonWidget);
    player->return_pause()->setIcon(icon.at(2));
    player->return_play()->setIcon(icon.at(3));
    player->return_fast()->setIcon(icon.at(1));
    player->return_back()->setIcon(icon.at(0));
    player->return_sound()->setIcon(icon.at(4));
    player->return_play()->setAutoRaise(true);
    player->return_pause()->setAutoRaise(true);
    player->return_fast()->setAutoRaise(true);
    player->return_back()->setAutoRaise(true);
    player->return_sound()->setAutoRaise(true);
    player->return_play()->setIconSize(size);
    player->return_pause()->setIconSize(size);
    player->return_fast()->setIconSize(size);
    player->return_back()->setIconSize(size);
    player->return_sound()->setIconSize(size);
    c_la->addWidget(player->return_back());
    c_la->addWidget(player->return_pause());
    c_la->addWidget(player->return_play());
    c_la->addWidget(player->return_fast());
    c_la->addWidget(player->return_sound());
    c_la->addWidget(player->return_qw_sound());
    player->return_qw_sound()->resize(250,50);

    // tell the player what buttons and videos are available
    player->setContent(&buttons, & videos);
    player->setVolume(0);

    // create the main window and layout
    QWidget window;
    QVBoxLayout *top = new QVBoxLayout();
    window.setLayout(top);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(800, 680);

    // add the video and the buttons to the top level widget
    top->addWidget(videoWidget);
    top->addWidget(buttonWidget);
    top->addWidget(frame);
    top->addWidget(control);

    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}
