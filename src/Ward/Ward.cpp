#include "src/Ward/Ward.h"

using namespace std;

Ward::Ward() 
{
}

Ward::Ward(string name, Point entry, Point exit, double length) 
{
    nameDepartment = name;

    this->entry.setPoint(entry.getX(), entry.getY());
    this->exit.setPoint(exit.getX(), exit.getY());

    pointA.setPoint(entry.getX() - length / 2, entry.getY());
    pointB.setPoint(entry.getX() + length / 2, entry.getY());
    pointC.setPoint(exit.getX() + length / 2, exit.getY());
    pointD.setPoint(exit.getX() - length / 2, exit.getY());
}