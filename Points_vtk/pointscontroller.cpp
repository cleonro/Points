#include "pointscontroller.h"

#include <fstream>
#include <cmath>

double PointsController::sZero = 0.0000001;

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

bool PointsController::computePlane()
{
    m_planeNormal = (m_plane[1] - m_plane[0]) * (m_plane[2] - m_plane[0]);
    double norm = sqrt(m_planeNormal.dot(m_planeNormal));
    if(norm < sZero)
    {
        return false;
    }

    m_planeNormal = (1.0 / norm) * m_planeNormal;

    std::cout << "Computed plane parameters!" << std::endl;
    return true;
}

void PointsController::computeProjections()
{
    m_projections.clear();
    Vector vp;
    Vector v;
    for(size_t i = 0; i < m_points.size(); ++i)
    {
        v = m_points[i];
        Vector v0 = v - m_plane[0];
        vp = v - m_planeNormal.dot(v0) * v0;
        m_projections.push_back(vp);
        //just for testing
        if(m_planeNormal.dot(vp - m_plane[0]) > sZero)
        {
            std::cout << "Error computing projections!" << std::endl;
            break;
        }
    }
    std::cout << "Computed " << m_projections.size() << " projected points!" << std::endl;
}

void PointsController::sortProjections()
{
    m_sortedProjections.clear();
    if(m_projections.size() == 0 || m_projections.size() != m_points.size())
    {
        return;
    }
    Vector v;
    double dist = 0.0;
    for(size_t i = 0; i < m_projections.size(); ++i)
    {
        v = m_points[i] - m_projections[i];
        dist = sqrt(v.dot(v));
        m_sortedProjections[dist] = m_projections[i];
    }

    //just for testing
    for(std::map<double, Vector>::iterator it = m_sortedProjections.begin(); it != m_sortedProjections.end(); ++it)
    {
        std::cout << it->second << ", " << it->first << std::endl;
    }

    std::cout << "Sorted " << m_sortedProjections.size() << " projected points!" << std::endl;
}

bool PointsController::writeProjections(const std::string &filePath)
{
    std::filebuf cacheFile;
    if(!cacheFile.open(filePath, std::ios::out))
    {
        return false;
    }
    std::ostream cache(&cacheFile);

    for(size_t i = 0; i < m_projections.size(); ++i)
    {
        cache << m_projections[i] << std::endl;
        if(!cache)
        {
            return false;
        }
    }

    return true;
}

bool PointsController::writeSortedProjections(const std::string &filePath)
{
    std::filebuf cacheFile;
    if(!cacheFile.open(filePath, std::ios::out))
    {
        return false;
    }
    std::ostream cache(&cacheFile);

    for(std::map<double, Vector>::iterator it = m_sortedProjections.begin(); it != m_sortedProjections.end(); ++it)
    {
        cache << it->second << ", " << it->first << std::endl;
        if(!cache)
        {
            return false;
        }
    }

    return true;
}
