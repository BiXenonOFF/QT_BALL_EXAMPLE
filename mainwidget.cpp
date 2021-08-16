#include "mainwidget.hpp"
#include "ui_mainwidget.h"
#include <QTimer>
#include <string>
#include <QPainter>
#include <random>
#include <QKeyEvent>
#include "line.hpp"


// рандом
int GetRandom(int min, int max)
{
    static std::mt19937 prng(std::random_device{}());
    return std::uniform_int_distribution<>(min,max)(prng);
}

struct List
{
    List* next;
    Line l;
};

List* CreateElement(Line l)
{
    List* res = new List;
    res->next = nullptr;
    res->l = l;
    return res;
}

List* AddElement(List* head, Line l)
{
    List* curr = head;
    if(head==nullptr)
    {
        head = CreateElement(l);
    }
    else
    {
        while(curr->next!=nullptr) curr = curr->next;
        curr->next = CreateElement(l);
    }
    return head;
}

bool NeedRemove(List* head,int height)
{
    List* curr = head;
    while(curr)
    {
        if(curr->l.GetSetPosition()>height) return true;
        curr = curr->next;
    }
    return false;
}

List* Remove_At(List* head,int id)
{
    List* curr = head;
    List* prev = nullptr;
    if(id==0)
    {
        if(head->next==nullptr)
        {
            delete head;
            head = nullptr;
        }
        else
        {
            curr = head->next;
            delete head;
            head = curr;
        }
    }
    else
    {
        for(int a = 1; a<id; a++)
        {
            curr = curr->next;
        }
        prev = curr;
        curr = curr->next;
        if(prev->next->next==nullptr)
        {
            prev->next = nullptr;
            delete curr;
        }
        else
        {
            prev->next = prev->next->next;
            delete curr;
        }
    }
    return head;
}




List* CheckAndRemoveElement(List* head, int height)
{
    List* curr = nullptr;

    int remove_at=0;

    curr = head;

    while(NeedRemove(head,height))
    {
        curr=head;
        remove_at = 0;
        while(curr)
        {
            if(curr->l.GetSetPosition()>height) break;
            remove_at+=1;
            curr = curr->next;
        }
        head = Remove_At(head,remove_at);
    }

    return head;
}



/*
 * глобальные переменные
*/

List* head = nullptr; // голова списка

long long int Timer_counter = 0; // счетчик
bool visible_flag = true; // видимый ли шарик
bool Collision = false; // произошло ли столкновение

const int max_speed = 20; // макс скорость линии
const int min_speed = 10; // мин скорость линии

const int Line_Width = 5; // толщина линий

int Visible_Timer = 100; // счетчик на выход из невидимости

bool pause = false; // проверка на паузу

/*
 * конец переменных
*/

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    this->setFocusPolicy(Qt::StrongFocus);

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timer->start(100);
}

MainWidget::~MainWidget()
{
    delete ui;
}

// работа таймера
void MainWidget::onTimeout()
{
    if(!pause)
    {
        ui->Pause_label->setText("");

        if(!Collision)
        {
            Timer_counter+=1;
            ui->Score_label->setText("Score: "+QString::number(Timer_counter));

            if(visible_flag)
            {
                Visible_Timer = 100;
            }
            else
            {
                Visible_Timer-=1;
            }

            ui->Invis_show->setText("Invisible: "+QString::number((double)Visible_Timer/10));

            if(Visible_Timer == 0)
            {
                Visible_Timer = 100;
                visible_flag = true;
            }

            // новая линия
            if(Timer_counter%10==0)
            {
                head = AddElement(head,Line(40,GetRandom(min_speed,max_speed)));
            }

            // проверка и уничтожение прошедших линий
            head = CheckAndRemoveElement(head,this->window()->height()-95);

            // проверка на столкновение
            if(visible_flag)
            {
                List* curr = head;
                while(curr)
                {
                    if(curr->l.GetSetPosition()+Line_Width>=this->window()->height()-120&&curr->l.GetSetPosition()<=this->window()->height()-70)
                    {
                        Collision = true;
                        break;
                    }
                    curr = curr->next;
                }
            }

            // перемещение линии
            List* curr = head;
            while(curr)
            {
                curr->l.GetSetPosition()+=curr->l.GetSetSpeed();
                curr = curr->next;
            }


        }
        else
        {
            ui->Pause_label->setText("GAME OVER");
        }
    }
    else
    {
        if(!Collision)ui->Pause_label->setText("GAME PAUSED");
        else
        {
            ui->Pause_label->setText("GAME OVER");
        }
    }
    this->repaint();
}

//перерисовка
void MainWidget::paintEvent(QPaintEvent *event)
{
    QPainter graph(this);
    QColor color;

    if(visible_flag)
    {
        color.setRed(255);
        color.setGreen(7);
        color.setBlue(0);
        graph.setBrush(color);
        graph.drawEllipse(this->window()->width()/2-25,this->window()->height()-100,50,50);
    }
    else
    {
        color.setRed(255);
        color.setGreen(147);
        color.setBlue(144);
        graph.setBrush(color);
        graph.drawEllipse(this->window()->width()/2-25,this->window()->height()-100,50,50);
    }

    List* curr = head;

    while(curr)
    {
        color.setRed(0);
        color.setGreen(0);
        color.setBlue(0);
        graph.setBrush(color);
        graph.drawRect(0,curr->l.GetSetPosition(),this->window()->width(),Line_Width);
        curr=curr->next;
    }

}

// нажатие клавиш
void MainWidget::keyPressEvent(QKeyEvent *event)
{
    if(!pause&&!Collision)
    {
        // нажали на пробел
        if(event->key()==Qt::Key_Space)
        {
            visible_flag = !visible_flag;
        }
    }

    if(event->key()==Qt::Key_P)
    {
        pause = !pause;
    }



    this->repaint();
}

