#include "pointscontroller.h"

#include <fstream>
#include <cmath>

#include <vtkPlaneSource.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkProperty.h>
#include <vtkAxesActor.h>
#include <vtkCaptionActor2D.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkLine.h>
#include <vtkCamera.h>

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
        vp = v - m_planeNormal.dot(v0) * m_planeNormal;
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

void PointsController::showVTKScene()
{
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> interactorStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    renderWindowInteractor->SetInteractorStyle(interactorStyle);
    renderer->SetBackground(0.1, 0.2, 0.3);

    buildVTKScene(renderer);

    vtkCamera* camera = renderer->GetActiveCamera();
    camera->SetPosition(150.0, 150.0, 150.0);
    camera->SetFocalPoint(0.0, 0.0, 0.0);

    renderWindow->SetSize(1280, 1024);
    renderWindow->Render();
    renderWindowInteractor->Start();
}

void PointsController::buildVTKScene(vtkSmartPointer<vtkRenderer> renderer)
{
    //axes
    vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
    axes->SetTotalLength(100.0, 100.0, 100.0);
    axes->SetConeRadius(0.1);
    axes->SetAxisLabels(0);
    axes->GetXAxisCaptionActor2D()->GetTextActor()->SetTextScaleMode(vtkTextActor::TEXT_SCALE_MODE_NONE);
    axes->GetXAxisCaptionActor2D()->GetTextActor()->GetTextProperty()->SetFontSize(12);
    axes->GetYAxisCaptionActor2D()->GetTextActor()->SetTextScaleMode(vtkTextActor::TEXT_SCALE_MODE_NONE);
    axes->GetYAxisCaptionActor2D()->GetTextActor()->GetTextProperty()->SetFontSize(12);
    axes->GetZAxisCaptionActor2D()->GetTextActor()->SetTextScaleMode(vtkTextActor::TEXT_SCALE_MODE_NONE);
    axes->GetZAxisCaptionActor2D()->GetTextActor()->GetTextProperty()->SetFontSize(12);
    renderer->AddActor(axes);

    //plane
    vtkSmartPointer<vtkPlaneSource> planeSource = vtkSmartPointer<vtkPlaneSource>::New();
    //planeSource->SetCenter(m_plane[0].x(), m_plane[0].y(), m_plane[0].z());
    //planeSource->SetNormal(m_planeNormal.x(), m_planeNormal.y(), m_planeNormal.z());

    double planeSize = 1000.0;
    Vector axis1 = m_plane[1] - m_plane[0];
    axis1 = (1.0 / sqrt(axis1.dot(axis1))) * axis1;
    Vector axis2 = m_planeNormal * axis1;
    Vector origin = m_plane[0] - planeSize * axis1 - planeSize * axis2;
    Vector point1 = origin + 2.0 * planeSize * axis1;
    Vector point2 = origin + 2.0 * planeSize * axis2;

    planeSource->SetOrigin(origin.x(), origin.y(), origin.z());
    planeSource->SetPoint1(point1.x(), point1.y(), point1.z());
    planeSource->SetPoint2(point2.x(), point2.y(), point2.z());

    planeSource->SetResolution(200, 200);

    planeSource->Update();

    vtkPolyData* plane = planeSource->GetOutput();
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(plane);
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetRepresentationToWireframe();
    renderer->AddActor(actor);

    //lines
    vtkSmartPointer<vtkPoints> vtkpoints = vtkSmartPointer<vtkPoints>::New();
    for(size_t i = 0; i < m_points.size(); ++i)
    {
        vtkpoints->InsertNextPoint(m_points[i].x(), m_points[i].y(), m_points[i].z());
        vtkpoints->InsertNextPoint(m_projections[i].x(), m_projections[i].y(), m_projections[i].z());
    }
    vtkSmartPointer<vtkCellArray> cellarray = vtkSmartPointer<vtkCellArray>::New();
    int count = vtkpoints->GetNumberOfPoints() / 2;
    for(int i = 0; i < count - 1; ++i)
    {
        vtkSmartPointer<vtkLine> vtkline = vtkSmartPointer<vtkLine>::New();
        vtkline->GetPointIds()->SetId(0, 2 * i);
        vtkline->GetPointIds()->SetId(1, 2 * i + 1);
        cellarray->InsertNextCell(vtkline);
    }
    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints(vtkpoints);
    polydata->SetLines(cellarray);
    vtkSmartPointer<vtkPolyDataMapper> segMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    segMapper->SetInputData(polydata);
    vtkSmartPointer<vtkActor> pathActor = vtkSmartPointer<vtkActor>::New();
    pathActor->SetMapper(segMapper);
    renderer->AddActor(pathActor);

}
