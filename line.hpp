#ifndef LINE_HPP
#define LINE_HPP


class Line
{
    int position;
    int speed;

public:

    Line();
    Line(int position,int speed);

    const int& GetSetPosition()const;
    int& GetSetPosition();
    const int& GetSetSpeed()const;
    int& GetSetSpeed();
};

#endif // LINE_HPP
