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

#include "Resources.h"
#include "../nrEngine.h"


/**
 * Construct new instance of nrCResourceManager class and set
 * default values.
 **/
nrCResourceManager::nrCResourceManager(){
	mLastHandle = 1;
	mMemBudget = 0;
	mMemUsage = 0;
}


/**
 * Release all used memory and also mark all loaded resources to unload. After
 * you calling the destructor of nrCResourceManager class you will 
 * still be able to access to resources created by this manager, because
 * all of the resources pointers has got an reference counter.
 * As soon as you do not use the resource anymore and the manager is destroyed
 * the resource will be automaticly release used memory.
 *
 * But it is not a good way to use resources if the manager is not present.
 * You will get a WARNING in the log files if you do it this way, which will
 * help you to detect incostistencies and to remove resources completely before
 * you delete the resource manager.
 **/
nrCResourceManager::~nrCResourceManager(){

}

/**
 * Set amount of memory that can be used by the manager. Set this value to 80-90%
 * of your physical memory if you do not have any virtual memory in your system
 * available. The manager will try to stay in the budget you given for him.
 * Resources that are least used and has got lowest priority will be unloaded
 * to free the memory to stay in the given budget of memory.
 *
 * @param bytes Count of bytes which can be used for resource storing
 * @return either NR_OK or error code.
 *
 * @note You can set this value to ca 80-90% of your memory available.
 *		 Before resource manager unload least used resources it will allocate
 *		 memory for the new resource to check how much it needs. So we still need
 *		 some free memory for this temporary allocation. So 80-90% are good
 *		 values for memory budget.
 *		 If you want to be very precisious you can set this value to memory
 *		 size minus biggest resource size. Which will use maximal available
 *		 memory space.
 **/
nrResult nrCResourceManager::setMemoryBudget(size_t bytes){
	mMemBudget = bytes;
	return checkMemoryUsage();
}


/**
 * Get the memory budget resource manager have got.
 **/
size_t nrCResourceManager::getMemoryBudget() const{
	return mMemBudget;
}


/**
 * Returns the usage of memory in bytes.
 **/
size_t nrCResourceManager::getMemoryUsage() const{
	return mMemUsage;
}


/**
 * Here you can register any loader by the manager. Loader are used to load resources
 * from files/memory and to store resources in the memory. By registration of
 * a loader you have to specify his unique name. This name will be used to access
 * to stored loader (for example to remove it).
 * At the end you have to specify the filetypes for which this loader should be used.
 *
 * @param name Unique loader name
 * @param loader Smart pointer containing the loader
 * @param count Number of given filetypes strings.
 * @param fileTypes Array of strings containing all filetypes to which this loader
 *					should be bound.
 *
 * @return either NR_OK or error code:
 *			- NR_RES_LOADER_ALREADY_EXISTS
 *			- NR_BAD_PARAMETERS
 *
 * @note If you do specify two different loaders for same filetype,
 * so the last specified will be used.
 **/
nrResult nrCResourceManager::registerLoader(const string& name, nrResourceLoader loader,
											int32 count, string* fileTypes){

	// check whenver such a loader already exists
	if (mLoaderName.find(name) != mLoaderName.end()){
		return NR_RES_LOADER_ALREADY_EXISTS;
	}

	// check for bad parameters
	if (loader == NULL || (count > 0 && fileTypes == NULL)){
		return NR_BAD_PARAMETERS;
	}

	int32 id = mLoaderList.size();

	nrLog.Log(NR_LOG_ENGINE, "ResourceManager: Register new resource loader %s, id=%d", name.c_str(), id);
	
	// register the loader
	mLoaderList.push_back(loader);
	mLoaderName[name] = id;
	
	// Register filetypes
	try{
		for (int32 i=0; i < count; i++){
			nrLog.Log(NR_LOG_ENGINE, "ResourceManager: New loader %d for *.%s", id, fileTypes[i].c_str());
			mLoaderToFileType[fileTypes[i]] = id;
		}
	}catch(...){
		return NR_BAD_PARAMETERS;
	}

	return NR_OK;
}


