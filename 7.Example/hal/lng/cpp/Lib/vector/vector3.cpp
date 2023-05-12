#include "vector3.hpp"

vector3::vector3(float x, float y, float z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

vector3::~vector3()
{
}

float vector3::x(void) const { return m_x; }
float vector3::y(void) const { return m_y; }
float vector3::z(void) const { return m_z; }

vector3 vector3::operator+(const vector3& other) const
{
    return vector3(m_x + other.x(), m_y + other.y(), m_z + other.z());
}
vector3 vector3::operator-(const vector3& other) const
{
    return vector3(m_x - other.x(), m_y - other.y(), m_z - other.z());
}

void vector3::print(void)
{
    // std::cout << m_x << m_y << m_z << std::endl;
}