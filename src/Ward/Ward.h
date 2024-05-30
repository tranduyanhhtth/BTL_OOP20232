#ifndef WARD_H
#define WARD_H

#include <string>
using namespace std;

class Point
{
    double x;
    double y;
};

class Ward 
{
    string nameDepartment;
    Point enter;
    Point exit;
    Point pointA, pointB, pointC, pointD;
};

#endif