#include <Windows.h>
#include "indicator.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace mouse_pos;

INT WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, INT ) {
	Application::Run( gcnew indicator() );
	return TRUE;
}