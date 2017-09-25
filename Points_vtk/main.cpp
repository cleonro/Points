
#include "pointscontroller.h"

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
    if(argc < 5)
    {
        std::cout << "Not enough parameters!" << std::endl;
        return 1;
    }
    std::string pointsFilePath = argv[1];//("Points.csv");
    std::string planeFilePath = argv[2];//("PlanePoints.csv");
    std::string projectionsFilePath = argv[3];//("Projections.csv");
    std::string sortedProjectionsFilePath = argv[4];//("SortedProjections.csv");

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
    }

    return 0;
}
