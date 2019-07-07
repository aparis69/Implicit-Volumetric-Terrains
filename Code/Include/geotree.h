#pragma once
#include "basics.h"
#include "noise.h"

// Base node class
class GeoNode
{
public:
	inline GeoNode() { /* Empty */ }
	inline virtual ~GeoNode() { /* Empty */ }
	virtual float Intensity(const Vector3& p) const = 0;
	inline virtual int Memory() const { return sizeof(GeoNode); }
};

// Generic Cubic falloff
class GeoCubicFalloff : public GeoNode
{
protected:
	float r; //!< Radius.
	float e; //!< Energy.
public:
	GeoCubicFalloff(float r, float e);
	float Falloff(float sd) const;
};

// Horizontal strata primitive
class GeoStrata : public GeoCubicFalloff
{
protected:
	float z; //!< Altitude.
public:
	GeoStrata(float z, float r, float e);
	float Intensity(const Vector3& p) const;
	int Memory() const;
};

// Vertex skeleton primitive
class GeoVertex : public GeoCubicFalloff
{
protected:
	Vector3 c; //!< Center
public:
	GeoVertex(const Vector3& c, float r, float e);
	float Intensity(const Vector3& p) const;
	int Memory() const;
};

// Sphere skeleton primitive
class GeoSphere : public GeoCubicFalloff, protected Sphere
{
public:
	GeoSphere(const Vector3& c, float r, float sr, float e);
	float Intensity(const Vector3& p) const;
	int Memory() const;
};

// Uniform
class GeoUniform : public GeoNode
{
protected:
	float a; //!< %Uniform value.
public:
	GeoUniform(float A = 2.0);
	float Intensity(const Vector3& p) const;
	int Memory() const;
};

// A 1D noise, function of altitude
class GeoNoise1D : public GeoNode
{
protected:
	float a;	//!< %Noise amplitude.
	float f;	//!< %Noise frequency.
	int o;		//!< %Noise octave count.
public:
	GeoNoise1D(float f, int o, float A = 2.0);
	float Intensity(const Vector3& p) const;
	int Memory() const;
};

// A 3D noise
class GeoNoise3D : public GeoNode
{
protected:
	float a; //!< %Noise amplitude.
	float f; //!< %Noise frequency.
public:
	GeoNoise3D(float a, float f);
	float Intensity(const Vector3& p) const;
	int Memory() const;
};

// 3D Fractal sum of noise
class GeoFractal3D : public GeoNode
{
protected:
	float a;	//!< %Noise amplitude.
	float f;	//!< %Noise frequency.
	int o;		//!< %Noise octave count.
public:
	GeoFractal3D(float a, float f, int o);
	float Intensity(const Vector3& p) const;
	int Memory() const;
};

// A 3D noise with hardcoded parameters, for the karst scene.
class GeoNoiseKarst : public GeoNode
{
public:
	GeoNoiseKarst();
	float Intensity(const Vector3& p) const;
};

// Binary node
class GeoBinary : public GeoNode
{
protected:
	GeoNode* e[2]; //!< Sub trees.
public:
	GeoBinary(GeoNode* e1, GeoNode* e2);
	virtual int Memory() const;
};

// Unary node
class GeoUnary : public GeoNode
{
protected:
	GeoNode * e;	//!< Sub tree.
public:
	GeoUnary(GeoNode* e);
	virtual int Memory() const;
};

// Blend
class GeoBlend : public GeoBinary
{
public:
	GeoBlend(GeoNode* e1, GeoNode* e2);
	float Intensity(const Vector3& p) const;
};

// Warping node along a noise
class GeoWarp : public GeoUnary
{
public:
	GeoWarp(GeoNode* e);
	float Intensity(const Vector3& p) const;
};

// Construction tree
class GeoTree
{
protected:
	GeoNode* root;	//!< Root node.

public:
	GeoTree(GeoNode* n);
	~GeoTree();

	float Intensity(const Vector3& p) const;
	void Blend(GeoNode* n);
	int Memory() const;
};