/**
 * Removes the loader from the loader list. All bounded filetypes
 * registrations will also be removed. So after you calling this you have
 * to check whenever you still able to load filetypes you want to use.
 *
 * @param name Unique name of the loader
 * @return either NR_OK or error code:
 *		- NR_RES_LOADER_NOT_REGISTERED
 *		- NR_RES_ERROR
 **/
nrResult nrCResourceManager::removeLoader(const string& name){

	// get id of the loader
	loader_map::const_iterator jt = mLoaderName.find(name);
	if (jt == mLoaderName.end()){
		return NR_RES_LOADER_NOT_REGISTERED;
	}
	int32 id = jt->second;


	mLoaderList.erase(mLoaderList.begin() + id);
	mLoaderName.erase(name);

	nrLog.Log(NR_LOG_ENGINE, "ResourceManager: Remove loader %s, id=%d", name.c_str(), id);

	// scan in filetype for loaders
	vector<string> types;
	loader_map::iterator it = mLoaderToFileType.begin();
	while (it != mLoaderToFileType.end()){
		if (it->second == id){
			types.push_back(it->first);
		}
		it ++;
	}

	// remove all bounded filetypes
	for (uint32 i=0; i < types.size(); i++){
		nrLog.Log(NR_LOG_ENGINE, "ResourceManager: File type *.%s can not be loaded now", types[i].c_str());
		mLoaderToFileType.erase(types[i]);
	}
		
	return NR_OK;
}


/**
 * Return the loader by given filetype. If there is no loader which can load
 * such a filetype NULL will be returned
 **/
nrResourceLoader nrCResourceManager::getLoaderByFile(const string& fileType){
	if (!mLoaderToFileType[fileType]){
		return nrResourceLoader();
	}

	// get id
	int32 id = mLoaderToFileType[fileType];
	return mLoaderList[id];
}


/**
 * Return the loader by given name. If the loader with this name does not
 * exists, so NULL will be given back
 **/
nrResourceLoader nrCResourceManager::getLoader(const string& name){
	if (!mLoaderName[name]){
		return nrResourceLoader();
	}

	// get id
	int32 id = mLoaderName[name];
	return mLoaderList[id];
}


/**
 * This is a template function which will create an empty resource of the given
 * resource type "ResourceType". This function allows us to use general resource
 * manager class for all kind of resources with different types. So we need somewhere
 * a place where we can create objects of derived classes from nrCResource class.
 * And because you do not have to do it outside of this class and you do not have to
 * derive your own managers, you just specify here the type and it will be created.
 *
 * After you created a resource you will still not be able to use it. You have
 * to load a resource from a file. You have also to load it, because we are using
 * concept of empty resources. So loader has to get access on the resource to
 * initialize it with empty data.
 *
 * @param name Unique name of that resource. The name must be specified !!!
 * @param group Name of the group to which this resource belongs.
 * @throw either NR_OK or error code:
 *		- NR_BAD_PARAMETERS
 *		- NR_OUT_OF_MEMORY
 *		- NR_ERROR
 *
 * @note If you want to store empty data in extra files or you want to debug
 * 		 resource by loading files which get your attention on it (pink texture,
 *		 big explosion sound, ...), you have to do it outside in the nrCResource
 *		 class. This manager does only manages the resource and is not responsible
 *		 for loading of resources from files.
 **/
template<class ResourceType>
nrResourcePtr nrCResourceManager::createResource(const string& name, const string& group){

	// check whenever such a resource already exists
	nrResourcePtr pRes = getByName(name);
	if (!pRes.isNull()){
		nrLog.Log(NR_LOG_ENGINE, "ResourceManager: Creating of resource fails. Resource %s already exists", name.c_str());
		return pRes;
	}
	
	// create new handle
	nrResourceHandle handle = mLastHandle++;
	nrLog.Log(NR_LOG_ENGINE, "ResourceManager: Create new empty resource %s, id=%d", name.c_str(), handle);
	
	// create resource through implicit creation function		
	nrIResource* res = createEmptyImpl(handle, name, group);
	
	// we have no function that create it in implicit way, so
	if (res == NULL){
		res = new ResourceType();		
	}else{
		nrLog.Log(NR_LOG_ENGINE, "ResourceManager: Resource %s created through derived class", name.c_str());
	}

	// create a holder for that resource
	shared_ptr<nrCResourceHolder> holder(new nrCResourceHolder());
	holder->resetRes(res);
	
	// store the resource in database
	mResourceGroup[group].push_back(handle);
	mResource[handle] = holder;
	mResourceName[name] = handle;

	res->mResGroup = group;
	res->mResName = name;
	res->mResHandle = handle;
	
	// return a pointer to that resource
	return nrResourcePtr(holder);
}


