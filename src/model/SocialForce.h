#ifndef SOCIAL_FORCE_H
#define SOCIAL_FORCE_H

#include "src/agv/AGV.h"
#include "src/agent/Agent.h"
#include "src/wall/Wall.h"

//update lib in here//
#include "src/Personel/personel.h"
#include "src/Patient/patient.h"
#include "src/Visitor/visitor.h"

//***//

#include <vector>
#include <algorithm>

class SocialForce {
private:
    std::vector<Agent *> crowd;
    std::vector<Wall *> walls;
    std::vector<AGV *> agvs;

    //update code in here//
    std::vector<Ward *> wards;
    std::vector<Pedestrian *> pedestrians;
    //***//

public:
    // SocialForce();
    ~SocialForce();

    void addAgent(Agent *agent);

    void addWall(Wall *wall);

    //update
    void addWard(Ward *wrad);

    //update
    void addPedestrian(Pedestrian *pedestrian);

    void addAGV(AGV *agv);

    const std::vector<Agent *> getCrowd() const { return crowd; }

    int getCrowdSize() const { return crowd.size(); }

    const std::vector<Wall *> getWalls() const { return walls; }

    int getNumWalls() const { return walls.size(); }

    const std::vector<AGV *> getAGVs() const { return agvs; }

    int getNumAGVs() const { return agvs.size(); }

    void removeAgent(int agentId);
    void removeCrowd(); // Remove all individuals and groups
    void removeWalls();

    //update
    void removeWards();

    //update
    void removePedestrians();

    
    void removeAGV();
    void removeAGVs();

    void moveCrowd(float stepTime);

    void moveAGVs(float stepTime);
};

#endif