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
 
#include "nrCLensFlare.h"


/*-----------------------------------------------------------------------------
	Defines 
 -----------------------------------------------------------------------------*/
#define FLARE_RANGE(A,B)    ( (rand()%((B)-(A)+1)) + (A) )
#define FLARE_FRANGE(A,B)   ( ((float)(rand()&0xffffff)/(float)0xfffffe)*((B)-(A)) + (A) )

#define MAKEID(a,b,c,d)     (((a)<<24) | ((b)<<16) | ((c)<<8) | ((d)<<0))

// Get height from width, assuming certain aspect ratio.
#define HEIGHTFROMWIDTH(w)  ((w)*SCREENheight/SCREENwidth)

#define isqrt(x)        (int)((double)(x))

#define FLARE_MINCOLOUR        MAKEID(60, 255, 200, 0)
#define FLARE_MAXCOLOUR        MAKEID(220, 235, 235, 64)
#define FLARE_MAXSIZE                   0.2f

#define FLARE_MAXELEMENTSPERFLARE         15
#define FLARE_MINELEMENTSPERFLARE         8

/**
 * Construtor
 * Initialize data with default values
 **/
nrCLensFlare::nrCLensFlare(){
	blendFactor = 1.0f;
	blendColor.r = 1.0f;
	blendColor.g = 1.0f;
	blendColor.b = 1.0f;
	blendColor.a = 1.0f;
	
	useZCulling = true;
}


/**
 * Destructor
 * Delete all data and free memory if needed
 **/
nrCLensFlare::~nrCLensFlare(){
	_cleanupLensFlare();  
}


/**
 * Generate a random lens flare.
 **/
void nrCLensFlare::_randomizeFlare(
                        int nTextures,
                        int nPieces,
                        float fMaxSize,
                        unsigned int minColour,
                        unsigned int maxColour)
{

	// local variables
    int             i;
    float           fFracDist;
    float           fEnvelopeSize;
    FLARE_ELEMENT_DEF *element;

	srand((unsigned)time(NULL));

    // Initialize flare with requested number of elements, with
    // random placement, colour, and size of elements.
    renderFlare.nPieces = nPieces;
    renderFlare.fScale = fMaxSize;
    renderFlare.fMaxSize = fMaxSize;
    fFracDist = 1.0f/(float)(renderFlare.nPieces - 1);
    
	// generate for each flare part 
    for (i = 0; i < renderFlare.nPieces; ++i){
        element = &renderFlare.element[i];
        element->fDistance = (fFracDist*i) + FLARE_FRANGE(0,fFracDist);

        // Envelope size is maximum at ends of line, minimum in the middle (i.e. two
        // cones, touching at the tips).
        fEnvelopeSize = (float)fabs(1.0f - 2*element->fDistance);

        element->fSize = FLARE_FRANGE(0.6f, 1.0f) * fEnvelopeSize;
        element->argb = FLARE_RANGE(minColour & 0xff000000, maxColour & 0xff000000)
					 | FLARE_RANGE(minColour & 0x00ff0000, maxColour & 0x00ff0000)
					 | FLARE_RANGE(minColour & 0x0000ff00, maxColour & 0x0000ff00)
					 | FLARE_RANGE(minColour & 0x000000ff, maxColour & 0x000000ff);
        element->texture = _getNthTexture( FLARE_RANGE(0, nTextures - 1) );
    }
	
}



/**
 * Draw lens flare with specified (lx,ly) light position
 * in screen coordinates, for given (cx,cy) position of
 * center of screen.
 **/
