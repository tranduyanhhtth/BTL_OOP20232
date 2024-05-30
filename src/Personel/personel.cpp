#include "src/Personel/personel.h"

Personal::Personal()
{
    json inputData = Utility::readInputData("data/input.json");
    int numDepartment = inputData["journeyDistribution"]["forPersonel"]["value"];
    
}