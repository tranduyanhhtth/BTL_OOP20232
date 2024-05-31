#if defined(__linux__)
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include <random>
#include <lib/nlohmann/json.hpp>

#include "model/SocialForce.h"
#include "constant/Constant.h"
#include "renderer/Renderer.h"

using namespace std;
using namespace Constant;
using namespace Renderer;
using json = nlohmann::json;

// Global Variables
GLsizei winWidth = 1600; // Window width (16:10 ratio)
GLsizei winHeight = 660; // Window height (16:10 ratio)
SocialForce *socialForce;
float fps = 0; // Frames per second
int currTime = 0; // = 0
int startTime = 0; // = 0
bool animate = false; // Animate scene flag
float speedConsiderAsStop = 0.2; // = 0.2

//là một đối tượng json, có nội dung giống trong file input.json
json inputData;

std::map<std::string, std::vector<float>> mapData; //là một map gồm tên giao lộ và chiều dài mở rộng của các giao lộ

//
std::vector<json> juncDataList; //vector chứa các đối tượng json của giao lộ

std::vector<float> juncData; //vector chứa dữ liệu của giao lộ

std::string juncName; //Tên giao lộ

int juncIndex = 0; // Chỉ số giao lộ = 0

float walkwayWidth; //chiều rộng lối đi

int classificationType = 0; // = 0

//chứa số lượng người có trong các luồng, size = inputData.size()
std::vector<int> numOfPeople; 

float minSpeed = -1;
float maxSpeed = -1;
int threshold = 0;


// Function Prototypes
void init();

// void createWalls();
//update code in here 
void createWalls_hospital();

void createLabel();
//******//
void createAgents();

void createPedestrians();

void createAGVs();

void display();

void reshape(int width, int height);

void normalKey(unsigned char key, int xMousePos, int yMousePos);

void update();

int main(int argc, char **argv)
{
    inputData = Utility::readInputData("data/input.json");
    mapData = Utility::readMapData("data/map.txt");
    std::string input1; //kịch bản

    if ((int)inputData["runMode"]["value"] == 0) //chế độ runMode = 0
    {
        do
        {
            cout << "Select the type of traffic you want to simulate" << endl;
            cout << "1. Hallway" << endl;
            cout << "2. Junction" << endl;
            cout << "Your choice: ";
            getline(cin, input1); //lựa chọn kịch bản
            if (input1 == "1") //chạy kịch bản ở hành lang
            {
                walkwayWidth = (float)inputData["hallwayWidth"]["value"]; //lấy chiều rộng lối đi trong inputJson
                //chiều dài hành lanh
                float length1Side = ((float)inputData["hallwayLength"]["value"]) / 2;
                juncData = {length1Side, length1Side}; //lưu trữ chiều dài của hành lang trên và dưới
            }
            else if (input1 == "2") //chạy kịch bản chỉ định
            {
                do
                {
                    cout << "Please enter the junction you want to emulate" << endl;
                    cout << "(Press enter to randomly select a junction in the map)" << endl;
                    cout << "Your choice: ";
                    getline(cin, juncName); //Nhập tên giao lộ
                    if (juncName == "") //Kịch bản ngẫu nhiên nếu không nhập gì
                    {
                        auto it = mapData.begin(); //it trỏ tới phần tử đầu tiên của map
                        std::advance(it, Utility::randomInt(1, mapData.size() - 3)); //lấy random
                        std::string random_key = it->first; //ramdom_key là con trỏ trỏ tới phần tử ngẫu nhiên trong đối trượng mapjson

                        juncName.assign(random_key); //sẽ là một trong các giao lộ ở file map.txt
                    }

                } while (mapData[juncName].size() < 3); //yêu cầu nhập lại nếu dữ liệu của giao lộ vừa nhập bé hơn 3
                juncData = mapData[juncName]; //lấy dữ liệu chiều dài của giao lộ
                walkwayWidth = mapData["walkwayWidth"][0]; //tính toán chiều rộng của lối đi
            }
        } while (input1 != "1" && input1 != "2"); //chỉ có 2 lựa chọn cho kịch bản
    }
    else //chế độ runMode = 1, kịch bản ở hành lang
    {
        juncDataList = Utility::convertMapData(mapData);
        //chiều dài lối đi
        float hallwayLength = juncDataList[juncIndex].items().begin().value();

        walkwayWidth = (float)inputData["hallwayWidth"]["value"];
        float length1Side = (hallwayLength) / 2;
        juncData = {length1Side, length1Side}; //dữ liệu chiều dài cạnh trên và cạnh dưới
    }


    float deviationParam = randomFloat(1 - (float)inputData["experimentalDeviation"]["value"] / 100, 1 + (float)inputData["experimentalDeviation"]["value"] / 100);
    //Ngưỡng người dừng ở hành lang
    threshold = int(inputData["numOfAgents"]["value"]) * deviationParam * (float)(inputData["stopAtHallway"]["value"]) / 100;

    //Xây dựng đồ họa với dữ liệu trên
    glutInit(&argc, argv); // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA |
                        GLUT_DEPTH);         // Set display mode  Default mode used
    glutInitWindowSize(winWidth, winHeight); // Set window width and height
    glutInitWindowPosition(90, 90);          // Set window position
    glutCreateWindow(
        "Crowd Simulation using Social Force"); // Set window title and create
    // display window

    animate = true;
    startTime = currTime;
    if ((int)inputData["graphicsMode"]["value"] == 0)
    {
        glutHideWindow();
    }

    init();                   // Initialization
    glutDisplayFunc(display); // Send graphics to display window
    glutReshapeFunc(reshape); // Maintain aspect ratio when window first created,
    // resized and moved

    glutKeyboardFunc(normalKey);
    glutIdleFunc(update); // Continuously execute 'update()'
    glutMainLoop();       // Enter GLUT's main loop

    return 0;
}


