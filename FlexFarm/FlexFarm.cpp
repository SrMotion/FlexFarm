//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
USEFORM("MainForm.cpp", Form1);
USEFORM("LoginForm.cpp", Form2);
//---------------------------------------------------------------------------
class ProcessMutex {
public:
	bool Acquire(LPCWSTR m_MutexName) {
		if (Handle = OpenMutex(MUTEX_ALL_ACCESS, 0, m_MutexName); !Handle) {
			Handle = CreateMutex(0, 1, m_MutexName);
			return true;
		}
		return false;
	}

	void Release() {
		DuplicateHandle(GetCurrentProcess(), Handle, 0, 0, 0, 0, DUPLICATE_CLOSE_SOURCE);
	}

	HANDLE Handle = 0;
};

int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
        ProcessMutex* Mutex = new ProcessMutex;
		if (Mutex->Acquire(L"2653723859")) {
			Application->Initialize();
			Application->MainFormOnTaskBar = true;
			TStyleManager::TrySetStyle("Glow");
			Application->CreateForm(__classid(TForm1), &Form1);
		Application->CreateForm(__classid(TForm2), &Form2);
		Application->Run();
		}
		else {
			Application->MessageBox(L"FlexFarm is already running!", L"Error", MB_ICONERROR);
		}

	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
