//
// Everything here has been adapted from https://github.com/nsf/mc
// A public domain marching cube implementation
//

#include <cstdio>
#include <cstdint>
#include <vector>

#include <iostream>
#include <fstream>

#include "ttree.h"

#include <cmath>
#include <cstdlib>
#include <cstdint>


//------------------------------------------------------------------------------
// Utility functions
//------------------------------------------------------------------------------

template <typename T>
static inline T lerp(const T &a, const T &b, float v)
{
	return a * (1 - v) + b * v;
}

template <typename T>
static inline T clamp(const T &value, const T &min, const T &max)
{
	if (value > max)
		return max;
	if (value < min)
		return min;
	return value;
}

template <typename T>
static inline const T &max(const T &v1, const T &v2)
{
	return (v1 > v2) ? v1 : v2;
}

// returns 0 if v1 > v2 or 1 otherwise
template <typename T>
static inline int max_i(const T &v1, const T &v2)
{
	return (v1 > v2) ? 0 : 1;
}

template <typename T>
static inline const T &min(const T &v1, const T &v2)
{
	return (v1 < v2) ? v1 : v2;
}

// returns 0 if v1 < v2 or 1 otherwise
template <typename T>
static inline int min_i(const T &v1, const T &v2)
{
	return (v1 < v2) ? 0 : 1;
}

template <typename T>
static inline const T &min3(const T &v1, const T &v2, const T &v3)
{
	return min(v1, min(v2, v3));
}

template <typename T>
static inline const T &max3(const T &v1, const T &v2, const T &v3)
{
	return max(v1, max(v2, v3));
}

template <typename T>
static inline int min3_i(const T &v1, const T &v2, const T &v3)
{
	const T *vs[] = { &v1, &v2, &v3 };
	int min = 0;
	for (int i = 1; i < 3; i++) {
		if (*vs[i] < *vs[min])
			min = i;
	}
	return min;
}

template <typename T>
static inline int max3_i(const T &v1, const T &v2, const T &v3)
{
	const T *vs[] = { &v1, &v2, &v3 };
	int max = 0;
	for (int i = 1; i < 3; i++) {
		if (*vs[i] > *vs[max])
			max = i;
	}
	return max;
}

static inline int next_power_of_2(int v)
{
	v -= 1;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	return v + 1;
}

static inline int floor_div(int a, int b)
{
	int q = a / b;
	int r = a % b;
	if (r != 0 && ((r < 0) != (b < 0))) q--;
	return q;
}

//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------

const float MATH_PI = 3.14159265359f;
const float MATH_2PI = MATH_PI * 2.0f;
const float MATH_HALF_PI = MATH_PI / 2.0f;
const float MATH_DEG_TO_RAD = MATH_PI / 180.0f;
const float MATH_RAD_TO_DEG = 180.0f / MATH_PI;
const float MATH_EPSILON = 1e-6f;

//------------------------------------------------------------------------------
// Vec2
//------------------------------------------------------------------------------
#define _DEFINE_VEC2_NO_FUNCTIONS(type, Vec2)
#define _DEFINE_VEC2_NO_MEMBERS(type, Vec2)


#define _DEFINE_VEC2_INT_MEMBERS(type, Vec2)                          \
	Vec2 &operator|=(const Vec2 &r) { x|=r.x; y|=r.y; return *this; } \
	Vec2 &operator&=(const Vec2 &r) { x&=r.x; y&=r.y; return *this; } \
	Vec2 &operator^=(const Vec2 &r) { x^=r.x; y^=r.y; return *this; } \
	Vec2 &operator%=(const Vec2 &r) { x%=r.x; y%=r.y; return *this; } \


#define _DEFINE_VEC2_INT_FUNCTIONS(type, Vec2)                                                                        \
static inline Vec2 floor_div(const Vec2 &a, const Vec2 &b) { return Vec2(floor_div(a.x, b.x), floor_div(a.y, b.y)); } \
static inline Vec2 operator&(const Vec2 &l, const Vec2 &r) { return Vec2(l.x & r.x, l.y & r.y); }                     \
static inline Vec2 operator|(const Vec2 &l, const Vec2 &r) { return Vec2(l.x | r.x, l.y | r.y); }                     \
static inline Vec2 operator^(const Vec2 &l, const Vec2 &r) { return Vec2(l.x ^ r.x, l.y ^ r.y); }                     \
static inline Vec2 operator%(const Vec2 &l, const Vec2 &r) { return Vec2(l.x % r.x, l.y % r.y); }                     \


#define _DEFINE_VEC2_FLOAT_FUNCTIONS(type, Vec2)                                      \
static inline type length(const Vec2 &v) { return std::sqrt(length2(v)); }            \
static inline Vec2 normalize(const Vec2 &v) { return v / Vec2(length(v)); }           \
static inline type distance(const Vec2 &v1, const Vec2 &v2) { return length(v2-v1); } \


