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


#include "nrCCamera.h"


/**
 * Constructor of camera object.
 **/
nrCCamera::nrCCamera(){
    m_vUpVector = vec3(0,1,0);

    viewPortWidth = 1;
    viewPortHeight = 1;
    nearPlane = 0.1;
    farPlane = 20000.0f;
    fieldOfView = 65.0f;
   
	frustumStackPos = 0;   
	
	eyeSeparation = 0.04f;
	focalLength = 1.0f;
	
	useStereo = false;
	
	// initialize frustum objects
	for (int i=0; i < FRUSTUM_STACK_LENGTH; i++)
		frustum[i].reset(new nrCFrustum());
		
}

nrCCamera::~nrCCamera(){
	// clean frustum objects
	for (int i=0; i < FRUSTUM_STACK_LENGTH; i++)
		frustum[i].reset();

}


/**
 * Returns currently used frustum object
 **/
shared_ptr<nrCFrustum> nrCCamera::getFrustum(){
	return frustum[frustumStackPos];
}


/**
 * Initialize OpenGL-State-Machine with our Camera settings
 * You have to call this function after you'd created window and setted all 
 * camera variables
 **/
void nrCCamera::setup(){
  
	// Check if we have allowed height
	if (viewPortHeight <= 0)
		viewPortHeight = 1;
	
	glViewport(0,0,viewPortWidth, viewPortHeight);
	
	// Setup projection with field of view 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Calculate projection matrix with viewingDistance as farplane
	gluPerspective(fieldOfView, 
			(GLfloat)viewPortWidth / (GLfloat)viewPortHeight, 
			nearPlane, viewingDistance);
	glGetFloatv(GL_PROJECTION_MATRIX, viewingDistProjection);
	glLoadIdentity();
	
	// calculate projection matrix with farPlane as far plane
	gluPerspective(fieldOfView, 
			(GLfloat)viewPortWidth / (GLfloat)viewPortHeight,nearPlane, farPlane);
	glGetFloatv(GL_PROJECTION_MATRIX, projection);
	
	// go to normal matrix mode
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}



/**
 * set camera viewing distance (NOTE: not OpenGL's far clipping plane)
 * @param d is the viewing distance
 **/
void nrCCamera::setViewingDistance(float d){
  viewingDistance = d;
}


/**
 * set camera's field of view (in degrees)
 * @param f open angle for our view
 * @note if stereo mode is active, so recalculation of projection
 * matricies will be done.
 **/
void nrCCamera::setFOV (float f){
	fieldOfView = f;
	if(useStereo)calculateEyeProjections(eyeSeparation,focalLength);
}


/**
 * position camera and orient it in given direction.
 * @param posX,posY,posZ - new position of camera
 * @param dirX,dirY,dirZ - new viewing direction of our camera
 * @param upVectorX,upVectorY,upVectorZ - up vector of our camera
 **/
void nrCCamera::setCameraByDirection (float posX, float posY, float posZ,
          float dirX, float dirY, float dirZ,
          float upVectorX, float upVectorY, float upVectorZ){

  // neue Blickposition
  setCamera (posX, posY, posZ, posX+dirX, posY+dirY, posZ+dirZ, 
  			upVectorX, upVectorY, upVectorZ);

}



/**
 * position and orient camera in the 3D-Space
 * @param positionX,positionY,positionZ - new position of the camera
 * @param viewX,viewY,viewZ - Look at point for our camera
 * @param upVectorX,upVectorY,upVectorZ - up vector of our view
 **/
void nrCCamera::setCamera(float positionX, float positionY, float positionZ,
                             float viewX,     float viewY,     float viewZ,
                             float upVectorX, float upVectorY, float upVectorZ)
{
    m_vPosition = vec3(positionX, positionY, positionZ);
    m_vView     = vec3(viewX, viewY, viewZ);
    m_vUpVector = vec3(upVectorX, upVectorY, upVectorZ);
}


/**
 * This allows us to look around using the mouse, like in most first person games.
 * @param frameWork - is the framework interface implementing mouse quering methods
 **/
