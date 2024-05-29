#ifndef PEDESTRIAN_H
#define PEDESTRIAN_H

#include <vector>
#include "src/AGVEvent/AGVEvent.h"
#include <random>
using namespace std;

class Point
{
    double x;
    double y;
};

class Ward 
{
    Point enter;
    Point exit;
    Point A, B, C, D;
};


enum class Walkability {
    noDisability,
    crutches,
    sticks,
    wheelchairs,
    blind
};

Walkability getRandomWalkability() {
    int ranNumber = randomInt(0, 4);
    if (ranNumber == 0) return Walkability::noDisability;
    if (ranNumber == 1) return Walkability::crutches;
    if (ranNumber == 2) return Walkability::sticks;
    if (ranNumber == 3) return Walkability::wheelchairs;
    return Walkability::blind;
}

class Emotion {
public:
    double pleasure = 0.75;
    double surprise = 0.5;
    double anger = -0.2;
    double fear = -0.2;
    double hate = -0.4;
    double sad = -0.4;
};

class Personality
{
    double lambda;
    double positiveEmotionThreshold;
    double negativeEmotionThreshold;
};


class Pedestrian
{ 
private:
    int ID;
    Ward start;
    Ward end;
    vector<Ward> journey;
    double velocity;
    Personality personality;
    Emotion emotion;
    vector<Event> events;
    double walkingTime;
    double distance;
    double age;
    AGVEvent impactOfAGV;
    Point tempPoints;
//public:
    //method
};

#endif