/**
 * Load a resource from a file. For loading of a resource the registered
 * loader will be used. The filetype is defined by the last
 * characters of the file name. If we can not find this characters
 * so the manual loader will be used if such one is specified. Manual loader
 * will also be used if it is specified and filetype is detected.
 * You can assign your resource to special group. The group names
 * has to be unique.
 *
 * If such a resource could not load, so error occurs and NULL will be returned.
 *
 * The loader will also be used to load empty resource for that resource type.
 * If the empty resource already loaded, it will not be loaded again.
 *
 * @param name Unique name fo the resource
 * @param fileName File name of the file to be loaded
 * @param group Group name to which this resource should be assigned
 * @param loader Loader which should be used if you want to overload
 *						the loader registered before for this filetype.
 *
 * @throw nrResult of:
 *			- NR_RES_LOADER_NOT_REGISTERED
 *			- NR_RES_BAD_FILETYPE
 *			- NR_RES_CAN_NOT_LOAD_EMPTY
 * 
 **/
template<class ResourceType>
nrResourcePtr nrCResourceManager::loadResource(const string& name,
							const string& fileName,
							const string& group,
							nrResourceLoader loader){

	// check whenever right parameters are specified
	ASSERT(name.length() > 0  && fileName.length() > 0);
	
	string type = "";
	nrIResource* res = NULL;
	nrResourceHandle handle;

	// check whenever such a resource already exists
	nrResourcePtr pRes = getByName(name);
	if (!pRes.isNull()){
		nrLog.Log(NR_LOG_ENGINE, "WARNING: ResourceManager: Resource %s already loaded", name.c_str());
		
		pRes->unloadRes();
	}else{
		
		// detect the file type by reading out its last characters		
		for (uint32 i = fileName.length()-1; i >= 0; i++){
			if (fileName[i] == '.'){
				break;
			}
			type = fileName.substr(i,i) + type;
		}
	
		if (type.length() == 0 && loader == NULL){
			nrLog.Log(NR_LOG_ENGINE, "ResourceManager: Could not detect filetype of %s", fileName.c_str());
			throw NR_RES_BAD_FILETYPE;	
		}
	
	
		// create new handle
		handle = mLastHandle++;
		nrLog.Log(NR_LOG_ENGINE, "ResourceManager: Load resource %s, id=%d from %s", name.c_str(), handle, fileName.c_str());

		// load resource through implicit loading function
		res = loadResourceImpl(handle, name, fileName, group, loader);
		
		// we have no function that create it in implicit way, so
		if (res == NULL){
			res = new ResourceType();		
		}else{
			nrLog.Log(NR_LOG_ENGINE, "ResourceManager: Resource %s created through derived class", name.c_str());
		}
	}
		

	// get appropriate loader for that filetype
	if (loader == NULL){
		if (mLoaderName[type]){
			int32 id = mLoaderName[type];
			loader = mLoaderList[id];
		}
	}

	if (loader == NULL){
		nrLog.Log(NR_LOG_ENGINE, "ResourceManager: Loader for *.%s not found", type.c_str());
		throw NR_RES_LOADER_NOT_REGISTERED;
	}

		
	// check whenever we could load the file
	res->mResLoader = loader;
	nrResult ret = res->reloadRes();
	if (ret != NR_OK){
		nrLog.Log(NR_LOG_ENGINE, "ResourceManager: Could not load resource %s", nrErrorGetShort(ret));
		throw ret;
	}

	
	// check now if a empty resource of that type already exists and load
	// it if not
	shared_ptr<nrIResource> empty;
	res_empty_map::iterator it = mEmptyResource.find(res->getResTypeName());
	if (it == mEmptyResource.end()){
		empty.reset(loader->createEmptyResource());
		if (empty.get() == NULL){
			nrLog.Log(NR_LOG_ENGINE, "ResourceManager: Cannot load empty resource of type %s", res->getResTypeName().c_str());
			throw NR_RES_CAN_NOT_LOAD_EMPTY;
		}

		mEmptyResource[res->getResTypeName()] = empty;
	}
	
	// create a holder for that resource
	shared_ptr<nrCResourceHolder> holder(new nrCResourceHolder());
	holder->resetRes(res);
	holder->setEmptyResource(empty);
	
	// store the resource in database
	mResourceGroup[group].push_back(handle);
	mResource[handle] = holder;
	mResourceName[name] = handle;

	res->mResGroup = group;
	res->mResName = name;
	res->mResHandle = handle;
	
	// check for memory usage
	mMemUsage += holder->getResource()->getDataSize();
	ret = checkMemoryUsage();
	
	if (ret != NR_OK){
		nrLog.Log(NR_LOG_ENGINE, "ResourceManager: Could not rearrange memory %s", nrErrorGetShort(ret));
		throw ret;
		return nrResourcePtr();
	}

	// return a pointer to that resource
	return nrResourcePtr(holder);
}