#define _DEFINE_VEC2(type, Vec2, ADDITIONAL_MEMBERS, ADDITIONAL_FUNCTIONS)                            \
struct Vec2 {                                                                                         \
	union {                                                                                           \
		struct {                                                                                      \
			type x, y;                                                                                \
		};                                                                                            \
		type data[2];                                                                                 \
	};                                                                                                \
                                                                                                      \
	Vec2() = default;                                                                                 \
	Vec2(type ax, type ay): x(ax), y(ay) {}                                                           \
	explicit Vec2(type v): x(v), y(v) {}                                                              \
                                                                                                      \
	Vec2 &operator+=(const Vec2 &r) { x+=r.x; y+=r.y; return *this; }                                 \
	Vec2 &operator-=(const Vec2 &r) { x-=r.x; y-=r.y; return *this; }                                 \
	Vec2 &operator*=(const Vec2 &r) { x*=r.x; y*=r.y; return *this; }                                 \
	Vec2 &operator/=(const Vec2 &r) { x/=r.x; y/=r.y; return *this; }                                 \
	ADDITIONAL_MEMBERS(type, Vec2)                                                                    \
                                                                                                      \
	type &operator[](int i) { return data[i]; }                                                       \
	type operator[](int i) const { return data[i]; }                                                  \
};                                                                                                    \
                                                                                                      \
static inline Vec2 Vec2##_X(type v = 1) { return {v, 0}; }                                            \
static inline Vec2 Vec2##_Y(type v = 1) { return {0, v}; }                                            \
                                                                                                      \
static inline bool operator==(const Vec2 &l, const Vec2 &r) { return l.x == r.x && l.y == r.y; }      \
static inline bool operator!=(const Vec2 &l, const Vec2 &r) { return l.x != r.x || l.y != r.y; }      \
static inline bool operator<(const Vec2 &l, const Vec2 &r)  { return l.x < r.x && l.y < r.y; }        \
static inline bool operator>(const Vec2 &l, const Vec2 &r)  { return l.x > r.x && l.y > r.y; }        \
static inline bool operator<=(const Vec2 &l, const Vec2 &r) { return l.x <= r.x && l.y <= r.y; }      \
static inline bool operator>=(const Vec2 &l, const Vec2 &r) { return l.x >= r.x && l.y >= r.y; }      \
                                                                                                      \
static inline Vec2 operator-(const Vec2 &v) { return Vec2(-v.x, -v.y); }                              \
static inline Vec2 operator+(const Vec2 &l, const Vec2 &r) { return Vec2(l.x + r.x, l.y + r.y); }     \
static inline Vec2 operator-(const Vec2 &l, const Vec2 &r) { return Vec2(l.x - r.x, l.y - r.y); }     \
static inline Vec2 operator*(const Vec2 &l, const Vec2 &r) { return Vec2(l.x * r.x, l.y * r.y); }     \
static inline Vec2 operator/(const Vec2 &l, const Vec2 &r) { return Vec2(l.x / r.x, l.y / r.y); }     \
                                                                                                      \
static inline type area(const Vec2 &v) { return v.x * v.y; }                                          \
static inline type length2(const Vec2 &v) { return v.x*v.x + v.y*v.y; }                               \
static inline type dot(const Vec2 &v1, const Vec2 &v2) { return v1.x*v2.x + v1.y*v2.y; }              \
static inline type distance2(const Vec2 &v1, const Vec2 &v2) { return length2(v2-v1); }               \
static inline Vec2 min(const Vec2 &v1, const Vec2 &v2) { return {min(v1.x, v2.x), min(v1.y, v2.y)}; } \
static inline Vec2 max(const Vec2 &v1, const Vec2 &v2) { return {max(v1.x, v2.x), max(v1.y, v2.y)}; } \
ADDITIONAL_FUNCTIONS(type, Vec2)                                                                      \


_DEFINE_VEC2(float, Vec2f, _DEFINE_VEC2_NO_MEMBERS, _DEFINE_VEC2_FLOAT_FUNCTIONS)
_DEFINE_VEC2(double, Vec2d, _DEFINE_VEC2_NO_MEMBERS, _DEFINE_VEC2_FLOAT_FUNCTIONS)
_DEFINE_VEC2(int32_t, Vec2i, _DEFINE_VEC2_INT_MEMBERS, _DEFINE_VEC2_INT_FUNCTIONS)
_DEFINE_VEC2(int16_t, Vec2s, _DEFINE_VEC2_INT_MEMBERS, _DEFINE_VEC2_INT_FUNCTIONS)
_DEFINE_VEC2(int8_t, Vec2b, _DEFINE_VEC2_INT_MEMBERS, _DEFINE_VEC2_INT_FUNCTIONS)

_DEFINE_VEC2(uint16_t, Vec2us, _DEFINE_VEC2_INT_MEMBERS, _DEFINE_VEC2_INT_FUNCTIONS)

static inline Vec2f ToVec2f(const Vec2i &v) { return Vec2f(float(v.x), float(v.y)); }
static inline Vec2i ToVec2i(const Vec2f &v) { return Vec2i(int(v.x), int(v.y)); }

//------------------------------------------------------------------------------
// Vec3
//------------------------------------------------------------------------------

#define _DEFINE_VEC3_NO_FUNCTIONS(type, Vec3)
#define _DEFINE_VEC3_NO_MEMBERS(type, Vec3)