// int main(int argc, char **argv) {
//     inputData = Utility::readInputData("data/input.json");
//     if ((int)inputData["runMode"]["value"] != 3) {
//         cerr << "Khong co kich ban nay" << endl;
//         return -1;
//     }
// }

void createWalls_hospital(){
    vector<Point> wallCoordinates = genAroundWard();
    vector<Ward> wards = genWard();
    Wall *wall;

    /****Create around wall****/
    //upper wall
    wall = new Wall(wallCoordinates[3].x,wallCoordinates[3].y-1,wallCoordinates[0].x,wallCoordinates[0].y-1);
    wall->setWallColor(255,0,0);
    socialForce->addWall(wall);
    
    //lower wall
    wall = new Wall(wallCoordinates[2].x,wallCoordinates[2].y+1,wallCoordinates[1].x,wallCoordinates[1].y+1);
    wall->setWallColor(255,0,0);
    socialForce->addWall(wall);
    
    //left wall
    wall = new Wall(wallCoordinates[2].x,wallCoordinates[2].y+1,wallCoordinates[3].x,(wallCoordinates[2].y+1 + wallCoordinates[3].y-1) / 2 - 0.5);
    wall->setWallColor(255,0,0);
    socialForce->addWall(wall);

    wall = new Wall(wallCoordinates[3].x,(wallCoordinates[2].y+1 + wallCoordinates[3].y-1) / 2 + 0.5, wallCoordinates[3].x,wallCoordinates[3].y-1);
    wall->setWallColor(255,0,0);
    socialForce->addWall(wall);
    //left wall
    
    //right wall
    wall = new Wall(wallCoordinates[0].x,wallCoordinates[0].y-1,wallCoordinates[1].x,(wallCoordinates[0].y-1 + wallCoordinates[1].y+1) / 2 + 0.5);
    wall->setWallColor(255,0,0);
    socialForce->addWall(wall);

    wall = new Wall(wallCoordinates[1].x,(wallCoordinates[0].y-1 + wallCoordinates[1].y+1) / 2 - 0.5, wallCoordinates[1].x,wallCoordinates[1].y+1);
    wall->setWallColor(255,0,0);
    socialForce->addWall(wall);
    //right wall

    // Create walls for wards
    for(int i = 0; i < wards.size(); ++i){
        Ward ward = wards[i];
        vector<Point> wallCoordinates = ward.getWallCoordinates();
        vector<pair<Point,Point>> entry = ward.getEntry();
        vector<pair<Point,Point>> exit = ward.getExit();
        string name = ward.getName();
        
        //cout << name << endl;
        
        if(i != wards.size()-1){
            /*top*/
            wall = new Wall(wallCoordinates[0].x, wallCoordinates[0].y, entry[0].first.x, entry[0].first.y);
            wall->setWallColor(255,255,0);
            socialForce->addWall(wall);

            wall = new Wall(entry[0].second.x, entry[0].second.y, wallCoordinates[(0+1)%4].x, wallCoordinates[(0+1)%4].y);
            wall->setWallColor(255,255,0);
            socialForce->addWall(wall);
            /*top*/
            
            /*bottom*/
            wall = new Wall(wallCoordinates[2].x, wallCoordinates[2].y, exit[0].second.x, exit[0].second.y);
            wall->setWallColor(255,255,0);
            socialForce->addWall(wall);

            wall = new Wall(exit[0].first.x, exit[0].first.y, wallCoordinates[(2+1)%4].x, wallCoordinates[(2+1)%4].y);
            wall->setWallColor(255,255,0);
            socialForce->addWall(wall);
            /*bottom*/
            
            /*right*/
            wall = new Wall(wallCoordinates[1].x, wallCoordinates[1].y, wallCoordinates[(1+1)%4].x, wallCoordinates[(1+1)%4].y);
            wall->setWallColor(255,255,0);
            socialForce->addWall(wall);
            
            /*left*/
            wall = new Wall(wallCoordinates[3].x, wallCoordinates[3].y, wallCoordinates[(3+1)%4].x, wallCoordinates[(3+1)%4].y);
            wall->setWallColor(255,255,0);
            socialForce->addWall(wall);
        } else {
            /*top*/
            wall = new Wall(wallCoordinates[0].x, wallCoordinates[0].y, entry[0].first.x, entry[0].first.y);
            wall->setWallColor(255,255,0);
            socialForce->addWall(wall);

            wall = new Wall(entry[0].second.x, entry[0].second.y, wallCoordinates[(0+1)%4].x, wallCoordinates[(0+1)%4].y);
            wall->setWallColor(255,255,0);
            socialForce->addWall(wall);
            /*top*/
            
            /*bottom*/
            wall = new Wall(wallCoordinates[2].x, wallCoordinates[2].y, exit[0].second.x, exit[0].second.y);
            wall->setWallColor(255,255,0);
            socialForce->addWall(wall);

            wall = new Wall(exit[0].first.x, exit[0].first.y, wallCoordinates[(2+1)%4].x, wallCoordinates[(2+1)%4].y);
            wall->setWallColor(255,255,0);
            socialForce->addWall(wall);
            /*bottom*/

            /*left*/
            wall = new Wall(wallCoordinates[0].x, wallCoordinates[0].y, entry[1].second.x, entry[1].second.y);
            wall->setWallColor(255,255,0);
            socialForce->addWall(wall);

            wall = new Wall(entry[1].first.x, entry[1].first.y, wallCoordinates[(2+1)%4].x, wallCoordinates[(2+1)%4].y);
            wall->setWallColor(255,255,0);
            socialForce->addWall(wall);
            /*left*/
            
            /*right*/
            wall = new Wall(wallCoordinates[1].x, wallCoordinates[1].y, exit[1].second.x, exit[1].second.y);
            wall->setWallColor(255,255,0);
            socialForce->addWall(wall);

            wall = new Wall(exit[1].first.x, exit[1].first.y, wallCoordinates[(1+1)%4].x, wallCoordinates[(1+1)%4].y);
            wall->setWallColor(255,255,0);
            socialForce->addWall(wall);
            /*right*/
        }
    }
}