void nrCCamera::setViewByMouse(nrIFramework& frameWork){

    int mouseX,mouseY;                              
	int middleX = frameWork.getRC()->getWindowWidth() >> 1;
    int middleY = frameWork.getRC()->getWindowHeight() >> 1;
	float angleY = 0.0f;
	float angleZ = 0.0f;                            
    static float currentRotX = 0.0f;

    // Get the mouse's current X,Y position
    frameWork.getCursorPosition(&mouseX, &mouseY);

    // If our cursor is still in the middle, we never moved... so don't update the screen
    if( (mouseX == middleX) && (mouseY == middleY) ) return;

    // Set the mouse position to the middle of our window
    frameWork.setCursorPosition(middleX, middleY);

    // Get the direction the mouse moved in, but bring the number down to a reasonable amount
    angleY = (float)( (middleX - mouseX) ) / 10.0f;
    angleZ = (float)( (middleY - mouseY) ) / 10.0f;

    // Here we keep track of the current rotation (for up and down) so that
    // we can restrict the camera from doing a full 360 loop.
    currentRotX -= angleZ;

    // If the current rotation (in radians) is greater than 1.0, we want to cap it.
    /*if(currentRotX > 10.0f)
        currentRotX = 10.0f;
    // Check if the rotation is below -1.0, if so we want to make sure it doesn't continue
    else if(currentRotX < -10.0f)
        currentRotX = -10.0f;
    // Otherwise, we can rotate the view around our position
    else*/
    {
        // To find the axis we need to rotate around for up and down
        // movements, we need to get a perpendicular vector from the
        // camera's view vector and up vector.  This will be the axis.
        vec3 vAxis = cross(m_vView - m_vPosition, m_vUpVector);
        vAxis = normalize(vAxis);

        // Rotate around our perpendicular axis and along the y-axis
        rotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
        rotateView(angleY, 0, 1, 0);

    }
}


/**
 * This rotates the position around certain point
 * @param vCenter - center of rotation
 * @param angle - angle of rotation
 * @param x,y,z - axis of rotation 
 **/
void nrCCamera::rotateAroundPoint(vec3 vCenter, float angle, float x, float y, float z){
    
	vec3 vNewPosition;
	
	// To rotate our position around a point, what we need to do is find
	// a vector from our position to the center point we will be rotating around.
	// Once we get this vector, then we rotate it along the specified axis with
	// the specified degree.  Finally the new vector is added center point that we
	
	// rotated around (vCenter) to become our new position.  That's all it takes.
	
	// Get the vVector from our position to the center we are rotating around
	vec3 vPos = m_vPosition - vCenter;
	
	// Calculate the sine and cosine of the angle once
	float cosTheta = (float)cos(TORAD(angle));
	float sinTheta = (float)sin(TORAD(angle));
	
	// Find the new x position for the new rotated point
	vNewPosition.x  = (cosTheta + (1 - cosTheta) * x * x)        * vPos.x;
	vNewPosition.x += ((1 - cosTheta) * x * y - z * sinTheta)    * vPos.y;
	vNewPosition.x += ((1 - cosTheta) * x * z + y * sinTheta)    * vPos.z;
	
	// Find the new y position for the new rotated point
	vNewPosition.y  = ((1 - cosTheta) * x * y + z * sinTheta)    * vPos.x;
	vNewPosition.y += (cosTheta + (1 - cosTheta) * y * y)        * vPos.y;
	vNewPosition.y += ((1 - cosTheta) * y * z - x * sinTheta)    * vPos.z;
	
	// Find the new z position for the new rotated point
	vNewPosition.z  = ((1 - cosTheta) * x * z - y * sinTheta)    * vPos.x;
	vNewPosition.z += ((1 - cosTheta) * y * z + x * sinTheta)    * vPos.y;
	vNewPosition.z += (cosTheta + (1 - cosTheta) * z * z)        * vPos.z;
		
	// Now we just add the newly rotated vector to our position to set
	// our new rotated position of our camera.
	m_vPosition = vCenter + vNewPosition;
}



/**
 * This rotates the view around the position using an axis-angle rotation
 * @param angle - angle of rotation
 * @param x,y,z axis of rotation
 **/