/**
 * Unload the resource from the memory.
 * Each resource should know how to unload itself. The holder holding
 * the resource will now show to an empty resource.
 *
 * @param name Unique name of the resource
 * @return either NR_OK or error code:
 *		- NR_RES_NOT_FOUND
 **/
nrResult nrCResourceManager::unload(const string& name){

	nrResourcePtr res = getByName(name);

	if (res.isNull()){
		return NR_RES_NOT_FOUND;
	} 

	return res->unloadRes();
}


/**
 * Unload the resource from the memory.
 * Each resource should know how to unload itself. The holder holding
 * the resource will now show to an empty resource.
 *
 * @param res Pointer to the resource
 * @return always NR_OK
 **/
nrResult nrCResourceManager::unload(nrResourcePtr& res){

	if (res.isNull()){
		return NR_RES_NOT_FOUND;
	} 

	return res->unloadRes();
}

/**
 * Unload the resource from the memory.
 * Each resource should know how to unload itself. The holder holding
 * the resource will now show to an empty resource.
 *
 * @param handle Handle of the resource
 * @return either NR_OK or error code:
 *		- NR_RES_NOT_FOUND
 **/
nrResult nrCResourceManager::unload(nrResourceHandle& handle){

	nrResourcePtr res = getByHandle(handle);

	if (res.isNull()){
		return NR_RES_NOT_FOUND;
	} 

	return res->unloadRes();
}


/**
 * Reload the resource, so it will now contain it's real data.
 * Call this function if want to get your resource back after
 * it was unloaded.
 *
 * @param name Unique name of the resource
 * @return either NR_OK or error code:
 *		- NR_RES_NOT_FOUND
 *
 * @note Reloading of resource will cost time. The resource will
 *		 try to access to it's file through the loader with which
 *		 one it was created. So try to implement very intellegent
 *		 algorithm to prevent reloading/unloading as much as possible
 **/
nrResult nrCResourceManager::reload(const string& name){
	
	nrResourcePtr res = getByName(name);

	if (res.isNull()){
		return NR_RES_NOT_FOUND;
	} 

	return res->reloadRes();
}

/**
 * Reload the resource, so it will now contain it's real data.
 * Call this function if want to get your resource back after
 * it was unloaded.
 *
 * @param handle Handle of the resource
 * @return either NR_OK or error code:
 *		- NR_RES_NOT_FOUND
 *
 * @note Reloading of resource will cost time. The resource will
 *		 try to access to it's file through the loader with which
 *		 one it was created. So try to implement very intellegent
 *		 algorithm to prevent reloading/unloading as much as possible
 **/