#define _DEFINE_VEC3_INT_MEMBERS(type, Vec3)                                  \
	Vec3 &operator&=(const Vec3 &r) { x&=r.x; y&=r.y; z&=r.z; return *this; } \
	Vec3 &operator|=(const Vec3 &r) { x|=r.x; y|=r.y; z|=r.z; return *this; } \
	Vec3 &operator^=(const Vec3 &r) { x^=r.x; y^=r.y; z^=r.z; return *this; } \
	Vec3 &operator%=(const Vec3 &r) { x%=r.x; y%=r.y; z%=r.z; return *this; } \


#define _DEFINE_VEC3_INT_FUNCTIONS(type, Vec3)                                                                                             \
static inline Vec3 floor_div(const Vec3 &a, const Vec3 &b) { return Vec3(floor_div(a.x, b.x), floor_div(a.y, b.y), floor_div(a.z, b.z)); } \
static inline Vec3 operator^(const Vec3 &l, const Vec3 &r) { return Vec3(l.x ^ r.x, l.y ^ r.y, l.z ^ r.z); }                               \
static inline Vec3 operator%(const Vec3 &l, const Vec3 &r) { return Vec3(l.x % r.x, l.y % r.y, l.z % r.z); }                               \
static inline Vec3 operator&(const Vec3 &l, const Vec3 &r) { return Vec3(l.x & r.x, l.y & r.y, l.z & r.z); }                               \
static inline Vec3 operator|(const Vec3 &l, const Vec3 &r) { return Vec3(l.x | r.x, l.y | r.y, l.z | r.z); }                               \
static inline Vec3 operator~(const Vec3 &v) { return Vec3(~v.x, ~v.y, ~v.z); }                                                             \


#define _DEFINE_VEC3_FLOAT_FUNCTIONS(type, Vec3)                                                                                          \
static inline Vec3 abs(const Vec3 &v) { return Vec3(std::abs(v.x), std::abs(v.y), std::abs(v.z)); }                                       \
static inline type length(const Vec3 &v) { return std::sqrt(length2(v)); }                                                                \
static inline Vec3 normalize(const Vec3 &v) { type vv = length(v); if (vv == 0) { return v; } else { return v / Vec3(vv); } }			  \
static inline bool is_nan(const Vec3 &v) { return std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z); }                                \
static inline type distance(const Vec3 &v1, const Vec3 &v2) { return length(v2-v1); }                                                     \
static inline Vec3 lerp(const Vec3 &a, const Vec3 &b, float v) { return a * Vec3(1 - v) + b * Vec3(v); }                                  \
static inline Vec3 mod(const Vec3 &a, const Vec3 &b) { return Vec3(std::fmod(a.x, b.x), std::fmod(a.y, b.y), std::fmod(a.z, b.z)); }      \
static inline Vec3 pow(const Vec3 &v1, const Vec3 &v2) { return Vec3(std::pow(v1.x, v2.x), std::pow(v1.y, v2.y), std::pow(v1.z, v2.z)); } \


#define _DEFINE_VEC3(type, Vec3, Vec2, ADDITIONAL_MEMBERS, ADDITIONAL_FUNCTIONS)                                                                           \
struct Vec3 {                                                                                                                                              \
	union {                                                                                                                                                \
		struct {                                                                                                                                           \
			type x, y, z;                                                                                                                                  \
		};                                                                                                                                                 \
		type data[3];                                                                                                                                      \
	};                                                                                                                                                     \
                                                                                                                                                           \
	Vec3() = default;                                                                                                                                      \
	Vec3(type ax, type ay, type az): x(ax), y(ay), z(az) {}                                                                                                \
	explicit Vec3(type v): x(v), y(v), z(v) {}                                                                                                             \
                                                                                                                                                           \
	Vec3 &operator+=(const Vec3 &r) { x+=r.x; y+=r.y; z+=r.z; return *this; }                                                                              \
	Vec3 &operator-=(const Vec3 &r) { x-=r.x; y-=r.y; z-=r.z; return *this; }                                                                              \
	Vec3 &operator*=(const Vec3 &r) { x*=r.x; y*=r.y; z*=r.z; return *this; }                                                                              \
	Vec3 &operator/=(const Vec3 &r) { x/=r.x; y/=r.y; z/=r.z; return *this; }                                                                              \
	ADDITIONAL_MEMBERS(type, Vec3)                                                                                                                         \
                                                                                                                                                           \
	type &operator[](int i) { return data[i]; }                                                                                                            \
	type operator[](int i) const { return data[i]; }                                                                                                       \
                                                                                                                                                           \
	Vec2 XY() const { return {x, y}; }                                                                                                                     \
	Vec2 XZ() const { return {x, z}; }                                                                                                                     \
	Vec2 YZ() const { return {y, z}; }                                                                                                                     \
};                                                                                                                                                         \
                                                                                                                                                           \