void createLabel(){
    vector<Ward> wards= genWard();
    for(Ward ward : wards){
        int num = 1;
        vector<pair<Point,Point>> entry = ward.getEntry();
        vector<pair<Point,Point>> exit = ward.getExit();
        string name = ward.getName();
        for (pair<Point,Point> point : entry){
            string entrName = name + to_string(num);
            if(name == "A"&& num == 2) {
                drawText(point.first.x,point.first.y,entrName.c_str());
            } else { 
                drawText(point.second.x-0.3,point.second.y-0.5,entrName.c_str());
            }
            ++num;
        }
        
        for (pair<Point,Point> point : exit){
            string exitName = name + to_string(num);
            if(name == "A"&& num == 3) {
                drawText(point.first.x,point.first.y,exitName.c_str());
            } else if(name == "A" && num == 4) {
                drawText(point.second.x-0.6,point.second.y-0.5,exitName.c_str());
            } else {    
                drawText(point.second.x-0.3,point.second.y,exitName.c_str());
            }
            ++num;
        }
    }
}

void init()
{
    // General Light Intensity
    GLfloat gnrlAmbient[] = {
        0.8F, 0.8F, 0.8F, 1.0}; // Ambient (R, G, B, A) light intensity of entire scene

    // Object Light Intensity
    GLfloat lghtDiffuse[] = {0.7F, 0.7F, 0.7F, 1.0}; // Diffuse (R, G, B, A) light intensity

    // Light Position
    GLfloat lghtPosition[] = {4.0, -4.0, 4.0, 0.0};

    glClearColor(1.0, 1.0, 1.0, 0.0);       // Set color used when color buffer cleared
    glShadeModel(GL_SMOOTH); // Set shading option

    // General Lighting
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, gnrlAmbient);

    // Object Lighting
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lghtDiffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, lghtPosition);

    glEnable(GL_DEPTH_TEST); // Enable hidden surface removal
    glEnable(GL_NORMALIZE);  // Normalize normal vector
    glEnable(GL_LIGHTING);   // Prepare OpenGL to perform lighting calculations
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_LIGHT0);

    glCullFace(GL_BACK);    // Specify face to be culled
    glEnable(GL_CULL_FACE); // Enable culling of specified face

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);

    srand(1604010629); // Seed to generate random numbers

    socialForce = new SocialForce;
    createWalls_hospital(); // new 
    // createWalls(); // old  
    createAgents();
    createAGVs();
}
/*
void createWalls()
{
    Wall *wall;

    vector<float> coors = Utility::getWallCoordinates(walkwayWidth, juncData);

    if (juncData.size() == 2)
    {
        // Upper Wall
        wall = new Wall(coors[0], coors[1], coors[2], coors[3]);
        socialForce->addWall(wall);
        // Lower Wall
        wall = new Wall(coors[4], coors[5], coors[6], coors[7]);
        socialForce->addWall(wall);
    }
    else
    {
        // Upper Wall
        if (juncData.size() == 4)
        {
            wall = new Wall(coors[0], coors[1], coors[2], coors[3]);
            socialForce->addWall(wall);

            wall = new Wall(coors[4], coors[5], coors[6], coors[7]);
            socialForce->addWall(wall);
        }
        else if (juncData.size() == 3)
        {
            wall = new Wall(coors[0], coors[1], coors[6], coors[7]);
            socialForce->addWall(wall);
        }

        // Lower Wall
        wall = new Wall(coors[8], coors[9], coors[10], coors[11]);
        socialForce->addWall(wall);

        wall = new Wall(coors[12], coors[13], coors[14], coors[15]);
        socialForce->addWall(wall);

        // Left Wall
        if (juncData.size() == 4)
        {
            wall = new Wall(coors[16], coors[17], coors[18], coors[19]);
            socialForce->addWall(wall);
        }

        wall = new Wall(coors[20], coors[21], coors[22], coors[23]);
        socialForce->addWall(wall);

        // Right Wall
        if (juncData.size() == 4)
        {
            wall = new Wall(coors[24], coors[25], coors[26], coors[27]);
            socialForce->addWall(wall);
        }

        wall = new Wall(coors[28], coors[29], coors[30], coors[31]);
        socialForce->addWall(wall);
    }
}
*/
void setAgentsFlow(Agent *agent, float desiredSpeed, float maxSpeed, float minSpeed, int caseJump)
{
    // if (socialForce->getCrowdSize() < threshold)
    // {
    //     agent->setStopAtCorridor(true);
    // }

    int codeSrc = 0;
    int codeDes = 0;

    int juncType = juncData.size();

    if (juncType == 4)
    {
        if (caseJump < 3)
        {
            codeSrc = 0; // Go from Left to Right
        }
        else if (caseJump < 6)
        {
            codeSrc = 1; // Go from Right to Left
        }
        else if (caseJump < 9)
        {
            codeSrc = 2; // Go from Top to Bottom
        }
        else
        {
            codeSrc = 3; // Go from Bottom to Top
        }
    }
    else if (juncType == 3)
    {
        if (caseJump < 6)
        {
            codeSrc = 0;
            if (caseJump % 2 == 0)
            {
                codeDes = 0;
            }
            else
            {
                codeDes = 2;
            }
        }
        else if (caseJump < 12)
        {
            codeSrc = 1;
            if (caseJump % 2 == 0)
            {
                codeDes = 1;
            }
            else
            {
                codeDes = 2;
            }
        }
        else if (caseJump < 18)
        {
            codeSrc = 3;
            if (caseJump % 2 == 0)
            {
                codeDes = 0;
            }
            else
            {
                codeDes = 1;
            }
        }
    }
    else if (juncType == 2)
    {
        if (caseJump < 3)
        {
            codeSrc = 0;
        }
        else
        {
            codeSrc = 1;
        }
    }

    vector<float> position = Utility::getPedesSource(
        codeSrc,
        (float)inputData["totalCrowdLength"]["value"],
        (float)inputData["headCrowdLength"]["value"],
        (float)inputData["crowdWidth"]["value"],
        walkwayWidth,
        juncData);
    vector<float> desList;
    if (juncType == 4 || juncType == 2)
    {
        desList = Utility::getPedesDestination(codeSrc, caseJump % 3, walkwayWidth, juncData, agent->getStopAtCorridor());
    }
    else if (juncType == 3)
    {
        desList = Utility::getPedesDestination(codeDes, caseJump % 3, walkwayWidth, juncData, agent->getStopAtCorridor());
    }

    agent->setPosition(position[0], position[1]);
    if (juncType == 3 && codeSrc != codeDes)
    {
        agent->setPath(randomFloat(-walkwayWidth / 2, walkwayWidth / 2), randomFloat(-walkwayWidth / 2, walkwayWidth / 2), 2.0);
    }
    agent->setPath(desList[0], desList[1], desList[2]);
    agent->setDestination(desList[0], desList[1]);
    agent->setDesiredSpeed(desiredSpeed);
    std::vector<float> color = getPedesColor(maxSpeed, minSpeed, agent->getDesiredSpeed(), classificationType);
    agent->setColor(color[0], color[1], color[2]);
    socialForce->addAgent(agent);
}

