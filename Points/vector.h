#ifndef _VECTOR_H_
#define _VECTOR_H_

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

    void setX(const double &x);
    void setY(const double &y);
    void setZ(const double &z);

    ///
    /// \brief Vector algebra operators
    ///
    Vector operator+(const Vector &v) const;
    Vector operator-(const Vector &v) const;
    friend Vector operator*(const double& a, const Vector& v);
    Vector operator*(const Vector& v) const;


private:
    double m_x;
    double m_y;
    double m_z;
};

#endif
