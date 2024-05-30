#ifndef WARD_H
#define WARD_H

#include <string>
#include "src/wall/Wall.h"
using namespace std;

class Point
{
private:
    double x;
    double y;
public:
    Point()
    {
        this->x = 0;
        this->y = 0;
    }
    void setPoint(double x, double y) {
        this->x = x;
        this->y = y;
    }

    double getX() {return x;}
    double getY() {return y;}
};

class Ward 
{
private:
    string nameDepartment;
    Point entry;
    Point exit;

    Point entryPlus; //for A
    Point exitPlus; //for A

    Point pointA, pointB, pointC, pointD;
public:
    Ward();

    Ward(string name, Point entry, Point exit, double length);

};

#endif