void createAgents()
{
    //đối tượng Agent
    Agent *agent;

    //inputData là một đối tượng json, có nội dung giống trong file input.json

    //độ lệch thực nghiệm
    float deviation = (float)inputData["experimentalDeviation"]["value"] / 100;

    //deviationParam là một gía trị ngẫu nhiên thuộc (1-deviation, 1+deviation)
    float deviationParam = randomFloat(1 - deviation, 1 + deviation);
    
    //juncData?
    //deviationParam?

    numOfPeople = Utility::getNumPedesInFlow(juncData.size(), int(int(inputData["numOfAgents"]["value"]) * deviationParam));
    
    //classificationType?
    //inputData?
    //deviationParam?
    
    //function getPedesVelocity?
    //danh sách vận tốc
    vector<double> velocityList = Utility::getPedesVelocity(classificationType, inputData, deviationParam);
    
    //velocityList?
    //what doing?
    if (classificationType == 0)
    {
        minSpeed = 0.52;
        maxSpeed = 2.28;
    }
    else
    {
        minSpeed = velocityList[0];
        maxSpeed = velocityList[velocityList.size() - 1];
    }
    //default_random_engine?
    //shuffle?
    auto rng = std::default_random_engine{};
    std::shuffle(velocityList.begin(), velocityList.end(), rng);
    int pedesCount = 0;

    //what doing?
    if (juncData.size() == 2)
    {
        for (int idx = 0; idx < 6; idx++)
        {
            for (int temp = 0; temp < numOfPeople[idx]; temp++)
            {
                agent = new Agent;
                setAgentsFlow(agent, velocityList[pedesCount], maxSpeed, minSpeed, idx);
                pedesCount = pedesCount + 1;
            }
        }
    }
    else if (juncData.size() == 3)
    {
        for (int idx = 0; idx < 18; idx++)
        {
            for (int temp = 0; temp < numOfPeople[idx]; temp++)
            {
                agent = new Agent;
                setAgentsFlow(agent, velocityList[pedesCount], maxSpeed, minSpeed, idx);
                pedesCount = pedesCount + 1;
            }
        }
    }
    else if (juncData.size() == 4)
    {
        for (int idx = 0; idx < 12; idx++)
        {
            for (int temp = 0; temp < numOfPeople[idx]; temp++)
            {
                agent = new Agent;
                setAgentsFlow(agent, velocityList[pedesCount], maxSpeed, minSpeed, idx);
                pedesCount = pedesCount + 1;
            }
        }
    }
}

