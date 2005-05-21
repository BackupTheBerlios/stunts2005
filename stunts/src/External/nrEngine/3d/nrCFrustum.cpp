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


#include "nrCFrustum.h"

/**-------------------------------------------------------------------------------
 *   This normalizes a plane (A side) from a given frustum.
 *------------------------------------------------------------------------------*/
void NormalizePlane(vec4& frustum){
    // Here we calculate the magnitude of the normal to the plane (point A B C)
    // Remember that (A, B, C) is that same thing as the normal's (X, Y, Z).
    // To calculate magnitude you use the equation:  magnitude = sqrt( x^2 + y^2 + z^2)
	float magnitude = (float)sqrt(frustum[0]*frustum[0] +
								frustum[1]*frustum[1] +
								frustum[2]*frustum[2]);
	
    // Then we divide the plane's values by it's magnitude.
    // This makes it easier to work with.
	frustum[0] /= magnitude;
	frustum[1] /= magnitude;
	frustum[2] /= magnitude;
	frustum[3] /= magnitude;
}
  

/**
 * This will calculate current frustum from OpenGL's matricies
 **/
void nrCFrustum::CalculateGLFrustum(){    
    
	mat4 proj;
	mat4 modl;
	
	glGetFloatv( GL_PROJECTION_MATRIX, proj );
	glGetFloatv( GL_MODELVIEW_MATRIX, modl );

	CalculateFrustum(modl, proj);
	
}

/**
 * This will calculate current frustum from given projection and modelview matrix
 * @param modl - modelview matrix
 * @param proj - projection matrix
 * @todo Optimize this function to increase computation speed. 
 *		 You can rewrite this with an Assembler to increase speed
 **/
void nrCFrustum::CalculateFrustum(const mat4& modl, const mat4& proj){    
    float   clip[16]; //This will hold the clipping planes

    // Now that we have our modelview and projection matrix, if we combine these 2 matrices,
    // it will give us our clipping planes.  To combine 2 matrices, we multiply them.

    clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
    clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
    clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
    clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

    clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
    clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
    clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
    clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

    clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
    clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
    clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
    clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

    clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
    clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
    clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
    clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];
    
    // Now we actually want to get the sides of the frustum.  To do this we take
    // the clipping planes we received above and extract the sides from them.

    // This will extract the RIGHT side of the frustum
    m_Frustum[RIGHT][A] = clip[ 3] - clip[ 0];
    m_Frustum[RIGHT][B] = clip[ 7] - clip[ 4];
    m_Frustum[RIGHT][C] = clip[11] - clip[ 8];
    m_Frustum[RIGHT][D] = clip[15] - clip[12];

    // Now that we have a normal (A,B,C) and a distance (D) to the plane,
    // we want to normalize that normal and distance.

    // Normalize the RIGHT side
    NormalizePlane(m_Frustum[RIGHT]);

    // This will extract the LEFT side of the frustum
    m_Frustum[LEFT][A] = clip[ 3] + clip[ 0];
    m_Frustum[LEFT][B] = clip[ 7] + clip[ 4];
    m_Frustum[LEFT][C] = clip[11] + clip[ 8];
    m_Frustum[LEFT][D] = clip[15] + clip[12];

    // Normalize the LEFT side
    NormalizePlane(m_Frustum[LEFT]);

    // This will extract the BOTTOM side of the frustum
    m_Frustum[BOTTOM][A] = clip[ 3] + clip[ 1];
    m_Frustum[BOTTOM][B] = clip[ 7] + clip[ 5];
    m_Frustum[BOTTOM][C] = clip[11] + clip[ 9];
    m_Frustum[BOTTOM][D] = clip[15] + clip[13];

    // Normalize the BOTTOM side
    NormalizePlane(m_Frustum[BOTTOM]);

    // This will extract the TOP side of the frustum
    m_Frustum[TOP][A] = clip[ 3] - clip[ 1];
    m_Frustum[TOP][B] = clip[ 7] - clip[ 5];
    m_Frustum[TOP][C] = clip[11] - clip[ 9];
    m_Frustum[TOP][D] = clip[15] - clip[13];

    // Normalize the TOP side
    NormalizePlane(m_Frustum[TOP]);

    // This will extract the BACK side of the frustum
    m_Frustum[BACK][A] = clip[ 3] - clip[ 2];
    m_Frustum[BACK][B] = clip[ 7] - clip[ 6];
    m_Frustum[BACK][C] = clip[11] - clip[10];
    m_Frustum[BACK][D] = clip[15] - clip[14];

    // Normalize the BACK side
    NormalizePlane(m_Frustum[BACK]);

    // This will extract the FRONT side of the frustum
    m_Frustum[FRONT][A] = clip[ 3] + clip[ 2];
    m_Frustum[FRONT][B] = clip[ 7] + clip[ 6];
    m_Frustum[FRONT][C] = clip[11] + clip[10];
    m_Frustum[FRONT][D] = clip[15] + clip[14];

    // Normalize the FRONT side
    NormalizePlane(m_Frustum[FRONT]);

}


