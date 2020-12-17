#ifndef THE_VIDEOQW_H
#define THE_VIDEOQW_H

#include<QVideoWidget>
#include<QTimer>
#include<QMouseEvent>

class The_videoQw : public QVideoWidget{
    Q_OBJECT

public:
    The_videoQw(QWidget *parent = 0);
    ~The_videoQw(){}

protected:
    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void mouseDoubleClickEvent(QMouseEvent * event);

private:
    QTimer timer;
    bool full;

private slots:
    void change();
};

#endif // THE_VIDEOQW_H
