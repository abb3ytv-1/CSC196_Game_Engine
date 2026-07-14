#pragma once

#include <cassert>
#include <cmath>

namespace nu {
    struct Vector3 {
        union {
            struct {
                float x;
                float y;
                float z;
            };

            struct {
                float r;
                float g;
                float b;
            };

            float data[3];
        };

        // Constructors
        Vector3()
            : x{ 0.0f }, y{ 0.0f }, z{ 0.0f } {}

        Vector3(float x, float y, float z)
            : x{ x }, y{ y }, z{ z } {}

        Vector3(float xyz)
            : x{ xyz }, y{ xyz }, z{ xyz } {}

        // Index operators
        const float& operator[](unsigned int i) const {
            assert(i < 3);
            return data[i];
        }

        float& operator[](unsigned int i) {
            assert(i < 3);
            return data[i];
        }

        // Comparison operators
        bool operator==(const Vector3& v) const {
            return x == v.x &&
                y == v.y &&
                z == v.z;
        }

        bool operator!=(const Vector3& v) const {
            return !(*this == v);
        }

        // Vector operators
        Vector3 operator+(const Vector3& v) const {
            return Vector3(
                x + v.x,
                y + v.y,
                z + v.z
            );
        }

        Vector3 operator-(const Vector3& v) const {
            return Vector3(
                x - v.x,
                y - v.y,
                z - v.z
            );
        }

        Vector3 operator*(const Vector3& v) const {
            return Vector3(
                x * v.x,
                y * v.y,
                z * v.z
            );
        }

        Vector3 operator/(const Vector3& v) const {
            return Vector3(
                x / v.x,
                y / v.y,
                z / v.z
            );
        }

        // Scalar operators
        Vector3 operator+(float v) const {
            return Vector3(x + v, y + v, z + v);
        }

        Vector3 operator-(float v) const {
            return Vector3(x - v, y - v, z - v);
        }

        Vector3 operator*(float v) const {
            return Vector3(x * v, y * v, z * v);
        }

        Vector3 operator/(float v) const {
            return Vector3(x / v, y / v, z / v);
        }

        // Vector compound operators
        Vector3& operator+=(const Vector3& v) {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        Vector3& operator-=(const Vector3& v) {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        Vector3& operator*=(const Vector3& v) {
            x *= v.x;
            y *= v.y;
            z *= v.z;
            return *this;
        }

        Vector3& operator/=(const Vector3& v) {
            x /= v.x;
            y /= v.y;
            z /= v.z;
            return *this;
        }

        // Scalar compound operators
        Vector3& operator+=(float v) {
            x += v;
            y += v;
            z += v;
            return *this;
        }

        Vector3& operator-=(float v) {
            x -= v;
            y -= v;
            z -= v;
            return *this;
        }

        Vector3& operator*=(float v) {
            x *= v;
            y *= v;
            z *= v;
            return *this;
        }

        Vector3& operator/=(float v) {
            x /= v;
            y /= v;
            z /= v;
            return *this;
        }

        // Vector functions
        float LengthSqr() const {
            return (x * x) + (y * y) + (z * z);
        }

        float Length() const {
            return std::sqrt(LengthSqr());
        }

        Vector3 Normalized() const {
            const float length = Length();

            if (length == 0.0f) {
                return Vector3{};
            }

            return (*this) / length;
        }
    };

    // A Color has the same three-component structure as Vector3
    using Color = Vector3;
}