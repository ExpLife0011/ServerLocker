// SLDaemon.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#pragma comment(linker,"/subsystem:\"windows\"  /entry:\"mainCRTStartup\"" )

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