void nrCLensFlare::_renderFlare(float lx, float ly, int cx, int cy, float alphaFactor){
    
	float     dx, dy;          // Screen coordinates of "destination"
    float     px, py;          // Screen coordinates of flare element
    float     maxflaredist, flaredist, flaremaxsize, flarescale;
    float     width, height, alpha;    // Piece parameters;
    int     i;
    FLARE_ELEMENT_DEF    *element;

    // Compute how far off-center the flare source is.
    maxflaredist = isqrt(cx*cx + cy*cy);
    flaredist = isqrt((lx - cx)*(lx - cx)+
                      (ly - cy)*(ly - cy));
    flaredist = (maxflaredist - flaredist);
    flaremaxsize = (int)(SCREENwidth * renderFlare.fMaxSize);
    flarescale = (int)(SCREENwidth * renderFlare.fScale);

    // Destination is opposite side of centre from source
    dx = cx + (cx - lx);
    dy = cy + (cy - ly);

    // Render each element.
    for (i = 0; i < renderFlare.nPieces; ++i){
        element = &renderFlare.element[i];

        // Position is interpolated along line between start and destination.
        px = ((1.0f - element->fDistance)*lx + element->fDistance*dx);
        py = ((1.0f - element->fDistance)*ly + element->fDistance*dy);

        // Piece size are 0 to 1; flare size is proportion of
        // screen width; scale by flaredist/maxflaredist.
        width = ((flaredist * flarescale * element->fSize) / maxflaredist);

        // Width gets clamped, to allows the off-axis flares
        // to keep a good size without letting the elements get
        // too big when centered.
        if (width > flaremaxsize){
            width = flaremaxsize;
        }

        // Flare elements are square (round) so height is just
        // width scaled by aspect ratio.
        height = HEIGHTFROMWIDTH(width);
        alpha = (flaredist * float(element->argb >> 24))/maxflaredist;
		alpha = (alpha* alphaFactor);
	    if ( width > 1 ){
            unsigned int argb = ((int)alpha << 24 )|(element->argb & 0x00ffffff);

            _drawQuad(px - width/2, py - height/2,
					width, height, element->texture, argb );
        }
    }
}


/**
 * Used to draw individual elements of the lens flare
 * in 2D.
 **/
void nrCLensFlare::_drawQuad( float x, float y, float width, float height,
					 shared_ptr<nrCTexture>& tex, unsigned int colour )
{
	if (tex.get()){
		tex->enable();
    	tex->bind();
    }
	
    glBegin(GL_QUADS);

    glColor4ub( (GLbyte)(colour >> 16 & 0xff),
                (GLbyte)(colour >>  8 & 0xff),
                (GLbyte)(colour >>  0 & 0xff),
                (GLbyte)(colour >> 24 & 0xff) );

    glTexCoord2f( 0.0f, 0.0f );
    glVertex3f( x, y, 1.0);
    glTexCoord2f( 1.0f, 0.0f );
    glVertex3f( x + width, y, 1.0);
    glTexCoord2f( 1.0f, 1.0f );
    glVertex3f( x + width, y + height, 1.0);
    glTexCoord2f( 0.0f, 1.0f );
    glVertex3f( x, y + height, 1.0);

    glEnd();
}


/**
 * Returns the texture according to given number
 **/
shared_ptr<nrCTexture> nrCLensFlare::_getNthTexture( int n ){

    if ( ( n < 0 ) || ( n >= NFLARETEXTURES ) )
        return shared_ptr<nrCTexture>();

    return gTextures[n];
}


/**
 * Load default flare textures from given image directory.
 * There must be followed files: 
 * <b>crcl.tga, hxgn.tga, ring.tga, flar.tga, glow.tga, back.tga</b>
 * Each of this file will be loaded.
 * @param vfs - virtual filesystem from which files should be loaded
 * @param imagePath - directory name where this files can be found
 * @return either NR_OK or error code from nrCTexture::loadFromFile()
 **/ 
nrResult nrCLensFlare::loadDefaultFlareTextures(nrCFileSystem& vfs, const string& imagePath){

	string filename[6];
	filename[0] = "/crcl.tga";
	filename[1] = "/hxgn.tga";
	filename[2] = "/ring.tga";
	filename[3] = "/flar.tga";
	filename[4] = "/glow.tga";
	filename[5] = "/back.tga";  
      
	// reset all textures
	for (int i = 0; i < NFLARETEXTURES; i++){
		gTextures[i].reset(new nrCTexture());
	}
	
	// load all textures	
	for (int i = 0; i < NFLARETEXTURES; i++){
		nrResult ret = loadFlareTexture((NR_FLARE_TYPE)i, vfs, 
										imagePath + filename[i]);			
		if (ret != NR_OK)
			return ret;
	}
	
	return NR_OK;
}


/**
 * Load flare texture of certain type from file.
 * @param type - Type of the flare texture
 * @param vfs - virtual filesystem from which to load
 * @param fileName - file name of the texture file
 * @return either NR_OK or error code from nrCTexture::loadFromFile()
 **/
nrResult nrCLensFlare::loadFlareTexture(NR_FLARE_TYPE type, nrCFileSystem& vfs, 
			const string& fileName){
		
	// reset texture
	gTextures[type].reset(new nrCTexture());
	
	// load texture
	return gTextures[type]->loadFromFile(fileName, 
							NR_TEXTURE_2D, NR_RGBA | NR_LINEAR | NR_CLAMP);

}


