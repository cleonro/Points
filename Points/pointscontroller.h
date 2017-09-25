#ifndef _POINTS_CONTROLLER_H_
#define _POINTS_CONTROLLER_H_

#include "vector.h"
#include <vector>
#include <string>

class PointsController
{
public:
    PointsController();
    ~PointsController();

    bool readPoints(const std::string &filePath);
    bool readPlanePoints(const std::string &filePath);

private:
    std::vector<Vector> m_points;
    Vector m_plane[3];

};

#endif
