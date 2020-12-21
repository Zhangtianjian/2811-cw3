#ifndef THE_QL_ITEM_H
#define THE_QL_ITEM_H

#include<QListWidgetItem>
#include<iostream>

using namespace std;

class The_ql_item : public QObject{
    Q_OBJECT
private:
    int a;
     QListWidgetItem *q_li;
public:
    The_ql_item(QString qs){
        q_li = new QListWidgetItem(qs);
    }

    QListWidgetItem * return_QListWidgetItem(){
        return q_li;
    }

    void Set_int(int a){
        this->a = a;
    }
    int return_a(){
        return  a;
    }

signals:
    void get_a(int);


private slots:
    void return_int(QListWidgetItem* ql){
        if(ql==this->q_li) emit get_a(a);
    }

};

#endif // THE_QL_ITEM_H
