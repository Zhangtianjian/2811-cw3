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
#include<QScrollArea>
#include<QMovie>
#include<QGridLayout>
#include<QScrollBar>
#include<QLineEdit>
#include<QPalette>
#include "the_videoQw.h"
#include "the_list.h"

using namespace std;

// read in videos and thumbnails to this directory
vector<TheButtonInfo> getInfoIn (string loc , vector<The_List*> &lists) {

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

                        TheButton *butt = new TheButton(nullptr);
                        butt->init(new TheButtonInfo(url,ico));

                        The_List *li = new The_List(f,butt);
                        lists.push_back(li);
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
        if(f.contains("z_zzzde")||f.contains("play")||f.contains("pause")||f.contains("fast")||f.contains("back")||f.contains("sound")||f.contains("z_replay")||f.contains("z_zquan")||f.contains("z_zzopen")) {
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

int main(int argc, char *argv[]) {

    QSize size(50,50);

    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application
    QApplication app(argc, argv);

    // collect all the videos in the folder
    vector<TheButtonInfo> videos;
    vector<The_List*> lists = vector<The_List*>();

    if (argc == 2)
        videos = getInfoIn( string(argv[1]),lists);
    vector<QIcon> icon = geticon("../2811_cw3/icons");

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
    The_videoQw *videoWidget = new The_videoQw;
    videoWidget->setBrightness(100);

    videoWidget->setAutoFillBackground(true);
    QPalette qplte;
    qplte.setColor(QPalette::Window, QColor(0,0,1));
    videoWidget->setPalette(qplte);
    videoWidget->setMinimumSize(1,500);

    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;
    player->setVideoOutput(videoWidget);

    // a row of buttons
    QWidget *buttonWidget = new QWidget();
    // a list of the buttons
    vector<TheButton*> buttons;
    // the buttons are arranged horizontally
    QHBoxLayout *layout = new QHBoxLayout();
    buttonWidget->setLayout(layout);

    QScrollArea *qs = new QScrollArea();

    qs->setStyleSheet("border: 0px solid;border-right-width: 1px;border-right-color: #dcdbdc;background-color: #f5f5f7;");

    qs->setMinimumSize(250,1);
    qs->setMaximumSize(250,10000);
    qs->setFrameShape(QFrame::NoFrame);

    QWidget *button_qw = new QWidget();

    qs->setWidget(button_qw);

    QVBoxLayout *button_qvbox = new QVBoxLayout();
    button_qw->setLayout(button_qvbox);
    button_qw->setGeometry(0,0,225,lists.size()*175);

    player->set_qs_sc(button_qw);

    for(int i=0;i<int(lists.size());i++){
        lists.at(i)->return_TheButton()->setParent(buttonWidget);
        lists.at(i)->return_TheButton()->connect(lists.at(i)->return_TheButton(), SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo* )));
        button_qvbox->addWidget(lists.at(i)->return_TheButton());
        button_qvbox->addWidget(lists.at(i)->return_label());
    }

    player->set_video_show(button_qvbox);

    QWidget* frame = new QWidget();
    QHBoxLayout *f_la = new QHBoxLayout();
    f_la->addWidget(player->return_current_time());
    f_la->addWidget(player->return_frame());
    f_la->addWidget(player->return_show());
    frame->setLayout(f_la);

    QWidget* control = new QWidget();
    QGridLayout *c_la = new QGridLayout();
    control->setLayout(c_la);

    QToolButton *q6 = new QToolButton();
    q6->setIcon(icon.at(6));
    q6->setAutoRaise(true);
    q6->setIconSize(size);
    q6->setToolTip("Full Screan");

    q6->connect(q6,SIGNAL(released()),videoWidget,SLOT(change()));


    QToolButton *q7 = new QToolButton();
    q7->setIcon(icon.at(7));
    q7->setAutoRaise(true);
    q7->setIconSize(size);
    q7->setToolTip("Open File");

    player->set_Open_file(q7);
    player->set_de_icon(&icon.at(8));

    player->setIc(buttonWidget);
    player->return_pause()->setIcon(icon.at(2));
    player->return_play()->setIcon(icon.at(3));
    player->return_fast()->setIcon(icon.at(1));
    player->return_back()->setIcon(icon.at(0));
    player->return_sound()->setIcon(icon.at(4));
    player->return_replay()->setIcon(icon.at(5));
    player->return_play()->setAutoRaise(true);
    player->return_pause()->setAutoRaise(true);
    player->return_fast()->setAutoRaise(true);
    player->return_back()->setAutoRaise(true);
    player->return_sound()->setAutoRaise(true);
    player->return_replay()->setAutoRaise(true);
    player->return_play()->setIconSize(size);
    player->return_pause()->setIconSize(size);
    player->return_fast()->setIconSize(size);
    player->return_back()->setIconSize(size);
    player->return_sound()->setIconSize(size);
    player->return_replay()->setIconSize(size);
    c_la->addWidget(player->return_replay(),0,1);
    c_la->addWidget(player->return_back(),0,2);
    c_la->addWidget(player->return_pause(),0,3);
    c_la->addWidget(player->return_play(),0,3);
    c_la->addWidget(player->return_fast(),0,4);
    c_la->addWidget(player->return_sound(),0,5);
    c_la->addWidget(q6,0,6);
    c_la->addWidget(q7,0,0);

    // tell the player what buttons and videos are available
    player->setContent(lists.at(1)->return_TheButton(),lists);

    // create the main window and layout
    QWidget window;

    QWidget *top_wow = new QWidget();
    QGridLayout *top = new QGridLayout();
    top_wow->setLayout(top);

    QGridLayout *hx_wind = new QGridLayout();

    QWidget *NEW_qw = new QWidget();
    QVBoxLayout *wula = new QVBoxLayout();
    NEW_qw->setLayout(wula);

    QToolButton *cla_butt = new QToolButton();
    cla_butt->setText("Class");

    cla_butt->connect(cla_butt,SIGNAL(released()),player,SLOT(do_class()));

    wula->addWidget(cla_butt);
    wula->addWidget(player->return_qline());
    wula->addWidget(qs);
    NEW_qw->setMaximumSize(275,10000);

    hx_wind->addWidget(NEW_qw,0,1,1,1);
    hx_wind->addWidget(top_wow,0,2,2,1);

    window.setLayout(hx_wind);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(800, 680);
    window.resize(1200,680);

    // add the video and the buttons to the top level widget
    top->addWidget(videoWidget,1,0,1,1);
    //top->addWidget(buttonWidget);
    top->addWidget(frame,3,0,1,1);
    top->addWidget(control,4,0,1,1);
    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}
