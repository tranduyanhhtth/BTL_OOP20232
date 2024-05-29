#ifndef PATIENT_H
#define PATIENT_H

#include "src/Pedestrian/pedestrian.h"

class Patient : public Pedestrian {
public:
    Walkability walkability;
};

#endif