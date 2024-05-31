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