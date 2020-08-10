#include <Windows.h>

using namespace System;
using namespace System::Windows::Forms;

class CDatParser{
public:
	String^ GetValueForKey( String^ file, String^ key );
	void SetValueForKey( String^ key, String^ value );
};

class CScriptWorker{
private:
	BOOL bCanExecute;
public:
	CScriptWorker(){ bCanExecute = TRUE; };
	void ExecuteLine( String^ input );
	void TerminateScript( void );
};