
#include "pointscontroller.h"

#include <cmath>
#include <fstream>
#include <iostream>

void generate()
{
    int n = 20;
    double h = 10;
    double t = 2.0 * M_PI / 50.0;
    double R = 40.0;

    std::filebuf planeFileBuf;
    planeFileBuf.open("PlanePoints0.csv", std::ios::out);
    std::ostream planeFile(&planeFileBuf);
    planeFile << Vector(0, 0, 0) << std::endl;
    planeFile << Vector(10, 0, 0) << std::endl;
    planeFile << Vector(0, 10, 0) << std::endl;
    planeFileBuf.close();

    std::filebuf pointsFileBuf;
    pointsFileBuf.open("Points0.csv", std::ios::out);
    std::ostream pointsFile(&pointsFileBuf);
    for(int i = 0; i < n; ++i)
    {
        double x = R * cos(t * i);
        double y = R * sin(t * i);
        double z = h * i;
        pointsFile << Vector(x, y, z) << std::endl;
    }
    pointsFileBuf.close();
}

///
/// \brief The program requires four parameters:
///         file path for the points
///         file path for the plane points
///         output file path for the projections
///         output file path for the sorted projections
/// \param argc
/// \param argv
/// \return
///
int main(int argc, char *argv[])
{
    std::string pointsFilePath;
    std::string planeFilePath;
    std::string projectionsFilePath;
    std::string sortedProjectionsFilePath;
    if(argc < 5)
    {
        generate();
        pointsFilePath = "Points0.csv";
        planeFilePath = "PlanePoints0.csv";
        projectionsFilePath = "Projections0.csv";
        sortedProjectionsFilePath = "SortedProjections0.csv";
    }
    else
    {
        pointsFilePath = argv[1];
        planeFilePath = argv[2];
        projectionsFilePath = argv[3];
        sortedProjectionsFilePath = argv[4];
    }

    PointsController controller;
    if(!controller.readPoints(pointsFilePath))
    {
        std::cout << "Points file was not read!" << std::endl;
        return 2;
    }
    if(!controller.readPlanePoints(planeFilePath))
    {
        std::cout << "Plane file was not read!" << std::endl;
        return 3;
    }
    if(!controller.computePlane())
    {
        std::cout << "Plane parameters were not computed!" << std::endl;
        return 4;
    }
    controller.computeProjections();
    controller.sortProjections();
    if(!controller.writeProjections(projectionsFilePath))
    {
        std::cout << "Projections file was not written!" << std::endl;
        return 5;
    }
    if(!controller.writeSortedProjections(sortedProjectionsFilePath))
    {
        std::cout << "Sorted projections file was not written!" << std::endl;
        return 6;
    }

    controller.showVTKScene();

    return 0;
}
