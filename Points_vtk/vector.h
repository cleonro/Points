#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <iostream>

class Vector
{
public:
    ///
    /// \brief Constructors, equality operator, destructor
    ///
    Vector();
    Vector(const double &x, const double &y, const double &z);
    Vector(const Vector &v);
    Vector& operator=(const Vector &v);
    ~Vector();

    ///
    /// \brief Access functions
    ///
    double x() const;
    double y() const;
    double z() const;
    bool readError();

    void setX(const double &x);
    void setY(const double &y);
    void setZ(const double &z);

    ///
    /// \brief Vector algebra operators
    ///
    Vector operator+(const Vector &v) const;
    Vector operator-(const Vector &v) const;
    friend Vector operator*(const double &a, const Vector &v);
    Vector operator*(const Vector &v) const;
    double dot(const Vector &v);

    ///
    /// \brief stream operators
    ///
    friend std::istream& operator>>(std::istream &s, Vector& v);
    friend std::ostream& operator<<(std::ostream &s, Vector& v);

private:
    double m_x;
    double m_y;
    double m_z;
    bool m_readError;
};

#endif
