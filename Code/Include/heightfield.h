#pragma once

#include "basics.h"

class TAnalyticCliff;

class HeightField : public ScalarField2D
{
public:
	HeightField();
	HeightField(int nx, int ny, const Box2D& bbox);
	HeightField(int nx, int ny, const Box2D& bbox, float value);
	HeightField(const TAnalyticCliff* cliff, int nx, int ny, const Box2D& bbox);
	~HeightField();
	
	ScalarField2D Slope() const;
};
