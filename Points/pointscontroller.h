#ifndef _POINTS_CONTROLLER_H_
#define _POINTS_CONTROLLER_H_

#include "vector.h"
#include <vector>
#include <string>

class PointsController
{
    static double sZero;

public:
    PointsController();
    ~PointsController();

    bool readPoints(const std::string &filePath);
    bool readPlanePoints(const std::string &filePath);
    bool computePlane();
    void computeProjections();

private:
    std::vector<Vector> m_points;
    Vector m_plane[3];
    Vector m_planeNormal;
    std::vector<Vector> m_projections;
};

#endif