void nrCCamera::rotateView(float angle, float x, float y, float z){

    vec3 vNewView;

    // Get the view vector (The direction we are facing)
    vec3 vView = m_vView - m_vPosition;

    // Calculate the sine and cosine of the angle once
    float cosTheta = (float)cos(TORAD(angle));
    float sinTheta = (float)sin(TORAD(angle));

    // Find the new x position for the new rotated point

    vNewView.x  = (cosTheta + (1 - cosTheta) * x * x)       * vView.x;
    vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)   * vView.y;
    vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)   * vView.z;

    // Find the new y position for the new rotated point
    vNewView.y  = ((1 - cosTheta) * x * y + z * sinTheta)   * vView.x;
    vNewView.y += (cosTheta + (1 - cosTheta) * y * y)       * vView.y;
    vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)   * vView.z;

    // Find the new z position for the new rotated point
    vNewView.z  = ((1 - cosTheta) * x * z - y * sinTheta)   * vView.x;
    vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)   * vView.y;
    vNewView.z += (cosTheta + (1 - cosTheta) * z * z)       * vView.z;

    // Now we just add the newly rotated vector to our position to set
    // our new rotated view of our camera.
    m_vView = m_vPosition + vNewView;
}



/**
 * This strafes the camera left and right depending on the speed (-/+)
 * @param speed - speed of the camera in m/s.
 **/
void nrCCamera::strafe(float speed){

    // Add the strafe vector to our position
    m_vPosition.x += m_RightVector.x * speed;
    m_vPosition.z += m_RightVector.z * speed;

    // Add the strafe vector to our view
    m_vView.x += m_RightVector.x * speed;
    m_vView.z += m_RightVector.z * speed;
}



/**
 * This will move the camera forward or backward depending on the speed
 * @param speed please set this value in dependence of frame interval to
 * get your movement frame independent (other CPU's same speed)
 **/
void nrCCamera::move(float speed){

    // Get the current view vector (the direction we are looking)
    vec3 vVector = m_vView - m_vPosition;
    vVector.normalize();

    m_vPosition.x += vVector.x * speed;     // Add our acceleration to our position's X
    m_vPosition.z += vVector.z * speed;     // Add our acceleration to our position's Z
    m_vPosition.y += vVector.y * speed;
    m_vView.x += vVector.x * speed;         // Add our acceleration to our view's X
    m_vView.z += vVector.z * speed;         // Add our acceleration to our view's Z
    m_vView.y += vVector.y * speed;         // Add our acceleration to our view's Z

}


/**
 * Rolls camera along its viewing direction. This is equivalent to changing
 * the up vector of our camera @see changeUp()
 **/
void nrCCamera::roll(float angle){
	
	// new up vector
	vec3 up = m_vUpVector;
	
	// create rotation matrix
	mat4 rotZ;
	rotZ.rotate_z(angle);
  
	// calculate new up vector
	up = rotZ * up;
	
	// change up vector
	changeUp(up);
}


/**
 * This updates the camera view (e.g. calculate frustum)
 **/
void nrCCamera::update() {
    
    // set camera matricies
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projection);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// check if we have new field of view (allows us dynamic changes of it)
	static float fovy = 0.0f;
	if (fovy != fieldOfView){
		fovy = fieldOfView;
		setup();
	}
	
	// orient camera using gluLookAt - Function
	Look();  
	
	// Calculate current frustum
	calculateFrustum();

	// Calculate camera in world directions (basis of camera matrix)
	calculateDirections();
}


/**
 * Set camera using gluLookAt - Function
 **/
void nrCCamera::Look(){

    // Give openGL our camera position, then camera view, then camera up vector
    gluLookAt(m_vPosition.x, m_vPosition.y, m_vPosition.z,  
              m_vView.x,     m_vView.y,     m_vView.z,  
              m_vUpVector.x, m_vUpVector.y, m_vUpVector.z);
}


/**
 * This will save our current frustum to allow us do new calculation
 * Think on glPushMatrix in OpenGL
 **/
void nrCCamera::pushFrustum(){
	// if we don't have enough place in our stack
	if (frustumStackPos > FRUSTUM_STACK_LENGTH){ 
		frustumStackPos = FRUSTUM_STACK_LENGTH;
	}
	frustumStackPos++;
}


