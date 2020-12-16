//
// Created by twak on 11/11/2019.
//

#include "the_player.h"
#include<iostream>

using namespace std;

// all buttons have been setup, store pointers here
void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i) {
    buttons = b;
    infos = i;
    jumpTo(buttons -> at(0) -> info);
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
            stop(); // starting playing again...
            break;
    default:
        break;
    }
}

void ThePlayer::jumpTo (TheButtonInfo* button) {
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

    if(soundslider->isEnabled()){
        soundslider->setEnabled(false);
        soundslider->setVisible(false);
        showsound->setEnabled(false);
        showsound->setVisible(false);
    }else{
        soundslider->setEnabled(true);
        soundslider->setVisible(true);
        showsound->setEnabled(true);
        showsound->setVisible(true);
        sound_qw->setGeometry(sound->x()+sound->width(),sound->y(),250,50);
    }
}
void ThePlayer::set_value_sound(){
    QString q1 = QString::asprintf("%d",soundslider->value());
    showsound->setText(q1);
}
