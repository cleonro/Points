
#include "pointscontroller.h"

int main(int argc, char *argv[])
{
    std::string pointsFilePath("Points.csv");
    std::string planeFilePath("PlanePoints.csv");
    PointsController controller;
    controller.readPoints(pointsFilePath);
    controller.readPlanePoints(planeFilePath);

    return 0;
}