/**
 * Load file with definitions for lens flare.
 * @param vfs - virtual file systems
 * @param path - path containing the file
 * @param filename - name of the file with definitions
 * @return either NR_OK or error code
 * @note If file contains any texture names, so this name should be relative 
 * to <b>path</b>
 **/
nrResult nrCLensFlare::loadFromFile(nrCFileSystem& vfs, 
			const string& path, const string& filename ){
	
	// local variables
    int     	fileID;
	nrResult 	ret = NR_OK;
	char        name[32] = { '\0', };	
    memset( &renderFlare, 0, sizeof( FLARE_DEF ) );

	// open file for reading
	ret = vfs.openFile(&fileID, (path + filename).c_str());
    if ( ret == NR_OK ){
		
		// read data from file
		ret = vfs.scanRead(fileID, 4, "%f %f %f %s", 
			&renderFlare.fScale, &renderFlare.fMaxSize, &blendFactor, name );
		if (ret != NR_OK) return ret;
		
		// load texture for backblending
		if (strlen(name) > 1 && name[0] != '#'){
			gTextures[NR_FLARE_BACK].reset(new nrCTexture());
			ret = gTextures[NR_FLARE_BACK]->loadFromFile(path + name, 
						NR_TEXTURE_2D, NR_RGBA | NR_LINEAR | NR_CLAMP);
			if (ret != NR_OK){
				return ret;
			}		
		}
			
		// read until we have data to read
        while ( ret == NR_OK ){
            // define some more variables
            double          dDist = 0.0, dSize = 0.0;
            unsigned int    a = 0, r = 0, g = 0, b = 0;

            // scan data from file
			ret = vfs.scanRead(fileID, 7, "%s %lf %lf ( %d %d %d %d )", 
								name, &dDist, &dSize, &a, &r, &g, &b );
			
			// read was sucessfull, so store data
			if (ret == NR_OK){
				FLARE_ELEMENT_DEF temp;
				
				temp.fDistance = (float)dDist;
                temp.fSize = (float)dSize;
                temp.argb = ( a << 24 ) | ( r << 16 ) | ( g << 8 ) | ( b << 0 );
				temp.texture.reset(new nrCTexture());
				ret = temp.texture->loadFromFile(path + name, 
						NR_TEXTURE_2D, NR_RGBA | NR_LINEAR | NR_CLAMP);
				if (ret != NR_OK) return ret;
				
				renderFlare.element.push_back(temp);
			}
        }

        renderFlare.nPieces = renderFlare.element.size();

        vfs.closeFile(fileID);
    
	// File could not be opened	
	}else{
		return NR_FILE_NOT_FOUND;
	}
	
	return NR_OK;
}


/**
 * Generate random flare.
 **/
void nrCLensFlare::RandomFlare(){
	_randomizeFlare(NR_FLARE_PIECES, 
			FLARE_RANGE(FLARE_MINELEMENTSPERFLARE, FLARE_MAXELEMENTSPERFLARE),
			FLARE_MAXSIZE, 
			FLARE_MINCOLOUR, FLARE_MAXCOLOUR);
}


/**
 * Bind camera object to this lens flare effect
 * @param cam - smart pointer containing camera object
 **/
void nrCLensFlare::bindToCamera(shared_ptr<nrCCamera> cam){
	camera = cam;
}
	


/**
 * Bind light source object to this lens flare effect
 * @param lit - smart pointer containing light object
 **/
void nrCLensFlare::bindToLight(shared_ptr<nrCLight> lit){
	light = lit;
}


/**
 * If no light source object was binded you can define the position of the light 
 * object here. 
 **/
void nrCLensFlare::setLightSourcePos(const vec3& pos){
	lightSourcePos = pos;
}


/**
 * Render the Lens flare effect on the screen.
 * @param alphaFactor - here you can define the transparency of flares.
 * @note If camera and light source is not binded, so no effect will be rendered
 **/
