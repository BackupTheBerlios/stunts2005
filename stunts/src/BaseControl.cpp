/* Stunts 2005 - A remake of the game Stunts
 *
 * Copyright (C) 2005
 *                    Stunts 2005 Workgroup, 
 *                    http://developer.berlios.de/projects/stunts2005
 *
 * Maintainer:        Art Tevs <tevs@mpi-sb.mpg.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, 
 * USA.
 */


#include "BaseControl.hpp"

#include "AICarControl.hpp"
#include "HumanControl.hpp"

using boost::shared_ptr;

namespace stunts {

	//--------------------------------------------------------------------------
	CBaseControl::CBaseControl()
	{
	}
	
	//--------------------------------------------------------------------------
	CBaseControl::~CBaseControl()
	{
	}
	
	
	//--------------------------------------------------------------------------
	CBaseControl* CBaseControl::createInstance(std::string objType)
	{
		// check whenever we have valid parameter
		if (objType.length() == 0) return NULL;
			
		// check whenever we do support this kind of object type
		if (objType == CHumanControl::getControlType())
			return new CHumanControl();
		else if (objType == CAICarControl::getControlType())
			return new CAICarControl();
		else
		{
			nrLog.Log(NR_LOG_APP, "CBaseControl::createInstance(): Not supported kind of controller type \"%s\"", objType.c_str());
			return NULL;
		}
		
	}

}

