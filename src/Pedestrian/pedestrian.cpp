
#include "src/Pedestrian/pedestrian.h"
#include <lib/nlohmann/json.hpp>

using json = nlohmann::json;
using namespace Utility;
using namespace std;

Walkability getRandomWalkability() {
    int ranNumber = randomInt(0, 4);
    if (ranNumber == 0) return Walkability::noDisability;
    if (ranNumber == 1) return Walkability::crutches;
    if (ranNumber == 2) return Walkability::sticks;
    if (ranNumber == 3) return Walkability::wheelchairs;
    return Walkability::blind;
}

