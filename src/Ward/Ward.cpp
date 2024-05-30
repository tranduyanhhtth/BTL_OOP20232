#include "src/Ward/Ward.h"

using namespace std;

Ward::Ward() 
{
    this->nameDepartment = "";

    this->entry = setPoint(0, 0);
    this->exit = setPoint(0, 0);

    this->pointA = setPoint(0, 0);
    this->pointB = setPoint(0, 0);
    this->pointC = setPoint(0, 0);
    this->pointD = setPoint(0, 0);

    a.start.set(0, 0);
    a.end.set(0, 0);

    b.start.set(0, 0);
    b.end.set(0, 0);

    c.start.set(0, 0);
    c.end.set(0, 0);

    d.start.set(0, 0);
    d.end.set(0, 0);
}

Ward::Ward(string name, Point entry, Point exit, double length) 
{
    this->nameDepartment = name;

    this->entry = setPoint(entry.getX(), entry.getY());
    this->exit = setPoint(exit.getX(), exit.getY());

    this->pointA = setPoint(entry.getX() - length / 2, entry.getY());
    this->pointB = setPoint(entry.getX() + length / 2, entry.getY());
    this->pointC = setPoint(exit.getX() + length / 2, exit.getY());
    this->pointD = setPoint(exit.getX() - length / 2, exit.getY());

    a.start.set(pointA.getX(), pointA.getY(), 0.0);
    a.end.set(pointB.getX(), pointB.getY(), 0.0);

    b.start.set(pointB.getX(), pointB.getY(), 0.0);
    b.end.set(pointC.get(), pointC.getY(), 0.0);

    c.start.set(pointC.getX(), pointC.getY(), 0.0);
    c.end.set(pointD.get(), pointD.getY(), 0.0);

    d.start.set(pointD.getX(), pointD.getY(), 0.0);
    d.end.set(pointA.getX(), pointA.getY(), 0.0);
}