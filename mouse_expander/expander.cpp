#include <Windows.h>
#include "expander_funcs.h"
#pragma comment( lib, "user32.lib" )
extern CDatParser pDatParser;
extern CScriptWorker pScriptWorker;
#define XDIF_LIMITER pDatParser.GetValueForKey("mouse_expander.dat", "XDifLimiter")
#define YDIF_LIMITER pDatParser.GetValueForKey("mouse_expander.dat", "YDifLimiter")
#define XDIF_START pDatParser.GetValueForKey("mouse_expander.dat", "XDifStart")
#define YDIF_START pDatParser.GetValueForKey("mouse_expander.dat", "YDifStart")
#define ALLOW_MOUSEMOVE pDatParser.GetValueForKey("mouse_expander.dat", "AdditionalMouseFuncs")
#define KEYMOVE_STEP pDatParser.GetValueForKey("mouse_expander.dat", "KeyMoveStep")
#define LOGFILE pDatParser.GetValueForKey("mouse_expander.dat", "LogFile")
#define ALLOW_KEYCONTROLS pDatParser.GetValueForKey("mouse_expander.dat", "KeyControls")

using namespace System;
using namespace System::Windows::Forms;

BOOL IsPressingMoveKeys( void ) {
	return ( GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_RIGHT) &&
		GetAsyncKeyState(VK_UP) && GetAsyncKeyState(VK_DOWN) );
}

INT WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, INT ) {
	if( !IO::File::Exists("mouse_expander.dat") ) {
		MessageBox::Show( "Unable to find \"mouse_expander.dat\" configuration file", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error );
		exit( -1 );
	}
	POINT pCurPos, pLastPos;
	INT xdif, ydif;
	BOOL bLoopScript = FALSE;

	String^ scriptToExec = pDatParser.GetValueForKey("mouse_expander.dat", "ExecScript");
	if( !String::IsNullOrEmpty(LOGFILE) )
		IO::File::AppendAllText( LOGFILE, "\nInitialized log file "+LOGFILE );

	//you better dont change the settings when app is executing
	SetFileAttributesA( "mouse_expander.dat", FILE_ATTRIBUTE_READONLY );

	if( IO::File::Exists(scriptToExec) ) {
		array< String^ >^ pScript = IO::File::ReadAllLines(scriptToExec);
		if( pScript[pScript->Length-1] == "Loop" )
			bLoopScript = TRUE;
		else{
			bLoopScript = FALSE;
			for( INT i = 0; i < pScript->Length; i++ ) {
				if( !String::IsNullOrEmpty(pScript[i]) && !pScript[i]->Contains("//") ) {
					pScriptWorker.ExecuteLine(pScript[i]);
				}
			}
		}
	}

	if( !String::IsNullOrEmpty(LOGFILE) )
		IO::File::AppendAllText( LOGFILE, "\nFound "+scriptToExec+" file?: "+IO::File::Exists(scriptToExec).ToString() );



	while( TRUE ) {
		if( IO::File::Exists(scriptToExec) && bLoopScript ) {
			array< String^ >^ pScript = IO::File::ReadAllLines(scriptToExec);
			for( INT i = 0; i < pScript->Length; i++ ) {
				if( !String::IsNullOrEmpty(pScript[i]) && !pScript[i]->Contains("//") ) {
					pScriptWorker.ExecuteLine(pScript[i]);
				}
			}
		}

		if( GetAsyncKeyState( VK_RSHIFT ) && Convert::ToInt16(ALLOW_KEYCONTROLS) ) {
			SetCursorPos( GetSystemMetrics(SM_CXSCREEN)/2, GetSystemMetrics(SM_CYSCREEN)/2 );
			if( !String::IsNullOrEmpty(LOGFILE) )
				IO::File::AppendAllText( LOGFILE, "\nReset cursor position" );
		}
		if( GetAsyncKeyState( VK_F12 ) ) {
			break;
		}
		if( GetAsyncKeyState( VK_F6 ) ) {
			pScriptWorker.TerminateScript();
			if( !String::IsNullOrEmpty(LOGFILE) )
				IO::File::AppendAllText( LOGFILE, "\nTerminated script" );
		}
		GetCursorPos( &pCurPos );
		if( GetAsyncKeyState( VK_LEFT ) && Convert::ToInt16(ALLOW_KEYCONTROLS) ) {
			pCurPos.x -= Convert::ToInt16(KEYMOVE_STEP);
			if( !String::IsNullOrEmpty(LOGFILE) )
				IO::File::AppendAllText( LOGFILE, "\n\tMove cursor left" );
		}
		if( GetAsyncKeyState( VK_RIGHT ) && Convert::ToInt16(ALLOW_KEYCONTROLS) ) {
			pCurPos.x += Convert::ToInt16(KEYMOVE_STEP);
			if( !String::IsNullOrEmpty(LOGFILE) )
				IO::File::AppendAllText( LOGFILE, "\n\tMove cursor right" );
		}
		if( GetAsyncKeyState( VK_UP ) && Convert::ToInt16(ALLOW_KEYCONTROLS) ) {
			pCurPos.y -= Convert::ToInt16(KEYMOVE_STEP);
			if( !String::IsNullOrEmpty(LOGFILE) )
				IO::File::AppendAllText( LOGFILE, "\n\tMove cursor up" );
		}
		if( GetAsyncKeyState( VK_DOWN ) && Convert::ToInt16(ALLOW_KEYCONTROLS) ) {
			pCurPos.y += Convert::ToInt16(KEYMOVE_STEP);
			if( !String::IsNullOrEmpty(LOGFILE) )
				IO::File::AppendAllText( LOGFILE, "\n\tMove cursor down" );
		}

		SetCursorPos( pCurPos.x, pCurPos.y );

		if( Convert::ToInt16(ALLOW_MOUSEMOVE) ) {
			if( ((pCurPos.x+pCurPos.y) != (pLastPos.x+pLastPos.y)) && !IsPressingMoveKeys() ) {
				xdif = pCurPos.x-pLastPos.x;
				ydif = pCurPos.y-pLastPos.y;
				if( (xdif < Convert::ToInt16(XDIF_LIMITER) && ydif < Convert::ToInt16(YDIF_LIMITER))
				&& (xdif > Convert::ToInt16(XDIF_START) && ydif > Convert::ToInt16(YDIF_START)) )
					SetCursorPos( pCurPos.x+xdif, pCurPos.y+ydif );
			}
		}

		if( GetAsyncKeyState( VK_RETURN ) && Convert::ToInt16(ALLOW_KEYCONTROLS) ) {
			mouse_event( MOUSEEVENTF_LEFTDOWN, pCurPos.x, pCurPos.y, NULL, NULL );
			mouse_event( MOUSEEVENTF_LEFTUP, pCurPos.x, pCurPos.y, NULL, NULL );
			if( !String::IsNullOrEmpty(LOGFILE) )
				IO::File::AppendAllText( LOGFILE, "\n\tMouse click at "+pCurPos.x.ToString()+", "+pCurPos.y.ToString() );
			SleepEx( 400, FALSE );
		}

		pLastPos = pCurPos;
		SleepEx( 10, FALSE );
	}
	return TRUE;
}