void nrCLensFlare::Render(float alphaFactor){

	if (camera.get()== NULL) return;
		
	GLdouble model[16], proj[16];
	GLint viewport[4];
	GLdouble winx, winy, winz;
	
	// get OpenGL's state
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev (GL_MODELVIEW_MATRIX, model);
	glGetDoublev (GL_PROJECTION_MATRIX, proj);

	SCREENwidth = camera->getWidth();
  	SCREENheight = camera->getHeight();

	// get position of the light source 
	vec3 pos;
	if (light.get() != NULL){
		pos = light->getPosition();
	}else{
		pos = lightSourcePos;
	}
		

	// check where the light source will be found on the screen
	gluProject (pos.x, pos.y, pos.z, model, proj,
				 viewport, &winx, &winy, &winz);

	// if the effect lies not in our view frustum, so no effect 
	if ((int)winx > viewport[2]+viewport[0] || (int)winy > viewport[3]+viewport[1] || 
		(int)winx < viewport[0] || (int)winy < viewport[1]) return;
	

	// if ZCulling is activated, so compare the values
	if (useZCulling){
		// get ZValue
		float oldZ = 0.0f;
		float newZ = camera->calcZValue(pos);
		glReadPixels((int)winx, (int)winy, 1, 1, 
					GL_DEPTH_COMPONENT, GL_FLOAT, &oldZ);
					
		// get current depth comparison state
		int value = 0;
		glGetIntegerv(GL_DEPTH_FUNC, &value);		
		
		// do clipping according to the state
		switch (value){
			case GL_EQUAL:if (newZ == oldZ)break;return;
			case GL_NOTEQUAL: if(newZ != oldZ)break;return;
			case GL_LEQUAL: if(newZ <= oldZ)break;return;
			case GL_LESS: if (newZ < oldZ)break;return;
			case GL_GEQUAL: if(newZ >= oldZ)break;return;
			case GL_GREATER: if (newZ > oldZ)break;return;
			case GL_NEVER: return;
			case GL_ALWAYS: break;
			default: return;
		}
	}
	
	
	GLdouble SunPosition[2];	
	SunPosition[0] = winx;
	SunPosition[1] = winy;
	
	int blendSrc,blendDst;
	glGetIntegerv(GL_BLEND_SRC, &blendSrc);
	glGetIntegerv(GL_BLEND_DST, &blendDst);
	
	// Set the correct blending mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	//  Set for orthographic projection.
	nrOrthoMode(viewport[0],viewport[1]+viewport[3], viewport[0]+viewport[2], viewport[1]);
	
	nrPushOffLit();
	nrPushOffDepthRead();
	nrPushOffDepthWrite();
	nrPushOffCullFace();
	nrPushOnBlend();
	
  	{
		// Calculate alpha value for sun glare
		int position[2];
		position[1] = abs(viewport[0] + (int)SunPosition[0] - (viewport[2]+viewport[0])/2);
		position[2] = abs(viewport[1] + (int)SunPosition[1] - (viewport[3]+viewport[1])/2);
    		float glare_distance = sqrt((double)(position[1] * position[1] + 
						position[2] * position[2]));
		float alpha = 1.0 - glare_distance / 500.0;
		alpha *= alphaFactor;
		vec4 blending = blendColor;
		blending.w = alpha * blendFactor;
		
		// Render sun glare
		if (gTextures[NR_FLARE_BACK]){
			gTextures[NR_FLARE_BACK]->enable();
			gTextures[NR_FLARE_BACK]->bind();
		}
		glBegin(GL_QUADS);
			glColor4fv(blending);
			glTexCoord2f( 0.0f, 0.0f );
			glVertex3f(0, 0, 1.0);
			glTexCoord2f( 1.0f, 0.0f );
			glVertex3f(camera->getWidth(), 1, 1.0);
			glTexCoord2f( 1.0f, 1.0f );
			glVertex3f(camera->getWidth(), camera->getHeight(), 1.0);
			glTexCoord2f( 0.0f, 1.0f );
			glVertex3f(0, camera->getHeight(), 1.0);
   		glEnd();
        
    		_renderFlare(SunPosition[0], SunPosition[1], 
				viewport[0]+SCREENwidth/2, viewport[1]+SCREENheight/2, alphaFactor);
  	}

	nrPopBlend();
	nrPopCullFace();
	nrPerspectiveMode();
	nrPopDepthRead();
	nrPopDepthWrite();
  	nrPopLit();
	
	// set mode back
	glBlendFunc(blendSrc, blendDst);
}


/**
 * Reset the lens flare by releasing the memory and generating new
 * lens flare effect
 **/
void nrCLensFlare::Reset(){
	RandomFlare();
}


/**
 * Release memory
 **/
void nrCLensFlare::_cleanupLensFlare(){
	for (int loop = 0; loop < NFLARETEXTURES; loop++){
		gTextures[loop].reset();
	}
}