/**
 * Restore our frustum from the stack
 **/
void nrCCamera::popFrustum(){
	// if we do not have any frustum on the stack
	if (frustumStackPos <= 0){return;}
	frustumStackPos --;	
}


/**
 * Calculate basis of camera matrix. Camera direction vectors in world space
 **/
void nrCCamera::calculateDirections(){
	
	glGetFloatv (GL_MODELVIEW_MATRIX, modelview);
	m_RightVector = vec3( modelview[0], modelview[4], modelview[8] );
	m_UpVector = vec3( modelview[1], modelview[5], modelview[9] );
	m_DirectionVector = vec3 (modelview[2],modelview[6],-modelview[10]);

	// normalize vectors
	m_RightVector.normalize();
	m_UpVector.normalize();
	m_DirectionVector.normalize();	
}



/**
 * calculate our current frustum
 **/
void nrCCamera::calculateFrustum(){

	glMatrixMode(GL_PROJECTION);

	// If we do not have stereo, so calculate frustum with 
	// far plane on viewing distance
	if (!useStereo){
		glPushMatrix();
		 glLoadIdentity();
    	 gluPerspective(fieldOfView, 
		 				(GLfloat)viewPortWidth / (GLfloat)viewPortHeight, 
		 				nearPlane, viewingDistance);
		 frustum[frustumStackPos]->CalculateGLFrustum();
		glPopMatrix();
	
	// else calculate current frustum
	// TODO: correct this to get also viewing distance support in stereo mode
	}else{
		 frustum[frustumStackPos]->CalculateGLFrustum();		
	}
	
	glMatrixMode(GL_MODELVIEW);
	
}


/////////////////////////////////////////////
//		Stereo View                        //
/////////////////////////////////////////////


/**
 * Calculate projection matricies for stereo 
 * @param eyeSep - eye seapration value in meters (e.g. 0.04 = 4 cm)
 * @param focLength - focal length in meters (e.g. 1 = 1 m)
 **/
void nrCCamera::calculateEyeProjections(float eyeSep, float focLength){

	// Variablen setzen
	focalLength = focLength;
	eyeSeparation = eyeSep;
	halfEyeSeparation = eyeSeparation / 2;
	
	// Paar Variablen berechnen
	float ratio = float(viewPortWidth)/float(viewPortHeight);
	float radiance = DEG2RAD * fieldOfView / 2;
	float wd2 = nearPlane * tan(radiance);
	float ndfl = nearPlane / focalLength;
	
	// Berechne Furs Rechte Auge
	float left = -ratio * wd2 - 0.5 * eyeSeparation * ndfl;
	float right = ratio * wd2 - 0.5 * eyeSeparation * ndfl;
	float top = wd2;
	float bottom = -wd2;
	
	// Nutze OpenGL-Frustum um Projection Matrix zu berechnen
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glFrustum(left,right,bottom,top,nearPlane,farPlane);
	glGetFloatv(GL_PROJECTION_MATRIX, asymmetricProjectionRight);

	// Berechne Furs Rechte Auge
	left = -ratio * wd2 + 0.5 * eyeSeparation * ndfl;
	right = ratio * wd2 + 0.5 * eyeSeparation * ndfl;
	top = wd2;
	bottom = -wd2;
	
	// Nutze OpenGL-Frustum um Projection Matrix zu berechnen
	glLoadIdentity();
	glFrustum(left,right,bottom,top,nearPlane,farPlane);
	glGetFloatv(GL_PROJECTION_MATRIX, asymmetricProjectionLeft);
	
	// Alles wie gewohnt stellen
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	
}


/**
 * Here we activate our left eye projection matrix. Also camera position will
 * be shifted along cameras right vector in necessary direction.
 * If useStereo is not activated, so there is no effect
 * @param setNewViewport if true, so new viewport will also be setted
 * New viewport will be half of viewPortWidth
 **/
