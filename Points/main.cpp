
#include "pointscontroller.h"

int main(int argc, char *argv[])
{
    std::string pointsFilePath("Points.csv");
    std::string planeFilePath("PlanePoints.csv");
    PointsController controller;
    controller.readPoints(pointsFilePath);
    controller.readPlanePoints(planeFilePath);
    controller.computePlane();
    controller.computeProjections();

    return 0;
}
