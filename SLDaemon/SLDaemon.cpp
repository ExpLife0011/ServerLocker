// SLDaemon.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "SLDaemon.h"

HINSTANCE m_hinstHookDll;    //    MonitorDll��ʵ�����
void HookLoad();            //    ����HOOK      
BOOL installhook();
void HookUnload();            //    ж��HOOK

using namespace std; 

int main(int argc, char *argv[])
{
	STARTUPINFO si;
	DWORD returnCode;
	PROCESS_INFORMATION pi;		//������Ϣ 
	CString cmd;
	cmd.Format(L"cmd / c %s", argv[1]);
	LPWSTR lpCmd = cmd.GetBuffer();
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	do {
		// �����ӽ��̣��ж��Ƿ�ִ�гɹ� 
		if (!CreateProcess(NULL, lpCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			return -1;
		}
		//����ִ�гɹ�����ӡ������Ϣ 
		//cout << "�������ӽ��̵���Ϣ��" << endl;
		//cout << "����ID pi.dwProcessID: " << pi.dwProcessId << endl;
		//cout << "�߳�ID pi.dwThreadID : " << pi.dwThreadId << endl;
		// �ȴ�֪���ӽ����˳�... 
		WaitForSingleObject(pi.hProcess, INFINITE);// �������Ƿ�ֹͣ 
												   // WaitForSingleObject()�����������״̬�������δȷ������ȴ�����ʱ 
												   // �ӽ����˳� 
		/*cout << "�ӽ����Ѿ��˳�" << endl;*/
		//��ȡ�ӽ��̵ķ���ֵ
		GetExitCodeProcess(pi.hProcess, &returnCode);
		/*std::cout << "process return code:" << returnCode << std::endl;*/
		// �رս��̺;�� 
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		if (returnCode == 0)
			return 0;
	} while (true);				// ��������˳����ٴ�ִ�з��� 
}

BOOL installhook()
{
	HINSTANCE hins = AfxGetInstanceHandle();
	HHOOK Hook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProc, hins, 0);
	return (BOOL)Hook;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	BOOL Discard = FALSE;

	if (nCode == HC_ACTION)
	{
		switch (wParam)
		{
		case WM_KEYDOWN:  case WM_SYSKEYDOWN:
		case WM_KEYUP:    case WM_SYSKEYUP:
			PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
			Discard =
				//Tab + Alt
				((p->vkCode == VK_TAB) && ((p->flags & LLKHF_ALTDOWN) != 0))
				//Esc + Alt
				|| ((p->vkCode == VK_ESCAPE) && ((p->flags & LLKHF_ALTDOWN) != 0))
				//Esc + Ctrl
				|| ((p->vkCode == VK_ESCAPE) && ((GetKeyState(VK_CONTROL) & 0x8000) != 0))
				//Ctrl + Space
				|| (((GetKeyState(VK_CONTROL) & 0x8000) != 0) && (p->vkCode == VK_SPACE));
			break;
		}
	}
	return(Discard ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
}

void HookLoad()
{
	m_hinstHookDll = LoadLibrary(_T("MonitorDll.dll"));
	CString loginfo;

	if (NULL == m_hinstHookDll)
	{
		loginfo.Format(_T("Load MonitorDll.dll failed with error code = [%d] "), GetLastError());
		AfxMessageBox(loginfo);
		return;
	}


	typedef BOOL(WINAPI* LoadMonitor)(HWND hwnd, DWORD dwProcessId);
	LoadMonitor loadMonitor = NULL;
	loadMonitor = (LoadMonitor)::GetProcAddress(m_hinstHookDll, "HookLoad");
	if (NULL == loadMonitor)
	{
		loginfo.Format(_T("Get function HookLoad failed with error code = [%d]"), GetLastError());
		AfxMessageBox(loginfo);
	}
	if (loadMonitor(m_hWnd, GetCurrentProcessId()))
	{
		loginfo.Format(_T("HOOK loaded successfully"));
		AfxMessageBox(loginfo);
	}
	else
	{
		loginfo.Format(_T("HOOK loading failed!"));
		AfxMessageBox(loginfo);
	}
}
void HookUnload()
{
	CString logInfo;
	if (m_hinstHookDll == NULL)
	{
		m_hinstHookDll = LoadLibrary(_T("MonitorDll.dll"));
		if (NULL == m_hinstHookDll)
		{
			logInfo.Format(_T("Load MonitorDll.dll failed with error code = [%d]"), GetLastError());
			AfxMessageBox(logInfo);
			return;
		}
	}

	typedef VOID(WINAPI* UnloadHook)();
	UnloadHook unloadHook = NULL;
	unloadHook = (UnloadHook)::GetProcAddress(m_hinstHookDll, "HookUnload");
	if (NULL == unloadHook)
	{
		logInfo.Format(_T("Get function HookLoad failed with error code = [%d]"), GetLastError());
		AfxMessageBox(logInfo);
		return;
	}

	unloadHook();

}
