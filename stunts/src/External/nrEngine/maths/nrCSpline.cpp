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
 
#include "nrCSpline.h"

/**
 * Create spline from given file.
 * The file should contains points representing the spline.
 * @param name - file name
 * @param vfs - filesystem object for loading file
 * @param close - if 1 so the spline is closed. That means last point is first 
 *	poin too.
 * @param tension,bias,continuity - are parameters describing the behaivor of
 * or computed spline curve
 * @return either NR_OK or error code
 **/
nrResult nrCSpline::loadFromFile(const string& name, nrCFileSystem& vfs, 
						int close, float tension, float bias,float continuity){
						
	// set internal parameters
	num_points = 0;
	length = 0.0f;

	// Open file through our vfs 
	nrResult ret = NR_OK;
	int fileID = 0;
	
	ret = vfs.openFile(&fileID, name.c_str());
	if (ret != NR_OK){
		return ret;		
	}
	
	// Check how much points there are
	vec3 v;
	int num_points = 0;
	bool ok = true;
	while(ok){
		ret = vfs.scanRead(fileID,3,"%f %f %f",&v.x,&v.y,&v.z);
		if (ret == NR_OK){
			num_points++;
		}else{
			ok = false;
		}
	}

	// Create temporary point array	
	shared_array<vec3> points (new vec3[num_points]);
	num_points = 0;
	
	// Seek file to the beginning
	ret = vfs.seek(fileID, 0, SEEK_SET);
	if (ret != NR_OK){
		return ret;
	}
	
	// Scan all points in
	ok = true;
	while(ok){
		ret = vfs.scanRead(fileID,3,"%f %f %f",&v.x,&v.y,&v.z);
		if (ret == NR_OK){
			// XXX: will crash if the file is grown inbetween
			// TODO
			points[num_points++] = v;
		}else{
			ok = false;
		}
	}
	
	// close file, create spline and delete temporary points	
	vfs.closeFile(fileID);
	
	return create(points,num_points,close,tension,bias,continuity);
		
}


/**
 * Create spline from given points.
 * @param points - smart array containing points
 * @param num_points - count of points given in the array
 * @param close - if 1 so the spline is closed. That means last point is first 
 *	poin too.
 * @param tension,bias,continuity - are parameters describing the behaivor of
 * or computed spline curve
 * @return either NR_OK or error code
 **/
nrResult nrCSpline::createFromPoints(const shared_array<vec3>& points,
						int num_points,int close,float tension,
						float bias,float continuity){						
	return create(points,num_points,close,tension,bias,continuity);
}



/**
 * Destructor. Release used memory.
 */
nrCSpline::~nrCSpline() {
	params.reset();
}



/**
 * Calculate spline parameters by given points.
 */
nrResult nrCSpline::create(const shared_array<vec3>& points,int num_points,int close,float tension,float bias,float continuity) {

	try{	
		// allocate memory to hold parameters
		this->num_points = num_points;
		length = 0;
		
		params.reset(new vec3[num_points * 4]);
		
		for(int i = 0; i < num_points; i++) {
			vec3 prev,cur,next;
			if(i == 0) {
				if(close) prev = points[num_points - 1];
				else prev = points[i];
				cur = points[i];
				next = points[i + 1];
			} else if(i == num_points - 1) {
				prev = points[i - 1];
				cur = points[i];
				if(close) next = points[0];
				else next = points[i];
			} else {
				prev = points[i - 1];
				cur = points[i];
				next = points[i + 1];
			}
			vec3 p0 = (cur - prev) * (1.0f + bias);
			vec3 p1 = (next - cur) * (1.0f - bias);
			vec3 r0 = (p0 + (p1 - p0) * 0.5f * (1.0f + continuity)) * (1.0f - tension);
			vec3 r1 = (p0 + (p1 - p0) * 0.5f * (1.0f - continuity)) * (1.0f - tension);
			params[i * 4 + 0] = cur;
			params[i * 4 + 1] = next;
			params[i * 4 + 2] = r0;
			if(i) params[i * 4 - 1] = r1;
			else params[(num_points - 1) * 4 + 3] = r1;
			length += (next - cur).length();
		}
		for(int i = 0; i < num_points; i++) {
			vec3 p0 = params[i * 4 + 0];
			vec3 p1 = params[i * 4 + 1];
			vec3 r0 = params[i * 4 + 2];
			vec3 r1 = params[i * 4 + 3];
			params[i * 4 + 0] = p0;
			params[i * 4 + 1] = r0;
			params[i * 4 + 2] = -p0 * 3.0f + p1 * 3.0f - r0 * 2.0f - r1;
			params[i * 4 + 3] = p0 * 2.0f - p1 * 2.0f + r0 + r1;
		}
	
	}catch(...){
		return NR_UNKNOWN_ERROR;
	}
	
	return NR_OK;
	
}

/**
 * Get curve value of our spline on certain point.
 * @param t - point in our curve.
 * @return f(t)
 * @return point on the curve
 */
vec3 nrCSpline::operator()(float t) {
	if(!num_points) return vec3(0,0,0);
	t *= num_points;
	int i = (int)t;
	t -= i;
	i = (i % num_points) * 4;
	float t2 = t * t;
	float t3 = t2 * t;
	return params[i + 0] + params[i + 1] * t + 
			params[i + 2] * t2 + params[i + 3] * t3;
}


/**
 * Same as operator()(float t) but here you can give also the sampling
 * speed. Give here as a speed the frame interval to get constant
 * sampling of spline points on different machines.
 * @param t - sampling point
 * @param speed - sampling speed
 * @return point on the curve
 */
vec3 nrCSpline::operator()(float t,float speed) {
	return operator()(t * speed / length);
}


/**
 * Just save given points in file which can be used later.
 * @param file - file name 
 * @param points smart array containing point
 * @param num_points - number of given points
 **/
void nrCSpline::savePoints(const string& file, const shared_array<vec3>& points,
					 int num_points){
	
	FILE * f = fopen (file.c_str(), "w");
	if (f == NULL) return;	
	
	for (int i=0; i < num_points; i++){
		fprintf(f, "%f %f %f\n", points[i].x, points[i].y, points[i].z);	
	}
	
	fclose (f);
}