void createWard()
{
    
}

void createPedestrians() 
{
}

void createAGVs()
{
    AGV *agv = NULL;
    vector<int> array;

    // test
    // agv = new AGV();
    // vector<Point3f> route = Utility::getRouteAGV(juncData.size(), 0, 2, walkwayWidth, juncData);
    // agv->setDirection(0, 2);
    // agv->setPosition(route[0].x, route[0].y);

    // for (Agent *agent : socialForce->getCrowd())
    // {
    //     if (agent->getPosition().distance(agv->getPosition()) < 0.5F)
    //     {
    //         do
    //         {
    //             agent->setPosition(agent->getPosition().x - 0.1F, agent->getPosition().y - 0.1F);
    //         } while (agent->getPosition().distance(agv->getPosition()) < 0.5F);
    //     }
    // }

    // agv->setDestination(route[route.size() - 1].x, route[route.size() - 1].y);
    // agv->setDesiredSpeed(0.6F);
    // agv->setAcceleration(inputData[9]);
    // agv->setDistance((float)inputData[10]);
    // for (int i = 1; i < route.size(); i++)
    // {
    //     agv->setPath(route[i].x, route[i].y, 1.0);
    //     std::cout << route[i] << endl;
    // }
    // socialForce->addAGV(agv);

    // test
    if ((int)inputData["runMode"]["value"] == 0)
    {
        for (int i = 0; i < juncData.size(); i++)
        {
            if (juncData.size() == 4)
            {
                array = {0, 1, 2};
            }
            else if (juncData.size() == 3)
            {
                if (i == 0)
                {
                    array = {1, 2};
                }
                else if (i == 1)
                {
                    array = {0, 2};
                }
                else
                {
                    array = {0, 1};
                }
            }
            else if (juncData.size() == 2)
            {
                array = {1};
            }

            for (int j : array)
            {
                agv = new AGV();
                vector<Point3f> route = Utility::getRouteAGV(i, j, walkwayWidth, juncData);
                agv->setDirection(i, j);
                agv->setPosition(route[0].x, route[0].y);

                // for (Agent *agent : socialForce->getCrowd())
                // {
                //     if (agent->getPosition().distance(agv->getPosition()) < 0.5F)
                //     {
                //         do
                //         {
                //             agent->setPosition(agent->getPosition().x - 0.1F, agent->getPosition().y - 0.1F);
                //         } while (agent->getPosition().distance(agv->getPosition()) < 0.5F);
                //     }
                // }

                agv->setDestination(route[route.size() - 1].x, route[route.size() - 1].y);
                agv->setDesiredSpeed(0.6F);
                agv->setAcceleration(inputData["acceleration"]["value"]);
                agv->setThresholdDisToPedes((float)inputData["thresDistance"]["value"]);
                for (int i = 1; i < route.size(); i++)
                {
                    agv->setPath(route[i].x, route[i].y, 1.0);
                }
                socialForce->addAGV(agv);
            }
        }
    }
    else
    {
        int numOfHallway = juncDataList.size();
        int numRunPerHallway = (int)inputData["noRunPerHallway"]["value"];
        int juncIndexTemp = 0;
        float hallwayLength = juncDataList[juncIndexTemp].items().begin().value();
        cout << "*****=> " << juncDataList[juncIndex].items().begin().key() << ": " << hallwayLength << endl;
        float length1Side = (hallwayLength) / 2;
        vector<float> juncDataTemp = {length1Side, length1Side};
        int numAGVPerRun = 1;
        if ((int)inputData["runConcurrently"]["value"] == 1)
        {
            numAGVPerRun = 2;
        }
        for (int i = 0; i < numOfHallway * numRunPerHallway; i++)
        {
            for (int j = 0; j < numAGVPerRun; j++)
            {
                agv = new AGV();
                vector<Point3f> route = Utility::getRouteAGV(j, 1, walkwayWidth, juncDataTemp); // Just need the source
                agv->setDirection(j, 1);
                agv->setPosition(route[0].x, route[0].y);

                agv->setDestination(route[route.size() - 1].x, route[route.size() - 1].y);
                agv->setDesiredSpeed((float)inputData["agvDesiredSpeed"]["value"]);
                agv->setAcceleration(inputData["acceleration"]["value"]);
                agv->setThresholdDisToPedes((float)inputData["thresDistance"]["value"]);
                for (int i = 1; i < route.size(); i++)
                {
                    agv->setPath(route[i].x, route[i].y, 1.0);
                }
                socialForce->addAGV(agv);

                int marker = numRunPerHallway * (juncIndexTemp + 1) - 1;
                if ((int)inputData["runConcurrently"]["value"] == 1)
                {
                    marker = numRunPerHallway * 2 * (juncIndexTemp + 1) - 1;
                }
                if (agv->getId() == marker)
                {
                    juncIndexTemp = juncIndexTemp + 1;
                    if (juncIndexTemp == juncDataList.size())
                    {
                        juncIndexTemp = 0;
                    }
                    hallwayLength = juncDataList[juncIndexTemp].items().begin().value();
                    length1Side = (hallwayLength) / 2;
                    juncDataTemp = {length1Side, length1Side};
                }
            }
        }
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffer
    glLoadIdentity();             // Initialize modelview matrix to identity matrix

    // Camera
    gluLookAt(0.0, 0.0, 18.0, // Position
              0.0, 0.0, 0.0,  // Look-at point
              0.0, 1.0, 0.0); // Up-vector

    glPushMatrix();
    glScalef(1.0, 1.0, 1.0);

    drawAgents(socialForce);
    createLabel();
    drawAGVs(socialForce, juncData, (int)inputData["runConcurrently"]["value"], (int)inputData["runMode"]["value"]);
    drawWalls(socialForce);
    glPopMatrix();

    showInformation(socialForce, fps, animate, currTime, startTime, classificationType, winWidth, winHeight);

    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // Initialize projection matrix to identity matrix
    gluPerspective(65.0, static_cast<GLfloat>(width) / height, 1.0,
                   100.0); // Create matrix for symmetric perspective-view frustum

    glMatrixMode(GL_MODELVIEW);

    winWidth = width;
    winHeight = height;
}

void normalKey(unsigned char key, int xMousePos, int yMousePos)
{
    switch (key)
    {
    case 'a': // Animate or inanimate scene
        animate = (!animate) ? true : false;
        startTime = currTime;
        break;

    case 27: // ASCII character for Esc key
        delete socialForce;
        socialForce = 0;

        exit(0); // Terminate program
        break;
    }
}

void update()
{
    int frameTime;       // Store time in milliseconds
    static int prevTime; // Stores time in milliseconds

    currTime = glutGet(GLUT_ELAPSED_TIME); // Get time in milliseconds since 'glutInit()' called
    frameTime = currTime - prevTime;
    prevTime = currTime;

    int count_agents = 0; //số lượng người đi bộ
    int count_agvs = 0; //số lượng AGV
    //Danh sách người đi bộ
    std::vector<Agent *> agents = socialForce->getCrowd();
    for (Agent *agent : agents)
    {
        Point3f src = agent->getPosition();
        Point3f des = agent->getDestination();

        if (Utility::isPositionErr(src, walkwayWidth, juncData.size(), socialForce->getAGVs()))
        {
            socialForce->removeAgent(agent->getId());
            continue;
        }

        if (agent->getVelocity().length() < LOWER_SPEED_LIMIT + 0.2 &&
            agent->getMinDistanceToWalls(socialForce->getWalls(), src, agent->getRadius()) < 0.2 &&
            (agent->interDes).size() == 0)
        {
            Point3f intermediateDes = Utility::getIntermediateDes(src, walkwayWidth, walkwayWidth);

            (agent->interDes).push_back(intermediateDes);
            agent->setPath(intermediateDes.x, intermediateDes.y, 1.0);
            agent->setPath(des.x, des.y, 1.0);
        }

        if ((agent->interDes).size() > 0)
        {
            float distanceToInterDes = src.distance((agent->interDes).front());
            if (distanceToInterDes <= 1)
            {
                (agent->interDes).clear();
            }
        }

        float distanceToTarget = src.distance(des);
        if (distanceToTarget <= 1 || isnan(distanceToTarget))
        {
            agent->setIsMoving(false);
            if (!agent->getStopAtCorridor())
            {
                socialForce->removeAgent(agent->getId());
            }
            count_agents = count_agents + 1;
        }
    }

    //Danh sách AGV
    std::vector<AGV *> agvs = socialForce->getAGVs();
    for (AGV *agv : agvs)
    {
        if (agv->getCollisionStartTime() == 0 && agv->getVelocity().length() < speedConsiderAsStop && agv->getIsMoving())
        {
            agv->setCollisionStartTime(glutGet(GLUT_ELAPSED_TIME));
            // cout << "- Start collision: " << convertTime(agv->getCollisionStartTime()) << endl;
        }

        if (agv->getCollisionStartTime() != 0 && agv->getVelocity().length() > speedConsiderAsStop && agv->getIsMoving())
        {
            agv->setTotalStopTime(agv->getTotalStopTime() + glutGet(GLUT_ELAPSED_TIME) - agv->getCollisionStartTime());
            // cout << "- Stop collision: " << convertTime(glutGet(GLUT_ELAPSED_TIME)) << endl;
            // cout << "=> Total collision: " << convertTime(agv->getTotalStopTime()) << endl;
            agv->setCollisionStartTime(0);
        }

        Point3f src = agv->getPosition();
        Point3f des = agv->getDestination();

        float distance = src.distance(des);
        if (distance <= 1 || isnan(distance))
        {
            if (agv->getIsMoving())
            {
                agv->setTravelingTime(glutGet(GLUT_ELAPSED_TIME) - agv->getTravelingTime());
                agv->setIsMoving(false);

                int numAGVCompleted = getNumAGVCompleted(socialForce->getAGVs());

                int marker = (int)inputData["noRunPerHallway"]["value"];
                if ((int)inputData["runConcurrently"]["value"] == 1)
                {
                    marker = (int)inputData["noRunPerHallway"]["value"] * 2;
                    if (numAGVCompleted % 2 == 0)
                    {
                        socialForce->removeCrowd();
                        createAgents();
                    }
                }
                else
                {
                    socialForce->removeCrowd();
                    createAgents();
                }

                if (numAGVCompleted > 0 && numAGVCompleted % marker == 0)
                {
                    juncIndex = juncIndex + 1;
                    if (juncIndex == juncDataList.size())
                    {
                        juncIndex = 0;
                    }
                    socialForce->removeWalls();
                    float hallwayLength = juncDataList[juncIndex].items().begin().value();
                    if (numAGVCompleted + 1 <= agvs.size())
                    {
                        cout << "*****=> " << juncDataList[juncIndex].items().begin().key() << ": " << hallwayLength << endl;
                    }
                    float length1Side = (hallwayLength) / 2;
                    juncData = {length1Side, length1Side};
                    createWalls_hospital(); // new 
                    // createWalls(); // old 
                    // cout << agv->getId() << " - Remove and re-create agent" << endl;
                }
            }
            count_agvs = count_agvs + 1;
        }
    }

    if (count_agvs == agvs.size())
    {
        int totalRunningTime = currTime - startTime;
        Utility::writeResult(
            "data/end.txt", juncName, inputData["graphicsMode"]["value"], agvs,
            juncDataList,
            (int)inputData["runConcurrently"]["value"],
            (int)inputData["runMode"]["value"],
            (int)inputData["noRunPerHallway"]["value"],
            totalRunningTime);

        std::cout << "Maximum speed: " << maxSpeed << " - Minimum speed: " << minSpeed << endl;
        std::cout << "Finish in: " << Utility::convertTime(totalRunningTime) << endl;
        delete socialForce;
        socialForce = 0;

        exit(0); // Terminate program
    }

    if (animate)
    {
        socialForce->moveCrowd(static_cast<float>(frameTime) / 1000); // Perform calculations and move agents
        socialForce->moveAGVs(static_cast<float>(frameTime) / 1000);
    }
    computeFPS(&fps);
    glutPostRedisplay();
    glutIdleFunc(update); // Continuously execute 'update()'
}
