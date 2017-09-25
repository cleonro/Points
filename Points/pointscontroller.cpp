#include "pointscontroller.h"

#include <fstream>

PointsController::PointsController()
{

}

PointsController::~PointsController()
{

}

bool PointsController::readPoints(const std::string &filePath)
{
    std::filebuf cacheFile;
    if(!cacheFile.open(filePath, std::ios::in))
    {
        return false;
    }
    std::istream cache(&cacheFile);
    m_points.clear();
    Vector v;
    while(cache && !v.readError())
    {
        cache >> v;
        if(!v.readError())
        {
            m_points.push_back(v);
        }
    }
    std::cout << "Read " << m_points.size() << " points!" << std::endl;
    return true;
}

bool PointsController::readPlanePoints(const std::string &filePath)
{
    std::filebuf cacheFile;
    if(!cacheFile.open(filePath, std::ios::in))
    {
        return false;
    }
    std::istream cache(&cacheFile);
    Vector v;
    for(int i = 0; i < 3; ++i)
    {
        if(cache && !v.readError())
        {
            cache >> v;
            m_plane[i] = v;
        }
        else
        {
            return false;
        }
    }
    std::cout << "Read plane points!" << std::endl;
    return true;
}
