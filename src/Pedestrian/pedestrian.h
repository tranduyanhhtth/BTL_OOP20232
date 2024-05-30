#ifndef PEDESTRIAN_H
#define PEDESTRIAN_H

#include <vector>
#include "src/AGVEvent/AGVEvent.h"
#include "src/utility/Utility.h"

using namespace Utility;
using namespace std;

#include "src/Ward/Ward.h"
#include "src/Emotion/Emotion.h"

enum class Walkability {
    noDisability,
    crutches,
    sticks,
    wheelchairs,
    blind
};

Walkability getRandomWalkability();

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
};

#endif