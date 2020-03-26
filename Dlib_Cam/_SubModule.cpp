#include "_SubModule.h"
#include <Windows.h>
#include <direct.h>
#include <map>
#include <vector>
#include "../Dlib_Basic/Basic.h"
#include "_System.h"
#include "LoadModuleFailedException.h"

using namespace Dlib::Basic;



NS_DLIB_CAM_BEGIN

typedef _System*(*entryFunc)(void);
static std::map<std::string, _SubModule> _Modules;



std::wstring GetSelfModuleDirectory () {

	MEMORY_BASIC_INFORMATION mbi;
	auto h = HMODULE ((::VirtualQuery (GetSelfModuleDirectory, &mbi, sizeof (mbi)) != 0) ? (HMODULE)mbi.AllocationBase : NULL);
	wchar_t pathBuff[MAX_PATH] = { 0 };
	GetModuleFileNameW (h, pathBuff, MAX_PATH);
	std::wstring dllPath = pathBuff;
	return dllPath.substr (0, dllPath.find_last_of (L"\\") + 1);

}



_SubModule _SubModule::GetSubModule (const std::string& name) {

	auto it = _Modules.find (name);
	if (it != _Modules.end ())
		return it->second;
	auto nameW = StringUtils::ToWString (name);
	auto dllFileNameW = L"Dlib_" + nameW + L".dll";
	auto selfDllDirectory = GetSelfModuleDirectory ();

	std::vector<std::wstring> searchDirectories ({
		selfDllDirectory + L"",
		selfDllDirectory + L"External\\"
	});
	HMODULE moduleHandler = 0;
	for (int i = 0; i < searchDirectories.size (); ++i) {
		auto path = searchDirectories[i] + dllFileNameW;
		moduleHandler = LoadLibraryW (path.c_str ());
		if (moduleHandler)
			break;
	}
	if (!moduleHandler)
		throw Exception::LoadModuleFailedException (dllFileNameW, searchDirectories);

	_SubModule subModule;
	subModule._handler = moduleHandler;
	subModule.p_system = ((entryFunc)GetProcAddress (moduleHandler, "entry")) ();
	_Modules.insert (std::make_pair (std::string (name), subModule));
	return subModule;

}



void _SubModule::UnloadModules () {

	for (auto it = _Modules.begin (); it != _Modules.end (); ++it) {
		delete it->second.p_system;
		FreeLibrary ((HMODULE)it->second._handler);
	}
	_Modules.clear ();

}

NS_DLIB_CAM_END