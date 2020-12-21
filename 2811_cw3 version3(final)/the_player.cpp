//
// Created by twak on 11/11/2019.
//

#include "the_player.h"
#include<iostream>

using namespace std;

// all buttons have been setup, store pointers here
void ThePlayer::setContent(TheButton *b , vector<The_List*> r) {
    lists = r;
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
            setPosition(1);
            play();// starting playing again...

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

    if(sound_big){
        sound_big = false;
        sound_qw->close();

        sound_qw = nullptr;
        showsound = nullptr;
        soundslider = nullptr;
        sound_layout = nullptr;

    }else{
        if(sound_qw!=nullptr) {
            cout << "hello" << endl;
            return;}
        else{
            sound_qw = new QWidget();
            sound_qw->setMaximumSize(QSize(320,50));
            sound_qw->setMinimumSize(QSize(320,50));
            sound_qw->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowTitleHint);

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

            sound_big = true;
        }
    }

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

    vector<The_List*> * lili;

    if(qline->text().toStdString()==""){
        lili = &lists;
        isSpace = true;
    }
    else if(qline->text().toStdString()=="1"){
        lili = &outhome;
        isSpace = true;
    }
    else if(qline->text().toStdString()=="2"){
        lili = &buzhidao;
        isSpace = true;
    }
    else if(qline->text().toStdString()=="3"){
        lili = &anthoner;
        isSpace = true;
    }
    else{
        lili = &lists;
         isSpace = false;
    }

    for(auto ve:lists){
        ve->return_label()->setEnabled(false);
        ve->return_label()->setVisible(false);
        ve->return_TheButton()->setEnabled(false);
        ve->return_TheButton()->setVisible(false);
    }

    for (int i = 0;i < int(lili->size());i++) {
        if(isSpace){
            lili->at(i)->return_label()->setEnabled(true);
            lili->at(i)->return_label()->setVisible(true);
            lili->at(i)->return_TheButton()->setEnabled(true);
            lili->at(i)->return_TheButton()->setVisible(true);
        }else{
            find = lists.at(i)->find_Key(qline->text());
            lili->at(i)->return_label()->setEnabled(find);
            lili->at(i)->return_label()->setVisible(find);
            lili->at(i)->return_TheButton()->setEnabled(find);
            lili->at(i)->return_TheButton()->setVisible(find);
            if(find) count++;
        }
    }

    if(isSpace==false) qs_qw->setGeometry(0,0,225,count*175);
    else qs_qw->setGeometry(0,0,225,lili->size()*175);

    qline->clear();
}

void ThePlayer::openF(){
    QFileDialog *qfd = new QFileDialog();

    qfd->setWindowTitle("Open Files");

    qfd->setDirectory(".");

    qfd->setNameFilter("(*.wmv)");

    qfd->setFileMode(QFileDialog::ExistingFiles);

    qfd->setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if(qfd->exec())
    {
       fileNames = qfd->selectedFiles();
    }
    for(auto tmp:fileNames){
        QUrl* url = new QUrl(QUrl::fromLocalFile( tmp ));
        QIcon *ico;
        QString thumb = tmp.left( tmp .length() - 4) +".png";
        if(QFile(thumb).exists()){
            QImageReader *imageReader = new QImageReader(thumb);
            QImage sprite = imageReader->read();
            if(!sprite.isNull()){
                ico = new QIcon(QPixmap::fromImage(sprite));
            }else{
                ico = de_icon;
            }
        }else{
            ico = de_icon;
        }

        TheButton *butt = new TheButton(nullptr);
        butt->setIconSize(QSize(200,110));
        butt->init(new TheButtonInfo(url,ico));

        The_List *li = new The_List(tmp,butt);
        lists.push_back(li);

        li->return_TheButton()->connect(li->return_TheButton(),SIGNAL(jumpTo(TheButtonInfo*)),this,SLOT(jumpTo(TheButtonInfo*)));

        video_show->addWidget(li->return_TheButton());
        video_show->addWidget(li->return_label());
    }

    connect_clear();
}

void ThePlayer::do_class(){
    do_cla = new QWidget();
    do_cla->setWindowTitle("Select Class");

    QHBoxLayout *qv  = new QHBoxLayout();
    do_cla->setLayout(qv);

    QToolButton *qt1 = new QToolButton();
    QToolButton *qt2 = new QToolButton();
    QToolButton *qt3 = new QToolButton();

    qt1->resize(250,250);
    qt2->resize(250,250);
    qt3->resize(250,150);

    qt1->connect(qt1,SIGNAL(released()),this,SLOT(new_win1()));
    qt2->connect(qt2,SIGNAL(released()),this,SLOT(new_win2()));
    qt3->connect(qt3,SIGNAL(released()),this,SLOT(new_win3()));
    qt1->setText("Others");
    qt2->setText("Sports");
    qt3->setText("Travel");

    qv->addWidget(qt1);
    qv->addWidget(qt2);
    qv->addWidget(qt3);

    do_cla->show();
}

