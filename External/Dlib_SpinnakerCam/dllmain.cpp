// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "global.h"
#include "SpinnakerSystem.h"



using namespace Dlib::Cam;



extern "C" {

	DLIBSPINNAKERCAM_INTERFACE _System* entry () {

		return new SpinnakerSystem ();

	}

}



BOOL APIENTRY DllMain (HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {

	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		OutputDebugStringA ("[Dlib_SpinnakerCam] DLL_PROCESS_ATTACH\n");
		break;
	case DLL_THREAD_ATTACH:
		OutputDebugStringA ("[Dlib_SpinnakerCam] DLL_THREAD_ATTACH\n");
		break;
	case DLL_THREAD_DETACH:
		OutputDebugStringA ("[Dlib_SpinnakerCam] DLL_THREAD_DETACH\n");
		break;
	case DLL_PROCESS_DETACH:
		OutputDebugStringA ("[Dlib_SpinnakerCam] DLL_PROCESS_DETACH\n");
		break;
	}
	return TRUE;

}