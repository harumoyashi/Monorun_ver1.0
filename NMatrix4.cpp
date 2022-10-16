#include "NMatrix4.h"
#include <cmath>

NMatrix4 Identity()
{
    static const NMatrix4 result
    {
        1.0f,0.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,0.0f,
        0.0f,0.0f,1.0f,0.0f,
        0.0f,0.0f,0.0f,1.0f
    };
    return result;
}

NMatrix4 Scale(const NVector3& s)
{
    NMatrix4 result
    {
        s.x,0,0,0,
        0,s.y,0,0,
        0,0,s.z,0,
        0,0,0,1
    };
    return result;
}

NMatrix4 RotateX(float angle)
{
    float sin = std::sin(angle);
    float cos = std::cos(angle);

    NMatrix4 result
    {
        1.0f,0.0f,0.0f,0.0f,
        0.0f,cos,sin,0.0f,
        0.0f,-sin,cos,0.0f,
        0.0f,0.0f,0.0f,1.0f
    };
    return result;
}

NMatrix4 RotateY(float angle)
{
    float sin = std::sin(angle);
    float cos = std::cos(angle);

    NMatrix4 result
    {
        cos,0.0f,-sin,0.0f,
        0.0f,1.0f,0.0f,0.0f,
        sin,0.0f,cos,0.0f,
        0.0f,0.0f,0.0f,1.0f
    };
    return result;
}

NMatrix4 RotateZ(float angle)
{
    float sin = std::sin(angle);
    float cos = std::cos(angle);

    NMatrix4 result
    {
        cos,sin,0.0f,0.0f,
        -sin,cos,0.0f,0.0f,
        0.0f,0.0f,1.0f,0.0f,
        0.0f,0.0f,0.0f,1.0f
    };
    return result;
}

NMatrix4 Translation(const NVector3& t)
{
    NMatrix4 result
    {
        1.0f,0.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,0.0f,
        0.0f,0.0f,1.0f,0.0f,
        t.x,t.y,t.z,1.0f
    };
    return result;
}

NVector3 Transform(const NVector3& v, const NMatrix4& m)
{
    float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

    NVector3 result
    {
        (v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0] * 0.0f) / w,
        (v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1] * 0.0f) / w,
        (v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2] * 0.0f) / w
    };

    return result;
}

NVector3 WTransform(const NVector3& v, const NMatrix4& m)
{
    float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

    NVector3 result
    {
        (v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w,
        (v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w,
        (v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w
    };

    return result;
}

NMatrix4& operator*=(NMatrix4& m1, const NMatrix4& m2)
{
    NMatrix4 result{ 0 };

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                result.m[i][j] += m1.m[i][k] * m2.m[k][j];
            }
        }
    }
    m1 = result;
    return m1;
}

const NMatrix4 operator*(const NMatrix4& m1, const NMatrix4& m2)
{
    NMatrix4 result = m1;

    return result *= m2;
}

const NVector3 operator*(const NVector3& v, const NMatrix4& m)
{
    return Transform(v, m);
}