static inline Vec3 Vec3##_X(type v = 1) { return {v, 0, 0}; }                                                                                              \
static inline Vec3 Vec3##_Y(type v = 1) { return {0, v, 0}; }                                                                                              \
static inline Vec3 Vec3##_Z(type v = 1) { return {0, 0, v}; }                                                                                              \
static inline Vec3 Vec3##_XY(const Vec2 &v) { return {v.x, v.y, 0}; }                                                                                      \
static inline Vec3 Vec3##_XZ(const Vec2 &v) { return {v.x, 0, v.y}; }                                                                                      \
static inline Vec3 Vec3##_YZ(const Vec2 &v) { return {0, v.x, v.y}; }                                                                                      \
                                                                                                                                                           \
static inline bool operator==(const Vec3 &l, const Vec3 &r) { return l.x == r.x && l.y == r.y && l.z == r.z; }                                             \
static inline bool operator!=(const Vec3 &l, const Vec3 &r) { return l.x != r.x || l.y != r.y || l.z != r.z; }                                             \
static inline bool operator<(const Vec3 &l, const Vec3 &r)  { return l.x < r.x && l.y < r.y && l.z < r.z; }                                                \
static inline bool operator>(const Vec3 &l, const Vec3 &r)  { return l.x > r.x && l.y > r.y && l.z > r.z; }                                                \
static inline bool operator<=(const Vec3 &l, const Vec3 &r) { return l.x <= r.x && l.y <= r.y && l.z <= r.z; }                                             \
static inline bool operator>=(const Vec3 &l, const Vec3 &r) { return l.x >= r.x && l.y >= r.y && l.z >= r.z; }                                             \
static inline Vec3 operator+(const Vec3 &l, const Vec3 &r) { return Vec3(l.x + r.x, l.y + r.y, l.z + r.z); }                                               \
static inline Vec3 operator-(const Vec3 &l, const Vec3 &r) { return Vec3(l.x - r.x, l.y - r.y, l.z - r.z); }                                               \
static inline Vec3 operator*(const Vec3 &l, const Vec3 &r) { return Vec3(l.x * r.x, l.y * r.y, l.z * r.z); }                                               \
static inline Vec3 operator/(const Vec3 &l, const Vec3 &r) { return Vec3(l.x / r.x, l.y / r.y, l.z / r.z); }                                               \
static inline Vec3 operator-(const Vec3 &v) { return Vec3(-v.x, -v.y, -v.z); }                                                                             \
                                                                                                                                                           \
static inline type length2(const Vec3 &v) { return v.x*v.x + v.y*v.y + v.z*v.z; }                                                                          \
static inline type dot(const Vec3 &v1, const Vec3 &v2) { return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; }                                                       \
static inline type volume(const Vec3 &v) { return v.x * v.y * v.z; }                                                                                       \
static inline Vec3 cross(const Vec3 &v1, const Vec3 &v2) { return Vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x); } \
static inline type distance2(const Vec3 &v1, const Vec3 &v2) { return length2(v2-v1); }                                                                    \
static inline Vec3 min(const Vec3 &v1, const Vec3 &v2) { return {min(v1.x, v2.x), min(v1.y, v2.y), min(v1.z, v2.z)}; }                                     \
static inline Vec3 max(const Vec3 &v1, const Vec3 &v2) { return {max(v1.x, v2.x), max(v1.y, v2.y), max(v1.z, v2.z)}; }                                     \
ADDITIONAL_FUNCTIONS(type, Vec3)                                                                                                                           \


_DEFINE_VEC3(float, Vec3f, Vec2f, _DEFINE_VEC3_NO_MEMBERS, _DEFINE_VEC3_FLOAT_FUNCTIONS)
_DEFINE_VEC3(double, Vec3d, Vec2d, _DEFINE_VEC3_NO_MEMBERS, _DEFINE_VEC3_FLOAT_FUNCTIONS)
_DEFINE_VEC3(int32_t, Vec3i, Vec2i, _DEFINE_VEC3_INT_MEMBERS, _DEFINE_VEC3_INT_FUNCTIONS)
_DEFINE_VEC3(int16_t, Vec3s, Vec2s, _DEFINE_VEC3_INT_MEMBERS, _DEFINE_VEC3_INT_FUNCTIONS)
_DEFINE_VEC3(int8_t, Vec3b, Vec2b, _DEFINE_VEC3_INT_MEMBERS, _DEFINE_VEC3_INT_FUNCTIONS)

_DEFINE_VEC3(uint16_t, Vec3us, Vec2us, _DEFINE_VEC3_INT_MEMBERS, _DEFINE_VEC3_INT_FUNCTIONS)


static inline Vec3f ToVec3f(const Vec3i &v) { return Vec3f(float(v.x), float(v.y), float(v.z)); }
static inline Vec3f ToVec3f(const Vec3d &v) { return Vec3f(float(v.x), float(v.y), float(v.z)); }
static inline Vec3d ToVec3d(const Vec3i &v) { return Vec3d(v.x, v.y, v.z); }
static inline Vec3d ToVec3d(const Vec3f &v) { return Vec3d(v.x, v.y, v.z); }
static inline Vec3i ToVec3i(const Vec3f &v) { return Vec3i(int(v.x), int(v.y), int(v.z)); }
static inline Vec3i ToVec3i(const Vec3d &v) { return Vec3i(int(v.x), int(v.y), int(v.z)); }

