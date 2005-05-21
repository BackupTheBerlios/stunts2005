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

#ifndef __PBUFFER_H__
#define __PBUFFER_H__

/*--------------------------------------------------------------------------
 * Includes
 *------------------------------------------------------------------------*/
#include "../nrISingleton.h"
#include <boost/scoped_ptr.hpp>

using boost::scoped_ptr;


/**
 * OpenGL - Pixel Buffer (PBuffer). Can be used for offscreen rendering. So you can render your
 * geometry in the way you hav edone before but this will be done in memory and will 
 * not be displayed
 * \ingroup 3d
 **/
class nrCPBuffer : public nrISingleton<nrCPBuffer> {
private:
	struct _nrCPBuffer_data;

public:
		
	nrCPBuffer();
	nrCPBuffer(int width,int height,int flags = NR_PBUFFER_RGBA | NR_PBUFFER_DEPTH);
	~nrCPBuffer();
	
	void init (int width,int height,int flags = NR_PBUFFER_RGBA | NR_PBUFFER_DEPTH);
	void clean();
	void enable();
	void disable();
	
	int width;
	int height;
	
	bool enabled;
	
protected:
	struct scoped_ptr<_nrCPBuffer_data> data;
};

#endif /* __PBUFFER_H__ */
