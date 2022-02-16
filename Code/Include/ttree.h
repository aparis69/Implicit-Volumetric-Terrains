#pragma once

#include "basics.h"
#include "heightfield.h"

class GeoTree;

// Generic node
class TNode
{
protected:
	Box box;	//!< %Bounding box.

public:
	TNode();
	TNode(const Box&);
	virtual ~TNode();

	virtual float Intensity(const Vector3&) const;
	virtual Vector3 Gradient(const Vector3&) const;
	virtual Box GetBox() const;
};

// Generic Primitive Node, without a bounding box.
class TPrimitive : public TNode
{
public:
	TPrimitive();
	TPrimitive(const Box&);

	virtual float Intensity(const Vector3&) const = 0;
};

// Base class for 2D terrain primitives
class TTerrainNode : public TNode
{
protected:
	float x;		//!< Amplitude.
	float r;		//!< Radius of influence.
	Box localbox;	//!< %Box.

public:
	TTerrainNode(const Box&, const float& X, const float& E);

	float Intensity(const Vector3&) const;
	virtual float Height(const Vector2&) const;
};

// Floating Island primitive used for the paper' images.
class TFloatingIsland : public TPrimitive
{
protected:
	Box localbox;	//!< %Box.
	float r;		//!< Radius.
	float depth;	//!< Depth.
	float height;	//!< Height.
	Vector3 c;		//!< Position.

public:
	TFloatingIsland(const Vector3& c, float r, float, float);

	float Intensity(const Vector3&) const;
};

// Floating Island primitive used for the paper' images.
class TFloatingIsland2 : public TPrimitive
{
protected:
	Box localbox;	//!< %Box.
	float r;		//!< Radius.
	float depth;	//!< Depth.
	float height;	//!< Height.
	Vector3 c;		//!< Position.

public:
	TFloatingIsland2(const Vector3& c, const float& r, const float&, const float&);

	float Intensity(const Vector3&) const;
};

// Heightfield, Elevation computed analytically with some warped noise.
class TAnalyticCliff : public TTerrainNode
{
protected:
	Vector2 c;						//!< Center.
	Vector2 minMaxElevation;		//!< Min and max elevation values.

public:
	TAnalyticCliff(const Box& B, const Vector2& minMaxElevation);

	float Height(const Vector2&) const;
};

// Cubic falloff, used by all skeletal primitives.
class TCubicFalloff : public TPrimitive
{
protected:
	float r;	//!< Radius.
	float e;	//!< Energy.

public:
	TCubicFalloff(float e, float r);

protected:
	float Falloff(float x) const;
	float Falloff(float x, float r) const;
};

// Vertex skeletal primitive
class TVertex : public TCubicFalloff
{
protected:
	Vector3 c;	//!< Center.

public:
	TVertex(const Vector3& c, float r, float e);

	virtual float Intensity(const Vector3&) const;
};

// Binary Operator 
class TBinary : public TNode
{
protected:
	TNode * e[2]; //!< Left and right sub-trees.

public:
	TBinary(TNode*, TNode*);
	~TBinary();
};

// Blend
class TBlend : public TBinary
{
public:
	TBlend(TNode*, TNode*);
	TBlend(TNode*, TNode*, TNode*);
	TBlend(TNode*, TNode*, TNode*, TNode*);
	float Intensity(const Vector3&) const;
	Vector3 Gradient(const Vector3&) const;
};

// Constructive Tree
class TTree
{
private:
	TNode* root;			//!< Root node.
	static float t;			//!< %Surface threshold value.

public:
	TTree(TNode*);
	virtual ~TTree();

	float Intensity(const Vector3&) const;
	Vector3 Gradient(const Vector3&) const;
	Box GetBox() const;
	void Blend(TNode*);
	bool Find(Vector3& p, bool s, const Box& box, int n) const;
	Vector3 Dichotomy(Vector3 a, Vector3 b, float va, float vb, float length, float epsilon) const;
	bool GetSample(Vector3& p, const Box& box) const;

	// Static
	static float T();
};

void marching_cube(const char* url, const TTree* tree, int res);
