#ifndef PLAY_PASUE_H
#define PLAY_PASUE_H

#include<QPushButton>
#include<QtWidgets>
#include<QIcon>
#include<QSize>
#include"the_player.h"

class Play_pasue{
private:
    QPushButton *play;
    QPushButton *pause;
    ThePlayer *player;
public:
    Play_pasue(){}
    void setPlay(QWidget *pl){
        play = new QPushButton(pl);
    }

    void setPause(QWidget *pl){
        pause = new QPushButton(pl);
        pause->setEnabled(false);
        pause->setVisible(false);
        pause->connect(pause,SIGNAL(clicked()),this,SLOT(change1(pause,play,player)));
    }

    void setPlayer(ThePlayer *player){
        this->player=player;
    }

    void setplayicon(QIcon *ic){
        play->setIcon(*ic);
        play->setIconSize(QSize(50,50));
    }

    void setpauseicon(QIcon *ic){
        pause->setIcon(*ic);
        pause->setIconSize(QSize(50,50));
    }

    QPushButton* return_play(){
        return play;
    }

    QPushButton* return_pause(){
        return pause;
    }

private slots:
    void change1(QPushButton *q1,QPushButton *q2,ThePlayer *pl){
        q1->setEnabled(true);
        q1->setVisible(true);
        q2->setEnabled(false);
        q2->setVisible(false);
        pl->pause();
    }

    void change2(QPushButton *q1,QPushButton *q2,ThePlayer *pl){
        q1->setEnabled(true);
        q1->setVisible(true);
        q2->setEnabled(false);
        q2->setVisible(false);
        pl->play();
    }
};

#endif // PLAY_PASUE_H
