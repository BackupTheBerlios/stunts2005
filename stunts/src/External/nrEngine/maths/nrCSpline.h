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
 ***************************************************************************
 *                                                                         *
 * Original code was written by  Alexander Zaprjagaev <frustum@frustum.org>*
 *                                                                         *
 **************************************************************************/

#ifndef __CSPLINE_H__
#define __CSPLINE_H__

class nrCSpline;

/**-------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include <string>
#include <boost/shared_array.hpp>
#include "nrMathlib.h"
#include "../vfs/nrVFS.h"

using std::string;
using boost::shared_array;

/**
 * nrCSpline is a class helping you creatings spline trajectories. You can for
 * example load any file containing spline points and this file will generate 
 * missing points in between. One good example is predefined camera path.
 * \ingroup math
 **/
class nrCSpline {
public:

	// Constructor & Destructor	
	nrCSpline(){};	
	virtual ~nrCSpline();
	
	nrResult loadFromFile(const string& name, nrCFileSystem& vfs, 
						int close = 0, float tension = 0.0,
						float bias = 0.0,float continuity = 0.0);
	nrResult createFromPoints(const shared_array<vec3>& points,
						int num_points,int close = 0,float tension = 0.0,
						float bias = 0.0,float continuity = 0.0);

	// Operators to get points
	vec3 operator()(float t);
	vec3 operator()(float t,float speed);
	
	// Static functions
	static void savePoints(const string& file, const shared_array<vec3>& points, int num_points);

protected:
	
	nrResult create(const shared_array<vec3>& points,int num_points,int close,
			float tension,float bias,float continuity);
	
	int num_points;
	float length;
	shared_array<vec3> params;
};

#endif 
