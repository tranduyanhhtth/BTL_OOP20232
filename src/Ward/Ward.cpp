#include "Ward.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

vector<Ward> genWard() {
    vector<Ward> wards;
    ifstream file("data/hospital.txt");
    string line;
    vector<Point> midPoints; 
    if (file.is_open()) {
        getline(file, line);
        for (int i = 2;i < 11;i++) { 
            vector<Point>wall;
            vector<pair<Point,Point>> entry,exit;
            Point midPoint1, midPoint2, pointA, pointB, pointC, pointD;
            Ward ward;
            double length;
            string name;
            if (!getline(file, line)) {
                cout << "Cann't read file";
                break;
            }
            istringstream iss(line);
            iss >> midPoint1.x >> midPoint1.y;
            iss >> midPoint2.x >> midPoint2.y;
            iss >> length;
            iss >> name;

            pointA.x = midPoint1.x - length / 2;
            pointA.y = midPoint1.y;
            pointB.x = midPoint1.x + length / 2;
            pointB.y = midPoint1.y;
            pointC.x = midPoint2.x + length / 2;
            pointC.y = midPoint2.y;
            pointD.x = midPoint2.x - length / 2;
            pointD.y = midPoint2.y;

            entry.push_back(make_pair(Point(midPoint1.x-0.3, midPoint1.y),Point(midPoint1.x+0.3, midPoint1.y)));  
            exit.push_back(make_pair(Point(midPoint2.x-0.3,midPoint2.y),Point(midPoint2.x+0.3, midPoint2.y)));
            wall.push_back(pointA);
            wall.push_back(pointB);
            wall.push_back(pointC);
            wall.push_back(pointD);

            ward.setEntry(entry);
            ward.setExit(exit);
            ward.setName(name);
            ward.setWallCoordinates(wall);

            wards.push_back(Ward(name, entry, exit, wall));
        }
        vector<Point>wall;
        vector<pair<Point,Point>> entry,exit;
        Ward ward;
        Point entry1,entry2,exit1,exit2, pointA, pointB, pointC, pointD;
        if (!getline(file, line)) {
            file.close();
            cout << "Can't read ward A position";
            return wards;
        }
        istringstream iss(line);
        iss >> entry1.x >> entry1.y >> exit1.x >> exit1.y;
        iss >> entry2.x >> entry2.y >> exit2.x >> exit2.y;

        entry.push_back(make_pair(Point(entry1.x-0.3, entry1.y),Point(entry1.x+0.3, entry1.y)));
        entry.push_back(make_pair(Point(entry2.x, entry2.y-0.3),Point(entry2.x, entry2.y+0.3)));
        exit.push_back(make_pair(Point(exit1.x-0.3, exit1.y),Point(exit1.x+0.3, exit1.y)));
        exit.push_back(make_pair(Point(exit2.x, exit2.y-0.3),Point(exit2.x, exit2.y+0.3)));

        pointA.x = pointD.x = entry2.x;
        pointA.y = pointB.y = entry1.y;
        pointB.x = pointC.x = exit2.x;
        pointC.y = pointD.y = exit1.y;

        wall.push_back(pointA);
        wall.push_back(pointB);
        wall.push_back(pointC);
        wall.push_back(pointD);
        
        wards.push_back(Ward("A", entry, exit, wall));
        file.close();
    } else {
        cout << "File not found" << endl;
    }
    return wards;
}

vector<Point> genAroundWard(){
    vector<Point> aroundWard;
    ifstream file("data/hospital.txt");
    string line;
    if(file.is_open()){
        for(int i=1;i<15;i++){
            getline(file, line); 
        }
        double x,y;
        istringstream iss(line);
        for(int i=0;i<4;i++){
            iss>>x>>y;
            aroundWard.push_back(Point(x,y));
        }
        file.close();
    }
    else{
        cout << "File not found" << endl;
    }
    return aroundWard;
}