/**
 * Check if a given point lies in our frustum
 * @param x,y,z out point in 3D-Space
 * @return true if inside or false otherwise
 **/
bool nrCFrustum::PointInFrustum( float x, float y, float z ){
    
	// Go through all the sides of the frustum
    for(int i = 0; i < 6; i++ ){
        // Calculate the plane equation and check if the point is behind a side of the frustum
        if(m_Frustum[i][A] * x + m_Frustum[i][B] * y + m_Frustum[i][C] * z + m_Frustum[i][D] <= 0)
        {
            // The point was behind a side, so it ISN'T in the frustum
            return false;
        }
    }

    // The point was inside of the frustum (In front of ALL the sides of the frustum)
    return true;
}


/**
 * Check if a given sphere lies in our frustum
 * @param x,y,z middle point of our sphere
 * @param radius radius of a sphere to be checked
 * @return true if inside or false otherwise
 **/
bool nrCFrustum::SphereInFrustum( float x, float y, float z, float radius )
{
    // Go through all the sides of the frustum
    for(int i = 0; i < 6; i++ ) 
    {
		float abstand = m_Frustum[i][A] * x + m_Frustum[i][B] * y + m_Frustum[i][C] * z + m_Frustum[i][D];
		
        // If the center of the sphere is farther away from the plane than the radius
        if( abstand <= -radius)
        {
            // The distance was greater than the radius so the sphere is outside of the frustum
            return false;
        }
    }


    
    // The sphere was inside of the frustum!
    return true;
}


/**
 * This determines if a cube is in or around our frustum by it's center and 1/2 it's length
 * @param x,y,z center of our cube
 * @param size 1/2 of cube's length (like radius on spheres)
 * @return true if inside or false otherwise
 **/
bool nrCFrustum::CubeInFrustum( float x, float y, float z, float size )
{
    // Basically, what is going on is, that we are given the center of the cube,

    // and half the length.  Think of it like a radius.  Then we checking each point
    // in the cube and seeing if it is inside the frustum.  If a point is found in front
    // of a side, then we skip to the next side.  If we get to a plane that does NOT have
    // a point in front of it, then it will return false.

    // *Note* - This will sometimes say that a cube is inside the frustum when it isn't.
    // This happens when all the corners of the bounding box are not behind any one plane.
    // This is rare and shouldn't effect the overall rendering speed.

    for(int i = 0; i < 6; i++ )
    {
        if(m_Frustum[i][A] * (x - size) + m_Frustum[i][B] * (y - size) + m_Frustum[i][C] * (z - size) + m_Frustum[i][D] > 0)
           continue;
        if(m_Frustum[i][A] * (x + size) + m_Frustum[i][B] * (y - size) + m_Frustum[i][C] * (z - size) + m_Frustum[i][D] > 0)
           continue;
        if(m_Frustum[i][A] * (x - size) + m_Frustum[i][B] * (y + size) + m_Frustum[i][C] * (z - size) + m_Frustum[i][D] > 0)
           continue;
        if(m_Frustum[i][A] * (x + size) + m_Frustum[i][B] * (y + size) + m_Frustum[i][C] * (z - size) + m_Frustum[i][D] > 0)
           continue;
        if(m_Frustum[i][A] * (x - size) + m_Frustum[i][B] * (y - size) + m_Frustum[i][C] * (z + size) + m_Frustum[i][D] > 0)
           continue;
        if(m_Frustum[i][A] * (x + size) + m_Frustum[i][B] * (y - size) + m_Frustum[i][C] * (z + size) + m_Frustum[i][D] > 0)
           continue;
        if(m_Frustum[i][A] * (x - size) + m_Frustum[i][B] * (y + size) + m_Frustum[i][C] * (z + size) + m_Frustum[i][D] > 0)
           continue;
        if(m_Frustum[i][A] * (x + size) + m_Frustum[i][B] * (y + size) + m_Frustum[i][C] * (z + size) + m_Frustum[i][D] > 0)
           continue;

        // If we get here, it isn't in the frustum
        return false;
    }

    return true;
}



/**
 * Check whenever a box is in frustum.
 * @param x,y,z Center point of the box
 * @param halfwidth  Half of the width of the Box
 * @param halfheight Half of the height 
 * @param halfdepth  Half of the depth
 **/
