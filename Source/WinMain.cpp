#pragma once
#include "Precompiled.h"
#include "Shlwapi.h"
#include <io.h>
#include <fcntl.h>
#include "SinSole.h"

#define SINBUILD
#ifdef SINBUILD

int main(void)
{
	SinSole::CreateConsole("SinSole");

	std::cout << "---- SinSole Initialized ----" << std::endl;

	system("pause");

	SinSole::DestroyConsole();
	return 0;
}

#endif