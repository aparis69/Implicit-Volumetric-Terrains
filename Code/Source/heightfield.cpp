#include "heightfield.h"
#include "vec.h"
#include "ttree.h"

#include <algorithm>
#include <array>
#include <deque>
#include <queue>

/*
\brief Main Class for representing 2D HeightField.
*/

/*
\brief Default constructor. Call Scalarfield2D default constructor.
*/
HeightField::HeightField() : ScalarField2D()
{
}

/*
\brief Constructor
\param nx width size of field
\param ny height size of field
\param bottomLeft bottom left vertex world coordinates
\param topRight top right vertex world coordinates
*/
HeightField::HeightField(int nx, int ny, const Box2D& bbox) : ScalarField2D(nx, ny, bbox)
{
}

/*
\brief Constructor
\param nx width size of field
\param ny height size of field
\param bbox bounding box in world space
\param value default value for the field
*/
HeightField::HeightField(int nx, int ny, const Box2D& bbox, float value) : ScalarField2D(nx, ny, bbox, value)
{

}

/*!
\brief Constructor from an analytic heightfield (from the TTree structure) mostly for convenience.
\param cliff analytic heightfield
\param nx width size of field
\param ny height size of field
\param bbox bounding box in world space
*/
HeightField::HeightField(const TAnalyticCliff* cliff, int nx, int ny, const Box2D& bbox) : ScalarField2D(nx, ny, bbox, 0.0f)
{
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < ny; j++)
			values[ToIndex1D(i, j)] = cliff->Height(Vector2(Vertex(i, j)));
	}
}

/*
\brief Destructor
*/
HeightField::~HeightField()
{
}

/*
\brief Compute the slope field, ie Magnitude(Gradient(i, j)).
*/
ScalarField2D HeightField::Slope() const
{
	ScalarField2D S = ScalarField2D(nx, ny, box);
	for (int i = 0; i < ny; i++)
	{
		for (int j = 0; j < nx; j++)
			S.Set(i, j, Magnitude(Gradient(i, j)));
	}
	return S;
}