bool nrCFrustum::BoxInFrustum( float x, float y, float z, 
			float halfwidth, float halfheight, float halfdepth ){
    for(int i = 0; i < 6; i++ )
    {
        if(m_Frustum[i][0] * (x - halfwidth) + m_Frustum[i][1] * (y - halfheight) + m_Frustum[i][2] * (z - halfdepth) + m_Frustum[i][3] > 0)
           continue;
        if(m_Frustum[i][0] * (x + halfwidth) + m_Frustum[i][1] * (y - halfheight) + m_Frustum[i][2] * (z - halfdepth) + m_Frustum[i][3] > 0)
           continue;
        if(m_Frustum[i][0] * (x - halfwidth) + m_Frustum[i][1] * (y + halfheight) + m_Frustum[i][2] * (z - halfdepth) + m_Frustum[i][3] > 0)
           continue;
        if(m_Frustum[i][0] * (x + halfwidth) + m_Frustum[i][1] * (y + halfheight) + m_Frustum[i][2] * (z - halfdepth) + m_Frustum[i][3] > 0)
           continue;
        if(m_Frustum[i][0] * (x - halfwidth) + m_Frustum[i][1] * (y - halfheight) + m_Frustum[i][2] * (z + halfdepth) + m_Frustum[i][3] > 0)
           continue;
        if(m_Frustum[i][0] * (x + halfwidth) + m_Frustum[i][1] * (y - halfheight) + m_Frustum[i][2] * (z + halfdepth) + m_Frustum[i][3] > 0)
           continue;
        if(m_Frustum[i][0] * (x - halfwidth) + m_Frustum[i][1] * (y + halfheight) + m_Frustum[i][2] * (z + halfdepth) + m_Frustum[i][3] > 0)
           continue;
        if(m_Frustum[i][0] * (x + halfwidth) + m_Frustum[i][1] * (y + halfheight) + m_Frustum[i][2] * (z + halfdepth) + m_Frustum[i][3] > 0)
           continue;

        // If we get here, it isn't in the frustum
        return false;
    }

    return true;
}

/**
 * Chack whenever an AABB is in our frustum
 * @param min, max - axis aligned bounding box corners
 * @return 1 if inside or partialy inside, 0 if outside
 **/
int nrCFrustum::AABBInside(const vec3 &min,const vec3 &max) {
	for(int i = 0; i < 6; i++) {
		if(vec4(m_Frustum[i]) * vec4(min[0],min[1],min[2],1.0) > 0.0) continue;
		if(vec4(m_Frustum[i]) * vec4(min[0],min[1],max[2],1.0) > 0.0) continue;
		if(vec4(m_Frustum[i]) * vec4(min[0],max[1],min[2],1.0) > 0.0) continue;
		if(vec4(m_Frustum[i]) * vec4(min[0],max[1],max[2],1.0) > 0.0) continue;
		if(vec4(m_Frustum[i]) * vec4(max[0],min[1],min[2],1.0) > 0.0) continue;
		if(vec4(m_Frustum[i]) * vec4(max[0],min[1],max[2],1.0) > 0.0) continue;
		if(vec4(m_Frustum[i]) * vec4(max[0],max[1],min[2],1.0) > 0.0) continue;
		if(vec4(m_Frustum[i]) * vec4(max[0],max[1],max[2],1.0) > 0.0) continue;
		return 0;
	}
	return 1;
}



/**
 * Chack whenever an AABB is in our frustum
 * @param min, max - axis aligned bounding box corners
 * @return 1 if completly inside, 0 if outside
 **/
int nrCFrustum::AABBFullInside(const vec3 &min,const vec3 &max) {
	for(int i = 0; i < 6; i++) {
		if(m_Frustum[i] * vec4(min[0],min[1],min[2],1.0) < 0.0) return 0;
		if(m_Frustum[i] * vec4(min[0],min[1],max[2],1.0) < 0.0) return 0;
		if(m_Frustum[i] * vec4(min[0],max[1],min[2],1.0) < 0.0) return 0;
		if(m_Frustum[i] * vec4(min[0],max[1],max[2],1.0) < 0.0) return 0;
		if(m_Frustum[i] * vec4(max[0],min[1],min[2],1.0) < 0.0) return 0;
		if(m_Frustum[i] * vec4(max[0],min[1],max[2],1.0) < 0.0) return 0;
		if(m_Frustum[i] * vec4(max[0],max[1],min[2],1.0) < 0.0) return 0;
		if(m_Frustum[i] * vec4(max[0],max[1],max[2],1.0) < 0.0) return 0;
	}
	return 1;
}



/**
 * Check if a given triangle lies in our frustum
 * @param v1,v2,v3 points of our triangle
 * @return true if inside or false otherwise
 **/
bool nrCFrustum::TriangleInFrustum(vec3 v1, vec3 v2, vec3 v3){
	float fa, fb, fc;
	unsigned int i;

	for (i = 0; i<6; i++)
	{
		fa=m_Frustum[i][A]*v1.x + m_Frustum[i][B]*v1.y + m_Frustum[i][C]*v1.z + m_Frustum[i][D];
		fb=m_Frustum[i][A]*v2.x + m_Frustum[i][B]*v2.y + m_Frustum[i][C]*v2.z + m_Frustum[i][D];
		if(fa*fb < 0) return true;

		fb=m_Frustum[i][A]*v2.x + m_Frustum[i][B]*v2.y + m_Frustum[i][C]*v2.z + m_Frustum[i][D];
		fc=m_Frustum[i][A]*v3.x + m_Frustum[i][B]*v3.y + m_Frustum[i][C]*v3.z + m_Frustum[i][D];
		if(fc*fb < 0) return true;

		fc=m_Frustum[i][A]*v3.x + m_Frustum[i][B]*v3.y + m_Frustum[i][C]*v3.z + m_Frustum[i][D];
		fa=m_Frustum[i][A]*v1.x + m_Frustum[i][B]*v1.y + m_Frustum[i][C]*v1.z + m_Frustum[i][D];
		if(fa*fc < 0) return true;
	}

	return false;
}
