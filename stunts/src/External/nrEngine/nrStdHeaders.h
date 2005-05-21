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

#ifndef __NR_STD_HEADERS_H_
#define __NR_STD_HEADERS_H_


#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>


// STL Library
#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>

// Hash Map if it is exists
#ifdef EXT_HASH
#	include <ext/hash_map>
#	include <ext/hash_set>
using __gnu_cxx::hash_map;
#else
#	include <hash_set>
#	include <hash_map>
#endif

// Streaming
#include <fstream>
#include <iostream>
#include <sstream>


// Namespace declarations
using std::vector;
using std::map;
using std::string;
using std::list;
using std::deque;
using std::queue;

#endif