void ThePlayer::new_win1(){
    new1 = new QWidget();
    new1->setWindowTitle("Others");

    int_list.clear();


    QVBoxLayout *new_qv = new QVBoxLayout();
    new1->setLayout(new_qv);

    QListWidget *new_ql = new QListWidget();
    new_ql->setSelectionMode(QAbstractItemView::ExtendedSelection);

    QToolButton *add = new QToolButton();
    add->setText("ADD");

    QToolButton *dele = new QToolButton();
    dele->setText("Delete");

    new_qv->addWidget(new_ql);
    new_qv->addWidget(add);
    new_qv->addWidget(dele);

    connect(add,SIGNAL(released()),this,SLOT(add_new_items()));
    connect(dele,SIGNAL(released()),this,SLOT(dele_old_items()));

    for(int i = 0; i < int(lists.size());i++){
        The_ql_item *ql_item;
        if(find_vector(lists.at(i),outhome))         ql_item = new The_ql_item(lists.at(i)->return_label()->text()+" (Others)");
        else ql_item = new The_ql_item(lists.at(i)->return_label()->text());
        new_ql->connect(new_ql,SIGNAL(itemClicked(QListWidgetItem*)),ql_item,SLOT(return_int(QListWidgetItem*)));
        connect(ql_item,SIGNAL(get_a(int)),this,SLOT(qs_action(int)));
        ql_item->Set_int(i);
        new_ql->addItem(ql_item->return_QListWidgetItem());
    }

    new1->show();
}

void ThePlayer::new_win2(){
    new1 = new QWidget();
    new1->setWindowTitle("Sports");

    int_list.clear();


    QVBoxLayout *new_qv = new QVBoxLayout();
    new1->setLayout(new_qv);

    QListWidget *new_ql = new QListWidget();
    new_ql->setSelectionMode(QAbstractItemView::ExtendedSelection);

    QToolButton *add = new QToolButton();
    add->setText("ADD");

    QToolButton *dele = new QToolButton();
    dele->setText("Delete");

    new_qv->addWidget(new_ql);
    new_qv->addWidget(add);
    new_qv->addWidget(dele);

    connect(add,SIGNAL(released()),this,SLOT(add_new_items()));
    connect(dele,SIGNAL(released()),this,SLOT(dele_old_items()));

    for(int i = 0; i < int(lists.size());i++){
        The_ql_item *ql_item;
        if(find_vector(lists.at(i),buzhidao))         ql_item = new The_ql_item(lists.at(i)->return_label()->text()+" (Sports)");
        else ql_item = new The_ql_item(lists.at(i)->return_label()->text());
        new_ql->connect(new_ql,SIGNAL(itemClicked(QListWidgetItem*)),ql_item,SLOT(return_int(QListWidgetItem*)));
        connect(ql_item,SIGNAL(get_a(int)),this,SLOT(qs_action(int)));
        ql_item->Set_int(i);
        new_ql->addItem(ql_item->return_QListWidgetItem());
    }

    new1->show();
}

void ThePlayer::new_win3(){
    new1 = new QWidget();
    new1->setWindowTitle("Travel");

    int_list.clear();


    QVBoxLayout *new_qv = new QVBoxLayout();
    new1->setLayout(new_qv);

    QListWidget *new_ql = new QListWidget();
    new_ql->setSelectionMode(QAbstractItemView::ExtendedSelection);

    QToolButton *add = new QToolButton();
    add->setText("ADD");

    QToolButton *dele = new QToolButton();
    dele->setText("Delete");

    new_qv->addWidget(new_ql);
    new_qv->addWidget(add);
    new_qv->addWidget(dele);

    connect(add,SIGNAL(released()),this,SLOT(add_new_items()));
    connect(dele,SIGNAL(released()),this,SLOT(dele_old_items()));

    for(int i = 0; i < int(lists.size());i++){
        The_ql_item *ql_item;
        if(find_vector(lists.at(i),anthoner))         ql_item = new The_ql_item(lists.at(i)->return_label()->text()+" (Travel)");
        else ql_item = new The_ql_item(lists.at(i)->return_label()->text());
        new_ql->connect(new_ql,SIGNAL(itemClicked(QListWidgetItem*)),ql_item,SLOT(return_int(QListWidgetItem*)));
        connect(ql_item,SIGNAL(get_a(int)),this,SLOT(qs_action(int)));
        ql_item->Set_int(i);
        new_ql->addItem(ql_item->return_QListWidgetItem());
    }

    new1->show();
}

void ThePlayer::qs_action(int a){
    if(find_int(a)==false) int_list.push_back(a);
}

void ThePlayer::add_new_items(){
    if(int_list.size() == 0) return;
    vector<The_List*> * add_lists;

    if(new1->windowTitle().toStdString()=="Others") add_lists = &outhome;
    else if(new1->windowTitle().toStdString()=="Sports") add_lists = &buzhidao;
    else if(new1->windowTitle().toStdString()=="Travel") add_lists = &anthoner;
    else return;

    for(auto b:int_list){
        if(find_vector(lists.at(b),*add_lists)==false)   add_lists->push_back(lists.at(b));
    }

    new1->close();
}

void ThePlayer::dele_old_items(){
    if(int_list.size() == 0) return;
    vector<The_List*> * add_lists;
    if(new1->windowTitle().toStdString()=="Others") add_lists = &outhome;
    else if(new1->windowTitle().toStdString()=="Sports") add_lists = &buzhidao;
    else if(new1->windowTitle().toStdString()=="Travel") add_lists = &anthoner;
    else return;

    for(auto b:int_list){
        for(int i = 0;i<int(add_lists->size());i++){

            if(lists.at(b)==add_lists->at(i)) add_lists->erase(add_lists->begin()+i);
        }
    }
    new1->close();
}
