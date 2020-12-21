#include "the_slider.h"
#include<iostream>

using namespace std;

The_slider::The_slider(QWidget *parent) : QSlider(parent){}
The_slider::The_slider(Qt::Orientation orientation, QWidget *parent):QSlider(orientation,parent){}

void The_slider::mousePressEvent ( QMouseEvent * event ){
    if(event->button()==Qt::LeftButton && isEnabled()){
        int dr = maximum()-minimum();

        int po = minimum() + dr * ((double)event->x() / width());

        if(po!=sliderPosition()){
            setValue(po);
            sliderMoved(po);
        }
    }
}
