#ifndef WARD_H
#define WARD_H

#include <string>
#include <vector>

using namespace std;

class Point
{
public:
    double x;
    double y;
    
    Point(){}

    Point(double x, double y)  {
        this->x = x;
        this->y = y;
    } 
};

/*
class Ward 
{
private:
    string nameDepartment;
    Point entry;
    Point exit;
    Point entry_plus;
    Point exit_plus;
    Point pointA, pointB, pointC, pointD;
public:
    Ward(){}

    Ward(string nameDepartment, Point entry, Point exit, Point pointA, Point pointB, Point pointC, Point pointD) {
        this->nameDepartment = nameDepartment;
        this->entry = entry;
        this->exit = exit;
        this->pointA = pointA;
        this->pointB = pointB;
        this->pointC = pointC;
        this->pointD = pointD;
    }

    Ward(string nameDepartment, Point entry, Point entry_plus, Point exit, Point exit_plus, Point pointA, Point pointB, Point pointC, Point pointD) {
        this->nameDepartment = nameDepartment;
        this->entry = entry;
        this->entry_plus = entry_plus;
        this->exit = exit;
        this->exit_plus = exit_plus;
        this->pointA = pointA;
        this->pointB = pointB;
        this->pointC = pointC;
        this->pointD = pointD;
    }

    string getName() {return nameDepartment;}
    void setName(string nameDepartment) {this->nameDepartment = nameDepartment;}

    Point getEntry() {return entry;}
    void setEnter(Point enter) {this->entry = entry;}

    Point getExit() {return exit;}
    void setExit(Point exit) {this->exit = exit;}

    Point getEntryPlus() {return entry_plus;}
    void setEntryPlus(Point entry_plus) {this->entry_plus = entry_plus;}

    Point getExitPlus() {return exit_plus;}
    void setExitPlus(Point exit_plus) {this->exit_plus = exit_plus;}

    Point getPointA() {return pointA;}
    void setPointA(Point pointA) {this->pointA = pointA;}

    Point getPointB() {return pointB;}  
    void setPointB(Point pointB) {this->pointB = pointB;}

    Point getPointC() {return pointC;}
    void setPointC(Point pointC) {this->pointC = pointC;}

    Point getPointD() {return pointD;}
    void setPointD(Point pointD) {this->pointD = pointD;}

};
*/

class Ward{
private:
    string name;
    vector<pair<Point,Point>> entry,exit;
    vector<Point> wallCoordinates;

public:
    Ward(){}
    Ward(string name, vector<pair<Point,Point>> entry, vector<pair<Point,Point>> exit, vector<Point> wallCoordinates) {
        this->name = name;
        this->entry = entry;
        this->exit = exit;
        this->wallCoordinates = wallCoordinates;
    }
    
    string getName() const { return name; }
    void setName(string name) { this->name = name; }

    vector<pair<Point,Point>> getEntry() const { return entry; }
    void setEntry(vector<pair<Point,Point>> entry) { this->entry = entry; }
    
    vector<pair<Point,Point>> getExit() const { return this->exit; }
    void setExit(vector<pair<Point,Point>> exit) { this->exit = exit; }

    vector<Point> getWallCoordinates() { return wallCoordinates; }
    void setWallCoordinates(vector<Point> wall){ this->wallCoordinates=wall; }
};

vector<Ward> genWard();
vector<Point> genAroundWard();

#endif