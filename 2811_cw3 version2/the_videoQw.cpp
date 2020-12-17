#include"the_videoQw.h"
#include<iostream>


using namespace std;

The_videoQw::The_videoQw(QWidget *parent) : QVideoWidget(parent){
    full = true;
}

void The_videoQw::mousePressEvent(QMouseEvent *event){
    if(event->button()==Qt::LeftButton) timer.start(250);
}

void The_videoQw::mouseDoubleClickEvent(QMouseEvent *event){
    if(event->button()==Qt::LeftButton){
    timer.stop();

    if(full){
        full = false;
        setFullScreen(true);}
    else  {
        full = true;
        setFullScreen(false);}
    }
}

void The_videoQw::change(){
    full = false;
    setFullScreen(true);
}
