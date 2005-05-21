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

 
#ifndef _NR_C_CAMERA_H_
#define _NR_C_CAMERA_H_

/*------------------------------------------------------------------------------
 Forward declaration
 -----------------------------------------------------------------------------*/
class nrCCamera;
	
/*------------------------------------------------------------------------------
 Includes to complete declaration of this class
 -----------------------------------------------------------------------------*/
#include "../nrEngine.h"

#define FRUSTUM_STACK_LENGTH 8

/**
 * cCamera - This is our camera class. It provides support for frustum culling. Also it 
 * has support for stereo view. Stereo view we are using is called passive stereo. 
 * This camera class also supports two kind of farPlane definition. You can define farPlane
 * and also viewing distance. By calling setUp() the projection matrix of frsutum with
 * far plane at viewing distance will be calculated. This will be used for frustum culling.
 * farPlane - property defines the standard far plane after that OpenGL do his clipping.
 * If you didn't understand for what you can use it, just define this two properties with same values.
 *
 * This class also supports frustum stack. That means you can push current frustum on the stack
 * and work with another one. After your work is done (e.g. offscreen rendering) you restore the old
 * frustum from the stack. The storing and restoring produce no extra costs O(1).
 * \ingroup 3d
 */
class nrCCamera{
protected:
				
	// Frustum
	shared_ptr<nrCFrustum> frustum[FRUSTUM_STACK_LENGTH];
	int frustumStackPos;
	
	// Properties of our viewing frustum
	int viewPortWidth;
	int viewPortHeight;
	float nearPlane;
	float fieldOfView;
	float farPlane;
	float viewingDistance;
		
	// current matrices
	mat4 modelview;
	mat4 projection;
	mat4 viewingDistProjection;
	
	// Camera orientation
	vec3 m_vPosition;
	vec3 m_vView;
	vec3 m_vUpVector;
	
	// camera to world orientation (usefull for billboards)	
	vec3 m_DirectionVector;
	vec3 m_RightVector;
	vec3 m_UpVector;
		
	// Stereo-View
	bool	useStereo;
	bool	currentView;				// Current view Left = 0, Right = 1;
	float	halfEyeSeparation;
	mat4	asymmetricProjectionLeft;		// Frustum for Left Eye  (Projection Matrix)
	mat4	asymmetricProjectionRight;		// 		   for Right Eye (Projection Matrix)
	bool	widthHalfed;
	float 	eyeSeparation, focalLength;
	
public:

    // Our camera constructor
    nrCCamera();
    ~nrCCamera();

#if 0
	/**
	 * This will bind the camera to specified render context object. We need to bind
	 * the camera on such a object, to fix the rendering to the specified context(window).
	 * If you want to use same camera for different objects, just use more cameras
	 * or reset camera to new context
	 * @param smRenderContext Smart pointer containing the render context object
	 **/
	void setRenderContext(shared_ptr<nrIRenderContext> smRenderContext){
		rcObj = smRenderContext;
	}
#endif
	
	/**
	 * Returns projection matrix of the left eye which is used for stereo view.
	 * @return mat4 as projection matrix
	 **/
	const mat4& getLeftEyeProjection(){return asymmetricProjectionLeft;}
	
	
	/**
	 * Returns projection matrix of the right eye which is used for stereo view.
	 * @return mat4 as projection matrix
	 **/
	const mat4& getRightEyeProjection(){return asymmetricProjectionRight;}
	
	/**
	 * Say if stereo mode is active now.
	 * @return true if mode is active, otherwise false
	 **/
	bool 		isUsingStereoView(){return useStereo;}
	
	/**
	 * Get eye separation
	 * @return eye separation in meters (e.g. 0.04m)
	 **/
	float 		getEyeSeparation(){return eyeSeparation;}
	
	/**
	 * Get focal length
	 * @return focal length in meters (e.g. 4.0m)
	 **/
	float 		getFocalLength() {return focalLength;}
	
	/**
	 * Say if we have right camera mode active now. If this function returns <b>true</b>,so we are
	 * using projection matrix for camera placed on the right eye. Otherwise returns <b>false</b>.
	 * <b>False</b> is also returned if we are using <b>Mono</b>-Mode. 
	 * @return eye separation in meters (e.g. 0.04m)
	 **/	
	bool 		isUsingRightEye(){return currentView;}
	
	void 		calculateEyeProjections(float eyeSep, float focLength);
	void 		calculateEyeProjections(){calculateEyeProjections(eyeSeparation, focalLength);}
	void 		setUseStereo(bool b);
	void 		activateLeftEye(bool setNewVieport = true);
	void 		activateRightEye(bool setNewVieport = true);
	void 		deactivateEyeViews(bool setNewVieport = true);
	
    
	