void nrCCamera::activateLeftEye(bool setNewViewport){

	// Only if stereo activated
	if (!useStereo){
		if (widthHalfed){
			// change viewport
			setWidth(getWidth()*2);
			widthHalfed = false;
		}
		return;
	}
	
	// if viewport was not touched, so do this now	
	if (!widthHalfed){
		setWidth(getWidth()/2);
		widthHalfed = true;
	}

	// change viewport if it is set
	if (setNewViewport){				
		glViewport(0,0,getWidth(),getHeight());
	}
		
	// Load projection matrix for left eye
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(asymmetricProjectionLeft);
	glMatrixMode(GL_MODELVIEW);
	
	vec3 r = m_RightVector * halfEyeSeparation;
	currentView = false;
	
	// Move camera view in left direction with half eye separation meters 
	glLoadIdentity();
	gluLookAt(m_vPosition.x - r.x, m_vPosition.y - r.y, m_vPosition.z - r.z,  
              m_vView.x - r.x,     m_vView.y - r.y,     m_vView.z - r.z,  
              m_vUpVector.x, m_vUpVector.y, m_vUpVector.z);
	
	calculateFrustum();
			
}

/**
 * Same as activateLeftEye but for right eye
 * @see activateLeftEye()
 **/
void nrCCamera::activateRightEye(bool setNewViewport){

	// Nur wenn Stereo aktiviert ist
	if (!useStereo){
		if (widthHalfed){
			// Kameraeinstellungen ändern
			setWidth(getWidth()*2);
			widthHalfed = false;
		}
		return;
	}

	// falls noch nicht geschehen, so breite halbieren	
	if (!widthHalfed){
		// Kameraeinstellungen ändern
		setWidth(getWidth()/2);
		widthHalfed = true;
	}

	// ändere den viewport, falls gewünscht
	if (setNewViewport){				
		// Jetzt Inhalt des rechten Auges rendern
		glViewport(getWidth(),0,getWidth(),getHeight());		
	}

	// Lade Projection furs Linke Auge
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(asymmetricProjectionRight);
	glMatrixMode(GL_MODELVIEW);
	
	// Kurze Berechnung
	vec3 r = m_RightVector * halfEyeSeparation;
	currentView = true;
	
	// Verschiebe die Sicht um das halbe Augenabstand nach rechts
	glLoadIdentity();
	gluLookAt(m_vPosition.x + r.x, m_vPosition.y + r.y, m_vPosition.z + r.z,  
              m_vView.x + r.x,     m_vView.y + r.y,     m_vView.z + r.z,  
              m_vUpVector.x, m_vUpVector.y, m_vUpVector.z);
	calculateFrustum();
		
}


/**
 * restore old viewport if stereo is activated
 * @note There is no stack support for viewport. New value will just be the 
 * doble of halfed width
 * Do not use it, if you have broken viewport values (e.g. glViewport(23,325,698,980))
 **/
void nrCCamera::deactivateEyeViews(bool setNewVieport){

	// falls unsere breite halbiert ist
	if (widthHalfed){
		// Kameraeinstellungen ändern
		setWidth(getWidth()*2);
		widthHalfed = false;
		
		if (setNewVieport)
			glViewport(0,0,getWidth(),getHeight());	
	}		

}


/**
 * activate/deactivate stereo view
 **/
void nrCCamera::setUseStereo(bool b){

	// if we was in stereo mode
	if (useStereo && !b){
		deactivateEyeViews();	
	}
	
	useStereo = b;
	currentView = false;

}


/**
 * Calculate the Z-Value of a point which will be transformed to a pixel
 * Calculated value is the same as OpenGL's depth-value
 * @param point is a point in 3D-space which will be transformed to 2D
 * @return z-value (OpenGL's Depth-Value) of a pixel according to this point
 * @note: use this if you want to find out which depth value will your
 * point have if it will be transformed by the OpenGL's pipeline
 **/
float nrCCamera::calcZValue(const vec3& point){

	vec3 p = projection * modelview * point;
	
	// calculate depth value like in OpenGL's specification
	return 2 - 0.5*(( (p.z*(farPlane+nearPlane) + 2*farPlane*nearPlane)
			/ (p.z*(farPlane-nearPlane)) ) + 1);
	
}
