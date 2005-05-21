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

#ifndef __STACKS_H_
#define __STACKS_H_


/*--------------------------------------------------------------------------
 Stack Definition for Headers
 -------------------------------------------------------------------------*/
 
#define STACK_DEFINE_EXTERN(name,type) extern type _nr_ ## name ## Stack[32];\
				       extern int  _nr_ ## name ## StackPos;\
				       extern bool _nr_ ## name ## IsOn;

				       				  
#define STACK_PUSH_FUNC_DEF(name) 	void nrPush ## name()
#define STACK_POP_FUNC_DEF(name)  	void nrPop ## name()
#define STACK_OFF_FUNC_DEF(name) 	void nrOff ## name()
#define STACK_ON_FUNC_DEF(name)  	void nrOn ## name()
#define STACK_PUSHOFF_FUNC_DEF(name) 	void nrPushOff ## name()
#define STACK_PUSHON_FUNC_DEF(name)  	void nrPushOn ## name()
				  
			

/*--------------------------------------------------------------------------
 Stack Definition for Source Files
 -------------------------------------------------------------------------*/
#define STACK_DEFINE(name,type)  type _nr_ ## name ## Stack[32];\
				 int  _nr_ ## name ## StackPos = 0;\
				 bool _nr_ ## name ## IsOn = false;


//----------------------------------------------------------------------------
// Definition of push function
// This allows us to push the current state of our variable
// getCurrentState should be a Function which returns current state
//----------------------------------------------------------------------------
#define STACK_PUSH_FUNC(name,type,getCurrentState)\
	void nrPush ## name(){\
	if (_nr_ ## name ## StackPos > 31){ _nr_ ## name ## StackPos = 31;}\
	_nr_ ## name ## Stack[_nr_ ## name ## StackPos] = static_cast<type>(getCurrentState);\
	_nr_ ## name ## StackPos++;\
	}

//----------------------------------------------------------------------------
// Definition of pop function
// Just restore our pushed state value
// setCurrentState is function which set the state by given stack data
//----------------------------------------------------------------------------
#define STACK_POP_FUNC(name,setCurrentState)\
	void nrPop ## name(){\
	if (_nr_ ## name ## StackPos < 1){return;}\
	_nr_ ## name ## StackPos--;\
	setCurrentState (_nr_ ## name ## Stack[_nr_ ## name ## StackPos]);\
	}

	
//----------------------------------------------------------------------------
// Definition of pop function
// Just restore our pushed state value
// Restoring is done through on or off.
//----------------------------------------------------------------------------
#define STACK_POP_ONOFF_FUNC(name)\
	void nrPop ## name(){\
	if (_nr_ ## name ## StackPos < 1){return;}\
	_nr_ ## name ## StackPos--;\
	if (static_cast<bool>(_nr_ ## name ## Stack[_nr_ ## name ## StackPos]))\
		nrOn ## name();\
	else\
		nrOff ## name();\
	}
	
	
//----------------------------------------------------------------------------
// Definition of extra push function
// push and then off/on the state
//----------------------------------------------------------------------------
#define STACK_PUSHOFF_FUNC(name)\
	void nrPushOff ## name(){\
	nrPush ## name(); nrOff ## name();\
	}
#define STACK_PUSHON_FUNC(name)\
	void nrPushOn ## name(){\
	nrPush ## name(); nrOn ## name();\
	}


//----------------------------------------------------------------------------
// get variable shows on/off state
//----------------------------------------------------------------------------
#define STACK_ISON_STATE(name) _nr_ ## name ## IsOn
	
/*--------------------------------------------------------------------------
 Define all stuff for simple enabler disabler
 -------------------------------------------------------------------------*/
#define DEFINE_STACK_FOR_IT(name, type)\
	STACK_DEFINE_EXTERN(name, type);\
	STACK_PUSH_FUNC_DEF(name);\
	STACK_POP_FUNC_DEF(name);\
	STACK_OFF_FUNC_DEF(name);\
	STACK_ON_FUNC_DEF(name);\
	STACK_PUSHOFF_FUNC_DEF(name);\
	STACK_PUSHON_FUNC_DEF(name);\
			


/*--------------------------------------------------------------------------
 Define enablers disablers
 -------------------------------------------------------------------------*/
DEFINE_STACK_FOR_IT(Lit,bool);
DEFINE_STACK_FOR_IT(Blend,bool);
DEFINE_STACK_FOR_IT(DepthRead,bool);
DEFINE_STACK_FOR_IT(ColorMaterial,bool);
DEFINE_STACK_FOR_IT(CullFace,bool);
DEFINE_STACK_FOR_IT(ScissorTest,bool);
DEFINE_STACK_FOR_IT(Texture2D,bool);

DEFINE_STACK_FOR_IT(DepthWrite,bool);
DEFINE_STACK_FOR_IT(ColorWrite,bool);


#undef DEFINE_STACK_FOR_IT

#endif