static inline Vec3i floor(const Vec3f &v) { return Vec3i(int(std::floor(v.x)), int(std::floor(v.y)), int(std::floor(v.z))); }
static inline Vec3i floor(const Vec3d &v) { return Vec3i(int(std::floor(v.x)), int(std::floor(v.y)), int(std::floor(v.z))); }

static inline bool axes_equal(const Vec3i &a, const Vec3i &b, const Vec2i &axes)
{
	return a[axes[0]] == b[axes[0]] && a[axes[1]] == b[axes[1]];
}

static inline bool aabb_aabb_intersection(const Vec3i &amin, const Vec3i &amax,
	const Vec3i &bmin, const Vec3i &bmax)
{
	return !(
		amax.x < bmin.x ||
		amax.y < bmin.y ||
		amax.z < bmin.z ||
		amin.x > bmax.x ||
		amin.y > bmax.y ||
		amin.z > bmax.z
		);
}

//------------------------------------------------------------------------------
// Vec4
//------------------------------------------------------------------------------

struct Vec4f {
	union {
		struct {
			float x, y, z, w;
		};
		float data[4];
	};

	Vec4f() = default;
	Vec4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	explicit Vec4f(float v) : x(v), y(v), z(v), w(v) {}

	Vec4f &operator+=(const Vec4f &r) { x += r.x; y += r.y; z += r.z; w += r.w; return *this; }
	Vec4f &operator-=(const Vec4f &r) { x -= r.x; y -= r.y; z -= r.z; w -= r.w; return *this; }
	Vec4f &operator*=(const Vec4f &r) { x *= r.x; y *= r.y; z *= r.z; w *= r.w; return *this; }
	Vec4f &operator/=(const Vec4f &r) { x /= r.x; y /= r.y; z /= r.z; w /= r.w; return *this; }

	float &operator[](int i) { return data[i]; }
	float operator[](int i) const { return data[i]; }
};

static inline Vec4f operator+(const Vec4f &l, const Vec4f &r) { return { l.x + r.x, l.y + r.y, l.z + r.z, l.w + r.w }; }
static inline Vec4f operator-(const Vec4f &l, const Vec4f &r) { return { l.x - r.x, l.y - r.y, l.z - r.z, l.w - r.w }; }
static inline Vec4f operator*(const Vec4f &l, const Vec4f &r) { return { l.x * r.x, l.y * r.y, l.z * r.z, l.w * r.w }; }
static inline Vec4f operator/(const Vec4f &l, const Vec4f &r) { return { l.x / r.x, l.y / r.y, l.z / r.z, l.w / r.w }; }

static inline float dot(const Vec4f &v1, const Vec4f &v2) { return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w; }

static inline Vec3f ToVec3(const Vec4f &v) { return Vec3f(v.x, v.y, v.z); }
static inline Vec4f ToVec4(const Vec3f &v) { return Vec4f(v.x, v.y, v.z, 1); }

//------------------------------------------------------------------------------
// Vec4i
//------------------------------------------------------------------------------

struct Vec4i {
	union {
		struct {
			int x, y, z, w;
		};
		int data[4];
	};

	Vec4i() = default;
	Vec4i(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}
	explicit Vec4i(int v) : x(v), y(v), z(v), w(v) {}

	Vec4i &operator+=(const Vec4i &r) { x += r.x; y += r.y; z += r.z; w += r.w; return *this; }
	Vec4i &operator-=(const Vec4i &r) { x -= r.x; y -= r.y; z -= r.z; w -= r.w; return *this; }
	Vec4i &operator*=(const Vec4i &r) { x *= r.x; y *= r.y; z *= r.z; w *= r.w; return *this; }
	Vec4i &operator/=(const Vec4i &r) { x /= r.x; y /= r.y; z /= r.z; w /= r.w; return *this; }

	int &operator[](int i) { return data[i]; }
	int operator[](int i) const { return data[i]; }
};

static inline Vec4i operator+(const Vec4i &l, const Vec4i &r) { return { l.x + r.x, l.y + r.y, l.z + r.z, l.w + r.w }; }
static inline Vec4i operator-(const Vec4i &l, const Vec4i &r) { return { l.x - r.x, l.y - r.y, l.z - r.z, l.w - r.w }; }
static inline Vec4i operator*(const Vec4i &l, const Vec4i &r) { return { l.x * r.x, l.y * r.y, l.z * r.z, l.w * r.w }; }
static inline Vec4i operator/(const Vec4i &l, const Vec4i &r) { return { l.x / r.x, l.y / r.y, l.z / r.z, l.w / r.w }; }

static inline Vec4f ToVec4(const Vec4i &v) { return Vec4f(float(v.x), float(v.y), float(v.z), float(v.w)); }
static inline Vec4i ToVec4i(const Vec4f &v) { return Vec4i(int(v.x), int(v.y), int(v.z), int(v.w)); }

//------------------------------------------------------------------------------
// Macro Utils
//------------------------------------------------------------------------------

#define VEC2(v) (v).x, (v).y
#define VEC3(v) (v).x, (v).y, (v).z
#define VEC4(v) (v).x, (v).y, (v).z, (v).w


