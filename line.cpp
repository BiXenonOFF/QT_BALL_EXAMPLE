#include "line.hpp"

Line::Line()
{
    this->speed = 0;
    this->position = 0;
}

Line::Line(int position, int speed)
{
    this->position = position;
    this->speed = speed;
}

const int& Line::GetSetPosition() const{return this->position;}
int& Line::GetSetPosition(){return this->position;}
const int& Line::GetSetSpeed()const{return this->speed;}
int& Line::GetSetSpeed(){return this->speed;}
