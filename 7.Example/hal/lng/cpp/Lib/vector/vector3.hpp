#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

class vector3 {
private:
    float m_x, m_y, m_z;

public:
    vector3(float x, float y, float z);
    ~vector3();

    float x(void) const;
    float y(void) const;
    float z(void) const;

    vector3 operator+(const vector3& other) const;
    vector3 operator-(const vector3& other) const;
    // vector3 operator*(const vector3& other) const;
    // vector3 operator/(const vector3& other) const;

    void print(void);
};

template <typename T>
T sum(T x, T y, T z)
{
    return x + y + z;
}

#endif