nrResult nrCResourceManager::reload(nrResourceHandle& handle){
	
	nrResourcePtr res = getByHandle(handle);

	if (res.isNull()){
		return NR_RES_NOT_FOUND;
	} 

	return res->reloadRes();
}

/**
 * Reload the resource, so it will now contain it's real data.
 * Call this function if want to get your resource back after
 * it was unloaded.
 *
 * @param res Pointer to the resource
 * @return always NR_OK
 *
 * @note Reloading of resource will cost time. The resource will
 *		 try to access to it's file through the loader with which
 *		 one it was created. So try to implement very intellegent
 *		 algorithm to prevent reloading/unloading as much as possible
 **/
nrResult nrCResourceManager::reload(nrResourcePtr& res){
	
	if (res.isNull()){
		return NR_RES_NOT_FOUND;
	} 

	return res->reloadRes();
}


/**
 * This will remove the resource from the database.
 * After you removed the resource and would try to access to it through the
 * manager NULL will be returned.
 *
 * However if you remove the resource it is not just deleted from the memory
 * because of shared pointer system. So your parts of appliation, who gets
 * pointers to it are still able to access to it.
 *
 * @param ptr Remove the resource whithin this pointer
 * @return either NR_OK or error code:
 *		- NR_RES_NOT_FOUND
 **/
nrResult nrCResourceManager::removeImpl(nrResourcePtr& ptr){

	ASSERT(!ptr.isNull() && "Given pointer should not be zero");
	
	nrResourceHandle hdl = ptr->getResHandle();
	if (hdl == 0){
		return NR_RES_NOT_FOUND;
	}
	nrLog.Log(NR_LOG_ENGINE, "ResourceManager: Remove resource %s", ptr->getResName().c_str());
	
	// remove the handle from the group list
	string grp = ptr->getResGroup();
	if (grp.length() > 0){
		nrLog.Log(NR_LOG_ENGINE, "ResourceManager: Remove resource %s from group", grp.c_str());
		res_grp_map::iterator jt = mResourceGroup.find(grp);
		if (jt != mResourceGroup.end()){
			jt->second.remove(hdl);
		}else{
			nrLog.Log(NR_LOG_ENGINE, "ResourceManager: WARNING: There is no group in the database according to resource's group name %s", grp.c_str());
		}
	}

	// clear the database
	mResourceName.erase(ptr->getResName());
	mResource.erase(hdl);
	
	return NR_OK;
}


/**
 * This will remove the resource from the database.
 * After you removed the resource and would try to access to it through the
 * manager NULL will be returned.
 *
 * However if you remove the resource it is not just deleted from the memory
 * because of shared pointer system. So your parts of appliation, who gets
 * pointers to it are still able to access to it.
 *
 * @param name Unique name of the resource
 * @return either NR_OK or error code:
 *		- NR_RES_NOT_FOUND
 **/
nrResult nrCResourceManager::remove(const string& name){

	// check whenever such a resource exists
	nrResourcePtr ptr = getByName(name);
	
	if (ptr.isNull()){
		nrLog.Log(NR_LOG_ENGINE, "ResourceManager: Remove resource %s not found", name.c_str());
		return NR_RES_NOT_FOUND;
	}

	return removeImpl(ptr);
}

/**
 *
 * @param handle Unique handle of the resource
 * @return either NR_OK or error code:
 *		- NR_RES_NOT_FOUND
 **/
nrResult nrCResourceManager::remove(nrResourceHandle& handle){

	// check whenever such a resource exists
	nrResourcePtr ptr = getByHandle(handle);
	
	if (ptr.isNull()){
		nrLog.Log(NR_LOG_ENGINE, "ResourceManager: Remove resource %d not found", handle);
		return NR_RES_NOT_FOUND;
	}

	return removeImpl(ptr);
}

/**
 *
 * @param ptr Pointer to the resource
 * @return either NR_OK or error code:
 *		- NR_RES_NOT_FOUND
 **/
nrResult nrCResourceManager::remove(nrResourcePtr& ptr){

	// check whenever such a resource exists
	if (!ptr.isNull()){
		return removeImpl(ptr);
	}
	return NR_OK;
}


