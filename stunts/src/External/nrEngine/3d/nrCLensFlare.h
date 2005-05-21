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
 * Original code was written by  Yossarian King, 2000 (Game Programming    *
 * Gems), Electronic Arts Canada, Inc.                                     *
 * Modified by Alan Baylis 20/10/2002                                      *
 *                                                                         *
 **************************************************************************/
 
#ifndef __NR_C_LENS_FLARE_H
#define __NR_C_LENS_FLARE_H


/*-----------------------------------------------------------------------------
	Includes 
 -----------------------------------------------------------------------------*/
#include "../nrEngine.h"


/**
 * This are definitions for each type of flare texture. As you know each lens flare
 * contains of some textures which are look different. There are hexagonal form, circle,
 * rainbow ring, flare, glow and so on. 
 **/
typedef enum NR_FLARE_TYPE{
    NR_FLARE_CRCL,
    NR_FLARE_HXGN,
    NR_FLARE_RING,
    NR_FLARE_FLAR,
	NR_FLARE_PIECES,
    NR_FLARE_GLOW = NR_FLARE_PIECES,
    NR_FLARE_BACK,
    NFLARETEXTURES
}NR_FLARE_TYPE;



/**
 * This class providesd possibility to use lens flare effects in our application.
 * To use this you just bind camera and light source to this class. After that by calling
 * Render() - Function the lens flare will be rendered.
 * There is always only one lens flare for one light source and one camera. So if you want
 * to have very realistic applications you have to create for each strong light source
 * one lens flare object. So you get lens flare effects on the camera from each light source.
 * You have also to load lens flare textures before you can use lens flare effect.
 *
 * How should the describing fil of the lens flare effect looks like:
 *	- First line should contain two float values: <b><i>scaleFactor maxSize blendFactor blendTex</i></b>.
 *	<ul>
 *		<li><i>scaleFactor</i> describes the factor for adjusting overall size of flare elements.
 *		<li><i>maxSize</i> describes the max size of largest element, as proprtion of screen width.
 *		<li><i>blendFactor</i> blend factor for blending the view by looking direct in light source
 *		<li><i>blendTex</i> Texture name for blending. If no texture so write # instead. 
 *	</ul>
 *	- The following lines should contain information about flare elements like:
 *		<b><i>name dDist dSize  (a r g b)</i></b>
 *		<ul>
 *			<li><i>name</i> File name of texture with this flare element
 *			<li><i>dDist</i> Distance along ray from source (-1.0-1.0)
 *			<li><i>dSize</i> Size relative to flare envelope (-1.0-1.0)
 *			<li><i>a r g b</i> ARGB-Value for color of the element (0-255)
 *		</ul>
 * Please use this format to describe lens flare effect
 *
 * @todo Implement different flares types for different light Types. Also you can implement
 * different lens flare behaivor for different types of lens (eye, camera, underwater, ...)
 * \ingroup 3d
 */
class nrCLensFlare{
private:
	
	struct FLARE_ELEMENT_DEF{
		shared_ptr<nrCTexture> texture;
		float        fDistance;	// Distance along ray from source (0.0-1.0)
		float        fSize;		// Size relative to flare envelope (0.0-1.0)
		unsigned int argb;		// ARGB for intensity mapping
	};
	
	
	struct FLARE_DEF{
		float	fScale;		// Scale factor for adjusting overall size of flare elements.
		float	fMaxSize;	// Max size of largest element, as proportion of screen width
		int		nPieces;	// Number of elements in use	
		vector<FLARE_ELEMENT_DEF>    element;
	};


	
	FLARE_DEF       renderFlare;
	shared_ptr<nrCTexture>	gTextures[NFLARETEXTURES];            

	
	void _randomizeFlare(int nTextures,
						int nPieces,
						float fMaxSize,
						unsigned int minColour,
						unsigned int maxColour);
	void _renderFlare(float lx, float ly, int cx, int cy, float alphaFactor=1.0f);
	void _drawQuad(float,float,float,float,shared_ptr<nrCTexture>& tex,unsigned int colour );
	shared_ptr<nrCTexture> _getNthTexture( int n );
	void _cleanupLensFlare();  

	int SCREENwidth;
	int SCREENheight;
		
	vec3 lightSourcePos;
	
protected:
	shared_ptr<nrCCamera> 	camera;
	shared_ptr<nrCLight>	light;
	
public:

	// Constructor & destructor	
	nrCLensFlare();
	virtual ~nrCLensFlare();
	
	// Binding functions
	void	bindToCamera(shared_ptr<nrCCamera> cam);
	void	bindToLight(shared_ptr<nrCLight> lit);
	
	// Initializing and rendering functions
	nrResult 	loadFromFile(nrCFileSystem& vfs, const string& path, const string& fileName);
	void 		Render(float alphaFactor = 1.0f);
	void		Reset();
	void		RandomFlare();
	
	// if no light source was binded
	void	setLightSourcePos(const vec3& pos);
	
	// Loading of textures for lens flare
	nrResult loadFlareTexture(NR_FLARE_TYPE type, nrCFileSystem& vfs, const string& fileName);
	nrResult loadDefaultFlareTextures(nrCFileSystem& vfs, const string& imagePath);

	/**
	 * Set this value to 1.0 if you want that direct view into light source will blend your camera
	 * or to 0.0 if you do not have any blending of the camera. No blending gives you just the
	 * lens flare effect. For light sources that do not blend the camera completely set any value
	 * between 0.0 and 1.0
	 **/
	float blendFactor;
	
	/**
	 * Blend color is color which will blend your view by looking direct into light source
	 **/
	vec4  blendColor;
		 
	/**
	 * If <b>useZCulling</b> is set to true, so simple Z-Buffer culling will be executed before
	 * lens flare effect is rendered. Z-Culling means that we check first if the light source is 
	 * visible from our camera. THis check is done by simple comparison between existen Z-Value
	 * and calculated Z-Value of the light source. If light source is covered by other objects,
	 * so no lens flare effect occurs.
	 * @todo Implement area coverenment algorithm to test how strong is the lgith source covered
	 * by other objects to realize more realistic behaivor of the lens flare effect.
	 **/
	bool useZCulling;
	
};

#endif
