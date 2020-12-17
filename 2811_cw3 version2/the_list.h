#ifndef THE_LIST_BUTTON_H
#define THE_LIST_BUTTON_H

#include<QString>
#include<QLabel>
#include"the_button.h"
#include<iostream>
#include<QLineEdit>

using namespace std;

class The_List : public QObject{
Q_OBJECT
private:
    QString qs;
    string str;

    QLabel *ql;

    TheButton *the_button;
public:

    The_List(QString qs){
        this->qs = qs;
        process_ql(qs.toStdString());
        this->ql= new QLabel();
        this->ql->setText(QString::asprintf("%s",str.c_str()));
    }

    The_List(QString qs, TheButton *the_button){
        this->qs=qs;
        this->the_button=the_button;
        process_ql(qs.toStdString());
        this->ql= new QLabel();
        this->ql->setText(QString::asprintf("%s",str.c_str()));
    }
    ~The_List(){}

    QLabel * return_label(){
        return  ql;
    }

    QString  return_Qstring(){
        return qs;
    }

    TheButton * return_TheButton(){
        return the_button;
    }

    void set_the_button(TheButton *the_bu){
        this->the_button = the_bu;
    }

    void process_ql(string str){
        size_t find = str.find("/");
        string sub = str.substr(find+1,str.size()-find);

        while(find!=string::npos){
             find = sub.find("/");
             sub = sub.substr(find+1,sub.size()-find);
        }

        this->str = sub;
    }

    bool find_Key(QString qstr){
        string ch_str = qstr.toStdString();

        size_t find = str.find(ch_str);

        if(find!=string::npos) return true;
        else return false;
    }

private slots:
    void seach_result(QString qstr){
        if(find_Key(qstr)) cout << "hello" << endl;
    }
};

#endif // THE_LIST_BUTTON_H