struct Vertex
{
	Vec3f position;
	Vec3f normal;
};

static std::vector<float> voxels;
static std::vector<Vertex> vertices;
static std::vector<int> indices;
static int nx, ny, nz;

static inline int offset_3d(const Vec3i &p, const Vec3i &size)
{
	return (p.z * size.y + p.y) * size.x + p.x;
}

static inline int offset_3d_slab(const Vec3i &p, const Vec3i &size)
{
	return size.x * size.y * (p.z % 2) + p.y * size.x + p.x;
}

static void generate_voxels(const TTree* tree, int res)
{
	Box clipped = tree->GetBox();
	clipped.SetParallelepipedic(res, nx, ny, nz);
	Vector3 d = clipped[1] - clipped[0];
	d.x /= (nx - 1);
	d.y /= (ny - 1);
	d.z /= (nz - 1);
	voxels.resize(nx * ny * nz);
	for (int x = 0; x < nx; x++)
	{
		for (int y = 0; y < ny; y++)
		{
			for (int z = 0; z < nz; z++)
			{
				const int offset = offset_3d({ x, y, z }, Vec3i(nx, ny, nz));
				Vector3 p = Vector3(clipped[0] + Vector3(x * d[0], y * d[1], z * d[2]));
				voxels[offset] = tree->Intensity(p);
			}
		}
	}
}

static const uint64_t marching_cube_tris[256] = {
	0ULL, 33793ULL, 36945ULL, 159668546ULL,
	18961ULL, 144771090ULL, 5851666ULL, 595283255635ULL,
	20913ULL, 67640146ULL, 193993474ULL, 655980856339ULL,
	88782242ULL, 736732689667ULL, 797430812739ULL, 194554754ULL,
	26657ULL, 104867330ULL, 136709522ULL, 298069416227ULL,
	109224258ULL, 8877909667ULL, 318136408323ULL, 1567994331701604ULL,
	189884450ULL, 350847647843ULL, 559958167731ULL, 3256298596865604ULL,
	447393122899ULL, 651646838401572ULL, 2538311371089956ULL, 737032694307ULL,
	29329ULL, 43484162ULL, 91358498ULL, 374810899075ULL,
	158485010ULL, 178117478419ULL, 88675058979ULL, 433581536604804ULL,
	158486962ULL, 649105605635ULL, 4866906995ULL, 3220959471609924ULL,
	649165714851ULL, 3184943915608436ULL, 570691368417972ULL, 595804498035ULL,
	124295042ULL, 431498018963ULL, 508238522371ULL, 91518530ULL,
	318240155763ULL, 291789778348404ULL, 1830001131721892ULL, 375363605923ULL,
	777781811075ULL, 1136111028516116ULL, 3097834205243396ULL, 508001629971ULL,
	2663607373704004ULL, 680242583802939237ULL, 333380770766129845ULL, 179746658ULL,
	42545ULL, 138437538ULL, 93365810ULL, 713842853011ULL,
	73602098ULL, 69575510115ULL, 23964357683ULL, 868078761575828ULL,
	28681778ULL, 713778574611ULL, 250912709379ULL, 2323825233181284ULL,
	302080811955ULL, 3184439127991172ULL, 1694042660682596ULL, 796909779811ULL,
	176306722ULL, 150327278147ULL, 619854856867ULL, 1005252473234484ULL,
	211025400963ULL, 36712706ULL, 360743481544788ULL, 150627258963ULL,
	117482600995ULL, 1024968212107700ULL, 2535169275963444ULL, 4734473194086550421ULL,
	628107696687956ULL, 9399128243ULL, 5198438490361643573ULL, 194220594ULL,
	104474994ULL, 566996932387ULL, 427920028243ULL, 2014821863433780ULL,
	492093858627ULL, 147361150235284ULL, 2005882975110676ULL, 9671606099636618005ULL,
	777701008947ULL, 3185463219618820ULL, 482784926917540ULL, 2900953068249785909ULL,
	1754182023747364ULL, 4274848857537943333ULL, 13198752741767688709ULL, 2015093490989156ULL,
	591272318771ULL, 2659758091419812ULL, 1531044293118596ULL, 298306479155ULL,
	408509245114388ULL, 210504348563ULL, 9248164405801223541ULL, 91321106ULL,
	2660352816454484ULL, 680170263324308757ULL, 8333659837799955077ULL, 482966828984116ULL,
	4274926723105633605ULL, 3184439197724820ULL, 192104450ULL, 15217ULL,
	45937ULL, 129205250ULL, 129208402ULL, 529245952323ULL,
	169097138ULL, 770695537027ULL, 382310500883ULL, 2838550742137652ULL,
	122763026ULL, 277045793139ULL, 81608128403ULL, 1991870397907988ULL,
	362778151475ULL, 2059003085103236ULL, 2132572377842852ULL, 655681091891ULL,
	58419234ULL, 239280858627ULL, 529092143139ULL, 1568257451898804ULL,
	447235128115ULL, 679678845236084ULL, 2167161349491220ULL, 1554184567314086709ULL,
	165479003923ULL, 1428768988226596ULL, 977710670185060ULL, 10550024711307499077ULL,
	1305410032576132ULL, 11779770265620358997ULL, 333446212255967269ULL, 978168444447012ULL,
	162736434ULL, 35596216627ULL, 138295313843ULL, 891861543990356ULL,
	692616541075ULL, 3151866750863876ULL, 100103641866564ULL, 6572336607016932133ULL,
	215036012883ULL, 726936420696196ULL, 52433666ULL, 82160664963ULL,
	2588613720361524ULL, 5802089162353039525ULL, 214799000387ULL, 144876322ULL,
	668013605731ULL, 110616894681956ULL, 1601657732871812ULL, 430945547955ULL,
	3156382366321172ULL, 7644494644932993285ULL, 3928124806469601813ULL, 3155990846772900ULL,
	339991010498708ULL, 10743689387941597493ULL, 5103845475ULL, 105070898ULL,
	3928064910068824213ULL, 156265010ULL, 1305138421793636ULL, 27185ULL,
	195459938ULL, 567044449971ULL, 382447549283ULL, 2175279159592324ULL,
	443529919251ULL, 195059004769796ULL, 2165424908404116ULL, 1554158691063110021ULL,
	504228368803ULL, 1436350466655236ULL, 27584723588724ULL, 1900945754488837749ULL,
	122971970ULL, 443829749251ULL, 302601798803ULL, 108558722ULL,
	724700725875ULL, 43570095105972ULL, 2295263717447940ULL, 2860446751369014181ULL,
	2165106202149444ULL, 69275726195ULL, 2860543885641537797ULL, 2165106320445780ULL,
	2280890014640004ULL, 11820349930268368933ULL, 8721082628082003989ULL, 127050770ULL,
	503707084675ULL, 122834978ULL, 2538193642857604ULL, 10129ULL,
	801441490467ULL, 2923200302876740ULL, 1443359556281892ULL, 2901063790822564949ULL,
	2728339631923524ULL, 7103874718248233397ULL, 12775311047932294245ULL, 95520290ULL,
	2623783208098404ULL, 1900908618382410757ULL, 137742672547ULL, 2323440239468964ULL,
	362478212387ULL, 727199575803140ULL, 73425410ULL, 34337ULL,
	163101314ULL, 668566030659ULL, 801204361987ULL, 73030562ULL,
	591509145619ULL, 162574594ULL, 100608342969108ULL, 5553ULL,
	724147968595ULL, 1436604830452292ULL, 176259090ULL, 42001ULL,
	143955266ULL, 2385ULL, 18433ULL, 0ULL,
};

