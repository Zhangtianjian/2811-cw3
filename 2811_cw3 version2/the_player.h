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
#include<QLineEdit>
#include"the_videoQw.h"
#include "the_list.h"

using namespace std;

class ThePlayer : public QMediaPlayer {

Q_OBJECT

private:
    vector<TheButtonInfo>* infos;
    vector<TheButton*>* buttons;
    vector<The_List*> lists;

    QTimer* mTimer;
    QToolButton *play1;
    QToolButton *pause1;
    QToolButton *fast;
    QToolButton *back;
    QToolButton *sound;
    QToolButton *replay;
    bool sound_big;
    QLabel *showtime;
    QLabel *showsound;
    QLabel *current_time;
    The_slider *frameslider;
    QWidget *sound_qw;
    QWidget *hello;
    QHBoxLayout *sound_layout;
    QSlider *soundslider;
    QWidget *qs_qw;
    QLineEdit *qline;
    The_videoQw *the_video;
    long updateCount = 0;

public:
    ThePlayer() : QMediaPlayer(NULL) {
        setVolume(0); // be slightly less annoying
        connect (this, SIGNAL (stateChanged(QMediaPlayer::State)), this, SLOT (playStateChanged(QMediaPlayer::State)) );

        setNotifyInterval(29);

        setVolume(29);

        qline = new QLineEdit();
        qline->setPlaceholderText("Input The Key Words");
        qline->setAlignment(Qt::AlignLeft);
        qline->connect(qline,SIGNAL(returnPressed()),this,SLOT(connect_clear()));

        the_video = new The_videoQw;

        sound_big = false;

        showtime = new QLabel("00:00");
        showtime->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
        current_time = new QLabel("Current Time:");
        current_time->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

        frameslider = new The_slider(Qt::Horizontal);
        frameslider->setTickInterval(1);
        frameslider->setSingleStep(1);

        connect(this,SIGNAL(positionChanged(qint64)),this,SLOT(update_frame()));
        connect(frameslider,SIGNAL(sliderMoved(int)),this,SLOT(update_postion()));


        mTimer = new QTimer(NULL);
        mTimer->setInterval(1000); // 1000ms is one second between ...
        mTimer->start();
        connect( mTimer, SIGNAL (timeout()), SLOT ( shuffle() ) ); // ...running shuffle method
    }

    // all buttons have been setup, store pointers here
    //void setContent(vector<TheButton*>* b, vector<TheButtonInfo>* i);
    void setContent(TheButton *b , vector<The_List*> r);

    void setIc(QWidget *pl){
        play1 = new QToolButton(pl);
        pause1 = new QToolButton(pl);
        fast = new QToolButton(pl);
        back = new QToolButton(pl);
        sound = new QToolButton(pl);
        replay = new QToolButton(pl);

        play1->setToolTip("Play");
        pause1->setToolTip("Pause");
        fast->setToolTip("Forward 10%");
        back->setToolTip("Back %10");
        sound->setToolTip("Set Sound");
        replay->setToolTip("Replay");

        play1->setVisible(false);
        play1->setEnabled(false);

        play1->connect(play1,SIGNAL(released()),this,SLOT(change2()));
        pause1->connect(pause1,SIGNAL(released()),this,SLOT(change1()));
        fast->connect(fast,SIGNAL(released()),this,SLOT(fast_f()));
        back->connect(back,SIGNAL(released()),this,SLOT(back_f()));
        sound->connect(sound,SIGNAL(released()),this,SLOT(sound_c()));
        replay->connect(replay,SIGNAL(released()),this,SLOT(re_play()));
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

    QToolButton* return_replay(){
        return replay;
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

    QLabel* return_current_time(){
        return current_time;
    }

    QWidget * return_qw_sound(){
        return sound_qw;
    }

    QLineEdit * return_qline(){
        return qline;
    }

    void set_qs_sc(QWidget *qw){
        qs_qw = qw;
    }

    The_videoQw* return_the_video(){
        return the_video;
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
    void re_play();
    void get();
    void connect_clear();
};

#endif //CW2_THE_PLAYER_H
