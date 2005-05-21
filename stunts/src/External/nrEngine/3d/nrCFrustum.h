/***************************************************************************
 *                                                                         *
 *   (c) Art Tevs, MPI Informatik Saarbruecken                             *
 *       mailto: <tevs@mpi-sb.mpg.de>                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __NR_C_FRUSTUM_H
#define __NR_C_FRUSTUM_H


/*------------------------------------------------------------------------------
 * Forward Declarations
 *----------------------------------------------------------------------------*/
class nrCFrustum;
	

/*------------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------------*/
#include "../nrEngine.h"


/**
 * This class is used to represent our viewing frustum. It can be used to implement frustum
 * culling to increase rendering speed. 
 * \ingroup 3d
 **/
class nrCFrustum {
public:

    // BBox inside
    int AABBInside(const vec3 &min,const vec3 &max);
    int AABBFullInside(const vec3 &min,const vec3 &max);

    // Call this every time the camera moves to update the frustum
    void CalculateFrustum(const mat4& modelview, const mat4& projection);
    void CalculateGLFrustum();

    // This takes a 3D point and returns TRUE if it's inside of the frustum
    bool PointInFrustum(float x, float y, float z);

    // This takes a 3D point and a radius and returns TRUE if the sphere is inside of the frustum
    bool SphereInFrustum(float x, float y, float z, float radius);

    // This takes the center and half the length of the cube.
    bool CubeInFrustum( float x, float y, float z, float size );

    bool TriangleInFrustum (vec3, vec3, vec3);
    bool BoxInFrustum( float x, float y, float z, float halfwidth, float halfheight, 
						float halfdepth );

private:
	// We create an enum of the sides so we don't have to call each side 0 or 1.
	// This way it makes it more understandable and readable when dealing with frustum sides.
	enum FrustumSide
	{
		RIGHT   = 0,        // The RIGHT side of the frustum
		LEFT    = 1,        // The LEFT  side of the frustum
		BOTTOM  = 2,        // The BOTTOM side of the frustum
		TOP     = 3,        // The TOP side of the frustum
		BACK    = 4,        // The BACK side of the frustum
		FRONT   = 5         // The FRONT side of the frustum
	}; 
	
	// Like above, instead of saying a number for the ABC and D of the plane, we
	// want to be more descriptive.
	enum PlaneData
	{
		A = 0,              // The X value of the plane's normal
		B = 1,              // The Y value of the plane's normal
		C = 2,              // The Z value of the plane's normal
	
		D = 3               // The distance the plane is from the origin
	};

    // This holds the A B C and D values for each side of our frustum.
    vec4 m_Frustum[6];
};


#endif
