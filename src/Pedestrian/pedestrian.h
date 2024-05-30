#ifndef PEDESTRIAN_H
#define PEDESTRIAN_H

#include <vector>
#include "src/Event/Event.h"
#include "src/utility/Utility.h"
#include "src/Ward/Ward.h"

using namespace std;

enum class Walkability {
    noDisability,
    crutches,
    sticks,
    wheelchairs,
    blind
};

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
    static int ID;
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
};

class Personal : public Pedestrian
{
public:
    Personal();
};

class Visitor : public Pedestrian {
public:
    Walkability walkability;
};

class Patient : public Pedestrian {
public:
    Walkability walkability;
};


#endif