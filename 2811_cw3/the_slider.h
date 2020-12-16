#ifndef THE_SLIDER_H
#define THE_SLIDER_H

#include<QSlider>
#include<QMouseEvent>

class The_slider : public QSlider{
Q_OBJECT

public:
    The_slider(QWidget *parent = 0);
    The_slider(Qt::Orientation orientation, QWidget * parent = 0);
    ~The_slider(){}

protected:
        virtual void mousePressEvent ( QMouseEvent * event ) ;
};

#endif // THE_SLIDER_H
