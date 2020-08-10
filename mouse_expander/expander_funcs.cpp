#include "expander_funcs.h"

String^ CDatParser::GetValueForKey( String^ file, String^ key ) {
	array< String^ >^ dat_contents = IO::File::ReadAllLines( file );
	for( INT i = 0; i < dat_contents->Length; i++ ) {
		if( !dat_contents[i]->Contains("//") && !String::IsNullOrEmpty(dat_contents[i]) ) {
			if( dat_contents[i]->Substring(0, dat_contents[i]->IndexOf('=')-1) == key ) {
				return dat_contents[i]->Substring( dat_contents[i]->IndexOf('=')+2, dat_contents[i]->Length-(dat_contents[i]->IndexOf('=')+2) );
			}
		}
	}
}

void CScriptWorker::ExecuteLine( String^ input ) {
	if( !bCanExecute ) return;
	POINT curpos;
	INT xpos, ypos;
	String^ axis_data;
	if( input->Contains("MoveMouseTo") ) {
		axis_data = input->Substring( strlen("MoveMouseTo")+1 );
		xpos = Convert::ToInt64( axis_data->Split(',')[0] );
		ypos = Convert::ToInt64( axis_data->Split(',')[1] );
		SetCursorPos( xpos, ypos );
	}
	else if( input->Contains("Wait") ) {
		SleepEx( Convert::ToInt16(input->Substring(5, input->Length-5)), FALSE );
	}
	else if( input->Contains("MouseClick") ) {
		GetCursorPos( &curpos );
		if( Convert::ToInt16(input->Substring(11, input->Length-11)) == 1 ) {
			mouse_event( MOUSEEVENTF_LEFTDOWN, curpos.x, curpos.y, NULL, NULL );
			mouse_event( MOUSEEVENTF_LEFTUP, curpos.x, curpos.y, NULL, NULL );
		}
		else if( Convert::ToInt16(input->Substring(11, input->Length-11)) == 2 ) {
			mouse_event( MOUSEEVENTF_RIGHTDOWN, curpos.x, curpos.y, NULL, NULL );
			mouse_event( MOUSEEVENTF_RIGHTUP, curpos.x, curpos.y, NULL, NULL );
		}
	}
	else if( !input->Contains("Loop") ) {
		MessageBox::Show( "Unknown identifier: "+input+"\nScript will be terminated", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error );
		TerminateScript();
	}
}

void CScriptWorker::TerminateScript( void ) {
	bCanExecute = FALSE;
}

CDatParser pDatParser;
CScriptWorker pScriptWorker;