	/**
	 * Returns projection matrix of camera. If we are using stereo mode, so use other functions.
	 * @return mat4 as projection matrix
	 **/
	const mat4& getProjection(){return projection;}
	
	/**
	 * Returns modelview matrix of our camera
	 * @return mat4 as modelview matrix
	 **/
    const mat4& getModelview(){return modelview;}

		
    // Frustum
    shared_ptr<nrCFrustum> 	getFrustum();
    void 					pushFrustum();
    void 					calculateFrustum();
    void 					popFrustum();
	
	
    // Distance & Viewport
    void 	setViewingDistance (float d);
    float 	getViewingDistance (){return viewingDistance;}
    void 	setFOV (float f);
    float 	getFOV(){return fieldOfView;}
    
	
	/**
	 * Set width of used viewport.
	 * @param w width of viewport
	 **/
	void 	setWidth(int w){viewPortWidth = w;}
    int 	getWidth(){return viewPortWidth;}
    
	
	/**
	 * Set height of used viewport.
	 * @param h height of viewport
	 **/
	void 	setHeight(int h){viewPortHeight = h;}
    int 	getHeight(){return viewPortHeight;}
    
	
    /**
	 * Get position of our camera in our 3D-Space.
	 **/
	const vec3& getPosition() 	{   return m_vPosition;     }
    
	/**
	 * Returns look at point of camera. 
	 **/
	const vec3& getView()     	{   return m_vView;         }
    
	/**
	 * Returns up vector from the used camera. 
	 **/
	const vec3& getUp() 		{   return m_vUpVector;     }

	
	    
	/**
	 * Returns view direction of our camera. View direction is the same as
	 * negative z-axis (for OpenGL) of camerea's modelview basis matrix 
	 **/
    const vec3& getViewDir()  {   return m_DirectionVector;     }
    
	/**
	 * Returns direction to the right. Right direction is the same as
	 * positive x-axis (for OpenGL) of camerea's modelview basis matrix 
	 **/	
	const vec3& getRightDir() {   return m_RightVector;         }
    
	/**
	 * Returns direction to the top. Top direction is the same as
	 * positive y-axis (for OpenGL) of camerea's modelview basis matrix 
	 **/	
	const vec3& getUpDir()    {   return m_UpVector;     		}

	    
    void setup();
	void calculateDirections();
    
    // This changes the position, view, and up vector of the camera.
    // This is primarily used for initialization
    void setCamera(float positionX, float positionY, float positionZ,
                        float viewX,     float viewY,     float viewZ,
                        float upVectorX, float upVectorY, float upVectorZ);

		
    void setCamera(const vec3& pos,const vec3& view,const vec3& up){
    		setCamera(pos.x, pos.y, pos.z, view.x,view.y,view.z, up.x,up.y,up.z);}

    // This will position camera
    void setCameraByDirection (float posX, float posY, float posZ,
          float dirX, float dirY, float dirZ,
          float upVectorX, float upVectorY, float upVectorZ);
	
    // Position camera
    void moveTo (const vec3& pos){setCamera(pos,m_vView,m_vUpVector);}	
    void moveTo (float posX, float posY, float posZ){
    		setCamera(vec3(posX,posY,posZ),m_vView,m_vUpVector);} 
    
    // Set view of the camera
    void lookAt (const vec3& view){setCamera(m_vPosition, view, m_vUpVector);}
    void lookAt (float x,float y,float z){setCamera(m_vPosition,vec3(x,y,z), m_vUpVector);}
    
    // Roll camera
    void changeUp (const vec3& up){setCamera(m_vPosition, m_vView, up);}
    void changeUp (float x,float y,float z){setCamera(m_vPosition,m_vView,vec3(x,y,z));}
    void roll(float angle);
	
    // This rotates the camera's view around the position depending on the values passed in.
    void rotateView(float angle, float X, float Y, float Z);

    // This moves the camera's view by the mouse movements (First person view)
    void setViewByMouse(nrIFramework& frameWork);

    // This rotates the camera around a point (I.E. your character).
    void rotateAroundPoint(vec3 vCenter, float angle, float X, float Y, float Z);

    // This strafes the camera left or right depending on the speed (+/-)
    void strafe(float speed);

    // This will move the camera forward or backward depending on the speed
    void move(float speed);

    // This updates the camera's view and other data (Should be called each frame)
    void update();
    
	// Neccessary stuff to get Z-Value of Depth-Buffer in certain point
	float calcZValue(const vec3& point);

private:
    // This uses gluLookAt() to tell OpenGL where to look
    void Look();
	
};

#endif
