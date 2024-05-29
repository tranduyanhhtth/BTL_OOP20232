#ifndef VISITOR_H
#define VISITOR_H

#include "src/Pedestrian/pedestrian.h"

class Visitor : public Pedestrian {
public:
    Walkability walkability;
};

#endif