#pragma once
//////////////////////////////////////////////////////////////////////////
// STL
//////////////////////////////////////////////////////////////////////////
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#include <string>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <map>
#include <utility>
#include <hash_map>
#include <initializer_list>
#include <set>
#include <queue>


//////////////////////////////////////////////////////////////////////////
// I / O
//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>

//////////////////////////////////////////////////////////////////////////
// MEMORY
//////////////////////////////////////////////////////////////////////////

#include <memory>
#include <crtdbg.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////
// HELPERS
//////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <locale>
#include <sstream>
#include <thread>

//////////////////////////////////////////////////////////////////////////
// ZILCH
//////////////////////////////////////////////////////////////////////////
#include "Zilch.hpp" //Must be included before Windows.

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// WINDOWS
//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32

#define NOMINMAX //Zilch defines these.
#define _WINSOCKAPI_ //For Zilch Debugger
#define NO_SHLWAPI_STRFCNS //Zilch also defines these.
#include <WinSock2.h>
#include <windows.h>
#include <windowsx.h>

#endif

//////////////////////////////////////////////////////////////////////////
// NAMESPACES
//////////////////////////////////////////////////////////////////////////
using namespace Zilch;

//////////////////////////////////////////////////////////////////////////
// GRAPHICS
//////////////////////////////////////////////////////////////////////////
#include <d3d11.h>
#pragma comment (lib, "D3DCompiler.lib")
#include <D3Dcompiler.h>
#pragma comment (lib, "d3d11.lib")
#include <DirectXMath.h>
#pragma comment (lib, "DXGI.lib")
#include <DXGI1_2.h>
#pragma comment (lib, "Dxgi.lib")


//////////////////////////////////////////////////////////////////////////
// SINENGINE
//////////////////////////////////////////////////////////////////////////
#include "SinSole.h"
#include "MemoryManager.h"
#include "SinEntity.h"
#include "Utility.h"
#include "ZilchCompiledLib.h"
#include "ArrayRange.h"
#include "Serializer.h"
#include "DataComponent.h"
#include "DataProperty.h"
#include "DataObject.h"
#include "DataLevel.h"
#include "DataFile.h"
#include "ResourceManager.h"

#pragma hdrstop