static void triangle(int a, int b, int c)
{
	Vertex &va = vertices[a];
	Vertex &vb = vertices[b];
	Vertex &vc = vertices[c];
	const Vec3f ab = va.position - vb.position;
	const Vec3f cb = vc.position - vb.position;
	const Vec3f n = cross(cb, ab);
	va.normal += n;
	vb.normal += n;
	vc.normal += n;
}

static void generate_geometry_smooth()
{
	static std::vector<Vec3i> slab_inds(nx * ny * 2);

	for (int z = 0; z < nx - 1; z++) 
	{
		for (int y = 0; y < ny - 1; y++) 
		{
			for (int x = 0; x < nz - 1; x++) 
			{
				const Vec3i p(x, y, z);
				const float vs[8] = {
					voxels[offset_3d({x,   y,   z},   Vec3i(nx, ny, nz))],
					voxels[offset_3d({x + 1, y,   z},   Vec3i(nx, ny, nz))],
					voxels[offset_3d({x,   y + 1, z},   Vec3i(nx, ny, nz))],
					voxels[offset_3d({x + 1, y + 1, z},   Vec3i(nx, ny, nz))],
					voxels[offset_3d({x,   y,   z + 1}, Vec3i(nx, ny, nz))],
					voxels[offset_3d({x + 1, y,   z + 1}, Vec3i(nx, ny, nz))],
					voxels[offset_3d({x,   y + 1, z + 1}, Vec3i(nx, ny, nz))],
					voxels[offset_3d({x + 1, y + 1, z + 1}, Vec3i(nx, ny, nz))],
				};

				const int config_n =
					((vs[0] < 0.0f) << 0) |
					((vs[1] < 0.0f) << 1) |
					((vs[2] < 0.0f) << 2) |
					((vs[3] < 0.0f) << 3) |
					((vs[4] < 0.0f) << 4) |
					((vs[5] < 0.0f) << 5) |
					((vs[6] < 0.0f) << 6) |
					((vs[7] < 0.0f) << 7);

				if (config_n == 0 || config_n == 255)
					continue;

				auto do_edge = [&](int n_edge, float va, float vb, int axis, const Vec3i &p) {
					if ((va < 0.0) == (vb < 0.0))
						return;

					Vec3f v = ToVec3f(p);
					v[axis] += va / (va - vb);
					slab_inds[offset_3d_slab(p, Vec3i(nx, ny, nz))][axis] = int32_t(vertices.size());
					vertices.push_back({ v, Vec3f(0) });
				};

				if (p.y == 0 && p.z == 0)
					do_edge(0, vs[0], vs[1], 0, Vec3i(x, y, z));
				if (p.z == 0)
					do_edge(1, vs[2], vs[3], 0, Vec3i(x, y + 1, z));
				if (p.y == 0)
					do_edge(2, vs[4], vs[5], 0, Vec3i(x, y, z + 1));
				do_edge(3, vs[6], vs[7], 0, Vec3i(x, y + 1, z + 1));

				if (p.x == 0 && p.z == 0)
					do_edge(4, vs[0], vs[2], 1, Vec3i(x, y, z));
				if (p.z == 0)
					do_edge(5, vs[1], vs[3], 1, Vec3i(x + 1, y, z));
				if (p.x == 0)
					do_edge(6, vs[4], vs[6], 1, Vec3i(x, y, z + 1));
				do_edge(7, vs[5], vs[7], 1, Vec3i(x + 1, y, z + 1));

				if (p.x == 0 && p.y == 0)
					do_edge(8, vs[0], vs[4], 2, Vec3i(x, y, z));
				if (p.y == 0)
					do_edge(9, vs[1], vs[5], 2, Vec3i(x + 1, y, z));
				if (p.x == 0)
					do_edge(10, vs[2], vs[6], 2, Vec3i(x, y + 1, z));
				do_edge(11, vs[3], vs[7], 2, Vec3i(x + 1, y + 1, z));

				int edge_indices[12];
				edge_indices[0] = slab_inds[offset_3d_slab({ p.x, p.y,   p.z }, Vec3i(nx, ny, nz))].x;
				edge_indices[1] = slab_inds[offset_3d_slab({ p.x, p.y + 1, p.z }, Vec3i(nx, ny, nz))].x;
				edge_indices[2] = slab_inds[offset_3d_slab({ p.x, p.y,   p.z + 1 }, Vec3i(nx, ny, nz))].x;
				edge_indices[3] = slab_inds[offset_3d_slab({ p.x, p.y + 1, p.z + 1 }, Vec3i(nx, ny, nz))].x;
				edge_indices[4] = slab_inds[offset_3d_slab({ p.x,   p.y, p.z }, Vec3i(nx, ny, nz))].y;
				edge_indices[5] = slab_inds[offset_3d_slab({ p.x + 1, p.y, p.z }, Vec3i(nx, ny, nz))].y;
				edge_indices[6] = slab_inds[offset_3d_slab({ p.x,   p.y, p.z + 1 }, Vec3i(nx, ny, nz))].y;
				edge_indices[7] = slab_inds[offset_3d_slab({ p.x + 1, p.y, p.z + 1 }, Vec3i(nx, ny, nz))].y;
				edge_indices[8] = slab_inds[offset_3d_slab({ p.x,   p.y,   p.z }, Vec3i(nx, ny, nz))].z;
				edge_indices[9] = slab_inds[offset_3d_slab({ p.x + 1, p.y,   p.z }, Vec3i(nx, ny, nz))].z;
				edge_indices[10] = slab_inds[offset_3d_slab({ p.x,   p.y + 1, p.z }, Vec3i(nx, ny, nz))].z;
				edge_indices[11] = slab_inds[offset_3d_slab({ p.x + 1, p.y + 1, p.z }, Vec3i(nx, ny, nz))].z;

				const uint64_t config = marching_cube_tris[config_n];
				const int n_triangles = config & 0xF;
				const int n_indices = n_triangles * 3;
				const int index_base = int(indices.size());

				int offset = 4;
				for (int i = 0; i < n_indices; i++) {
					const int edge = (config >> offset) & 0xF;
					indices.push_back(edge_indices[edge]);
					offset += 4;
				}
				for (int i = 0; i < n_triangles; i++) {
					triangle(
						indices[index_base + i * 3 + 0],
						indices[index_base + i * 3 + 1],
						indices[index_base + i * 3 + 2]);
				}
			}
		}
	}
	for (Vertex &v : vertices)
		v.normal = normalize(v.normal);
}


void marching_cube(const char* url, const TTree* tree, int res)
{
	// Clear old data
	vertices.clear();
	indices.clear();
	voxels.clear();

	// Query field function
	generate_voxels(tree, res);

	// Generate geometry
	generate_geometry_smooth();

	// Export as .obj file
	std::ofstream out;
	out.open(url);
	if (out.is_open() == false)
		return;
	out << "g " << "Obj" << std::endl;
	for (int i = 0; i < vertices.size(); i++)
		out << "v " << vertices.at(i).position.x << " " << vertices.at(i).position.y << " " << vertices.at(i).position.z << '\n';
	for (int i = 0; i < vertices.size(); i++)
		out << "vn " << vertices.at(i).normal.x << " " << vertices.at(i).normal.y << " " << vertices.at(i).normal.z << '\n';
	for (int i = 0; i < indices.size(); i += 3)
	{
		out << "f " << indices.at(i) + 1 << "//" << indices.at(i) + 1
			<< " " << indices.at(i + 1) + 1 << "//" << indices.at(i + 1) + 1
			<< " " << indices.at(i + 2) + 1 << "//" << indices.at(i + 2) + 1
			<< '\n';
	}
	out.close();
}
