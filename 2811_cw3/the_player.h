//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_PLAYER_H
#define CW2_THE_PLAYER_H


#include <QApplication>
#include <QMediaPlayer>
#include "the_button.h"
#include "the_slider.h"
#include <vector>
#include <QTimer>
#include <QPushButton>
#include <QToolButton>
#include<QSize>
#include<QLabel>
#include<QSlider>
#include<QHBoxLayout>

using namespace std;

class ThePlayer : public QMediaPlayer {

Q_OBJECT

private:
    vector<TheButtonInfo>* infos;
    vector<TheButton*>* buttons;
    QTimer* mTimer;
    QToolButton *play1;
    QToolButton *pause1;
    QToolButton *fast;
    QToolButton *back;
    QToolButton *sound;
    int sound_big;
    QLabel *showtime;
    QLabel *showsound;
    The_slider *frameslider;
    QWidget *sound_qw;
    QHBoxLayout *sound_layout;
    The_slider *soundslider;
    long updateCount = 0;

public:
    ThePlayer() : QMediaPlayer(NULL) {
        setVolume(0); // be slightly less annoying
        connect (this, SIGNAL (stateChanged(QMediaPlayer::State)), this, SLOT (playStateChanged(QMediaPlayer::State)) );

        sound_big = 0;

        showtime = new QLabel("00:00");
        showtime->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

        showsound = new QLabel(QString::asprintf("%d",sound_big));
        showsound->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

        frameslider = new The_slider(Qt::Horizontal);
        frameslider->setTickInterval(1);
        frameslider->setSingleStep(1);


        soundslider = new The_slider(Qt::Horizontal);
        soundslider->setRange(1,100);

        connect(this,SIGNAL(positionChanged(qint64)),this,SLOT(update_frame()));
        connect(frameslider,SIGNAL(sliderMoved(int)),this,SLOT(update_postion()));
        connect(soundslider,SIGNAL(valueChanged(int)),this,SLOT(set_value_sound()));

        connect(soundslider,SIGNAL(valueChanged(int)),this,SLOT(setVolume(int)));

        mTimer = new QTimer(NULL);
        mTimer->setInterval(1000); // 1000ms is one second between ...
        mTimer->start();
        connect( mTimer, SIGNAL (timeout()), SLOT ( shuffle() ) ); // ...running shuffle method

        showsound->setEnabled(false);
        showsound->setVisible(false);
        soundslider->setEnabled(false);
        soundslider->setVisible(false);

        sound_qw = new QWidget();
        sound_layout = new QHBoxLayout();
        sound_qw->raise();
        sound_qw->setWindowFlag(Qt::WindowStaysOnTopHint);
        sound_qw->setLayout(sound_layout);
        sound_layout->addWidget(showsound);
        sound_layout->addWidget(soundslider);
    }

    // all buttons have been setup, store pointers here
    void setContent(vector<TheButton*>* b, vector<TheButtonInfo>* i);

    void setIc(QWidget *pl){
        play1 = new QToolButton(pl);
        pause1 = new QToolButton(pl);
        fast = new QToolButton(pl);
        back = new QToolButton(pl);
        sound = new QToolButton(pl);

        play1->setToolTip("play");
        pause1->setToolTip("pause");
        fast->setToolTip("Forward 10%");
        back->setToolTip("back %10");
        sound->setToolTip("set sound");

        play1->setVisible(false);
        play1->setEnabled(false);

        play1->connect(play1,SIGNAL(released()),this,SLOT(change2()));
        pause1->connect(pause1,SIGNAL(released()),this,SLOT(change1()));
        fast->connect(fast,SIGNAL(released()),this,SLOT(fast_f()));
        back->connect(back,SIGNAL(released()),this,SLOT(back_f()));
        sound->connect(sound,SIGNAL(released()),this,SLOT(sound_c()));
    }

    QToolButton* return_play(){
        return play1;
    }

    QToolButton* return_fast(){
        return fast;
    }

    QToolButton* return_back(){
        return back;
    }

    QToolButton* return_pause(){
        return pause1;
    }


    QToolButton* return_sound(){
        return sound;
    }

    QSlider* return_frame(){
        return frameslider;
    }

    QSlider* return_soundslider(){
        return soundslider;
    }

    QLabel* return_show(){
        return showtime;
    }

    QLabel* return_show_sound(){
        return showsound;
    }

    QWidget * return_qw_sound(){
        return sound_qw;
    }

private slots:

    // change the image and video for one button every one second
    void shuffle();

    void update_frame();

    void update_postion();

    void sound_c();

    void set_value_sound();

    void playStateChanged (QMediaPlayer::State ms);

public slots:

    // start playing this ButtonInfo
    void jumpTo (TheButtonInfo* button);
    void change1();
    void change2();
    void fast_f();
    void back_f();
    void get();
};

#endif //CW2_THE_PLAYER_H
