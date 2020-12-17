//
// Created by twak on 11/11/2019.
//

#include "the_player.h"
#include<iostream>

using namespace std;

// all buttons have been setup, store pointers here
void ThePlayer::setContent(TheButton *b , vector<The_List*> r) {
    lists = r;
    jumpTo(b-> info);
}

// change the image and video for one button every one second
void ThePlayer::shuffle() {
//    TheButtonInfo* i = & infos -> at (rand() % infos->size() );
//        setMedia(*i->url);
//    buttons -> at( updateCount++ % buttons->size() ) -> init( i );
}

void ThePlayer::playStateChanged (QMediaPlayer::State ms) {
    switch (ms) {
        case QMediaPlayer::State::StoppedState:
            play(); // starting playing again...
            break;
    default:
        break;
    }
}

void ThePlayer::jumpTo (TheButtonInfo* button) {
    if(play1->isEnabled()){
        play1->setEnabled(false);
        play1->setVisible(false);
        pause1->setEnabled(true);
        pause1->setVisible(true);
    }
    setMedia( * button -> url);
    play();
}

void ThePlayer::get(){
    stop();
}

void ThePlayer::change1(){
    play1->setEnabled(true);
    play1->setVisible(true);
    pause1->setEnabled(false);
    pause1->setVisible(false);
    this->pause();
}

void ThePlayer::change2(){
    pause1->setEnabled(true);
    pause1->setVisible(true);
    play1->setEnabled(false);
    play1->setVisible(false);
    this->play();
}

void ThePlayer::update_frame(){
    bool hasframe = (this->position()>0);

    if(hasframe){
        if(this->duration()>0){
            frameslider->setMaximum(int(this->duration()));
        }
        else{
            if(this->position()>frameslider->maximum()){
                frameslider->setMaximum(int(this->position()));
            }
        }

        frameslider->setValue(int(this->position()));

        int a = int(position()/1000);
        int b = int(a/60);
        a = a%60;

        QString str = QString::asprintf("%.2d:%.2d",b,a);

        showtime->setText(str);

    }
    else frameslider->setMaximum(0);

    frameslider->setEnabled(hasframe);
}

void ThePlayer::update_postion(){
    qint64 q1 = frameslider->value();
    this->setPosition(q1);
}

void ThePlayer::fast_f(){

    qint64 a = qint64(this->duration()/10);

    if((this->position()+a)>this->duration()){
        this->setPosition(this->duration());
    }else this->setPosition(this->position()+a);

}

void ThePlayer::back_f(){
    qint64 a = qint64(this->duration()/10);

    if((this->position()-a)<=0){
        this->setPosition(0);
    }else this->setPosition(this->position()-a);
}

void ThePlayer::sound_c(){
       sound_qw = new QWidget();
       sound_qw->setMaximumSize(QSize(320,50));
       sound_qw->setMinimumSize(QSize(320,50));

       showsound = new QLabel(QString::asprintf("%d",this->volume()));
       showsound->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

       soundslider = new QSlider(Qt::Horizontal);
       soundslider->setRange(0,100);
       soundslider->setValue(this->volume());

       connect(soundslider,SIGNAL(valueChanged(int)),this,SLOT(set_value_sound()));
       connect(soundslider,SIGNAL(valueChanged(int)),this,SLOT(setVolume(int)));

       sound_layout = new QHBoxLayout();
       sound_qw->raise();
       sound_qw->setWindowFlag(Qt::WindowStaysOnTopHint);
       sound_qw->setLayout(sound_layout);
       sound_layout->addWidget(showsound);
       sound_layout->addWidget(soundslider);

       sound_qw->setAttribute(Qt::WA_DeleteOnClose);
       sound_qw->setWindowTitle("Sound Setting");
       sound_qw->show();

}

void ThePlayer::re_play(){
    this->setPosition(0);
    play();

    if(play1->isEnabled()){
        play1->setEnabled(false);
        play1->setVisible(false);
        pause1->setEnabled(true);
        pause1->setVisible(true);
    }
}

void ThePlayer::set_value_sound(){
    QString q1 = QString::asprintf("%d",soundslider->value());
    showsound->setText(q1);
}

void ThePlayer::connect_clear(){

    bool isSpace;
    bool find;

    int count = 0;

    if(qline->text().toStdString()=="") isSpace = true;
    else isSpace = false;

    for (int i = 0;i < int(lists.size());i++) {
        if(isSpace){
            lists.at(i)->return_label()->setEnabled(true);
            lists.at(i)->return_label()->setVisible(true);
            lists.at(i)->return_TheButton()->setEnabled(true);
            lists.at(i)->return_TheButton()->setVisible(true);
        }else{
            find = lists.at(i)->find_Key(qline->text());
            lists.at(i)->return_label()->setEnabled(find);
            lists.at(i)->return_label()->setVisible(find);
            lists.at(i)->return_TheButton()->setEnabled(find);
            lists.at(i)->return_TheButton()->setVisible(find);
            if(find) count++;
        }
    }

    if(isSpace==false) qs_qw->setGeometry(0,0,225,count*150);
    else qs_qw->setGeometry(0,0,225,lists.size()*150);

    qline->clear();
}