/**
 * Get the pointer to a resource by it's name.
 * If there is no resource with this name, so NULL pointer will be returned
 *
 * @param name Unique name of the resource
 *
 * @note At now you need a time of O(2n), where n is number of resources.
 *		 Performance can became better by using of hash_map O(1)
 **/
nrResourcePtr nrCResourceManager::getByName(const string& name){
	res_str_map::iterator it = mResourceName.find(name);

	if (it == mResourceName.end()){
		return nrResourcePtr();
	}

	res_hdl_map::iterator jt = mResource.find(it->second);

	ASSERT(jt != mResource.end() && "Fatal Error in the Database !!!");

	return nrResourcePtr(jt->second);
}


/**
 * Same as getByName(), but here you get the resource by handle.
 **/
nrResourcePtr nrCResourceManager::getByHandle(const nrResourceHandle& handle){
	res_hdl_map::iterator it = mResource.find(handle);

	if (it == mResource.end()){
		return nrResourcePtr();
	}

	return nrResourcePtr(it->second);
}


/** 
 * Check if we have now memory available.
 * If we need to remove some resources from the memory to get free place
 * so do it.
 **/
nrResult nrCResourceManager::checkMemoryUsage(){
	// TODO: Has to be implemented
	return NR_OK;
}


/**
 * This function will add a given resource to the resource management system.
 * From now one resource management get the rights for unloading/reloading and
 * removing resources from the memory.
 *
 * @param res  Resource pointer to the resource. (Be careful this is not a smart pointer)
 * @param name Unique name of the resource that should be used in the database
 * @param group Name of the group to which the resource belongs
 * @return either NR_OK or error code
 *
 * @note The resource should already know which loader does loading
 *		 of it from files. So database will not assign any loader to
 *		 that resource. You should do it before calling this method.
 **/
nrResult nrCResourceManager::add(nrIResource* res, const string& name,
											const string& group){

	// check for bad parameters
	if (res == NULL) return NR_OK;
	if (name.length() == 0) return NR_BAD_PARAMETERS;
	if (res->mResLoader == NULL) return NR_RES_LOADER_NOT_EXISTS;

	
	// check whenever such resource already exists
	if (!getByName(name).isNull()){
		nrLog.Log(NR_LOG_ENGINE, "ResourceManager: WARNING: Add resource %s but it already exists", name.c_str());
		return NR_OK;
	}

	nrResourceHandle handle = mLastHandle++;
	nrLog.Log(NR_LOG_ENGINE, "ResourceManager: Add resource %s id=%d to the database from outside", name.c_str(), handle);

	// check now if a empty resource of that type already exists and load
	// it if not
	shared_ptr<nrIResource> empty;
	res_empty_map::iterator it = mEmptyResource.find(res->getResTypeName());
	if (it == mEmptyResource.end()){
		empty.reset(res->mResLoader->createEmptyResource());
		if (empty.get() == NULL){
			nrLog.Log(NR_LOG_ENGINE, "ResourceManager: Cannot load empty resource of type %s", res->getResTypeName().c_str());
			return NR_RES_CAN_NOT_LOAD_EMPTY;
		}
		mEmptyResource[res->getResTypeName()] = empty;
	}

	
	// create a holder for that resource
	shared_ptr<nrCResourceHolder> holder(new nrCResourceHolder());
	holder->resetRes(res);
	holder->setEmptyResource(empty);
	
	// store the resource in database
	mResourceGroup[group].push_back(handle);
	mResource[handle] = holder;
	mResourceName[name] = handle;

	res->mResGroup = group;
	res->mResName = name;
	res->mResHandle = handle;
	
	// check for memory usage
	mMemUsage += holder->getResource()->getResDataSize();
	nrResult ret = checkMemoryUsage();
	
	if (ret != NR_OK){
		nrLog.Log(NR_LOG_ENGINE, "ResourceManager: Could not rearrange memory %s", nrErrorGetShort(ret).c_str());
		return ret;
	}

	// return a pointer to that resource
	return NR_OK;
}

