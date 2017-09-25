#include "vector.h"

Vector::Vector()
{
    m_x = 0.0;
    m_y = 0.0;
    m_z = 0.0;
}

Vector::Vector(const double &x, const double &y, const double &z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

Vector::Vector(const Vector &v)
{
    m_x = v.m_x;
    m_y = v.m_y;
    m_z = v.m_z;
}

Vector& Vector::operator=(const Vector &v)
{
    m_x = v.m_x;
    m_y = v.m_y;
    m_z = v.m_z;
    return *this;
}

Vector::~Vector()
{

}

double Vector::x() const
{
    return m_x;
}

double Vector::y() const
{
    return m_y;
}

double Vector::z() const
{
    return m_z;
}

void Vector::setX(const double &x)
{
    m_x = x;
}

void Vector::setY(const double &y)
{
    m_y = y;
}

void Vector::setZ(const double &z)
{
    m_z = z;
}

Vector Vector::operator+(const Vector &v) const
{
    Vector r(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z);
    return r;
}

Vector Vector::operator-(const Vector &v) const
{
    Vector r(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z);
    return r;
}

Vector operator*(const double &a, const Vector &v)
{
    Vector r(a * v.m_x, a * v.m_y, a * v.m_z);
    return r;
}

Vector Vector::operator*(const Vector& v) const
{
    Vector r(
                m_y * v.m_z - m_z * v.m_y,
                m_z * v.m_x - m_x * v.m_z,
                m_x * v.m_y - m_y * v.m_x
            );
    return r;
}
