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

// Blend
class GeoBlend : public GeoBinary
{
public:
	GeoBlend(GeoNode* e1, GeoNode* e2);
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
