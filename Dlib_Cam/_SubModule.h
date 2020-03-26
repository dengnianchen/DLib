#pragma once
#include "global.h"
#include <string>



NS_DLIB_CAM_BEGIN

class _System;

class _SubModule {
public:

	_System* p_system;

	static _SubModule GetSubModule (const std::string& name);
	static void UnloadModules ();

private:
	void* _handler;

};

NS_DLIB_CAM_END