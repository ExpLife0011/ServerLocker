// Run.cpp : �������̨Ӧ�ó������ڵ㡣
// For run ServerLocker System
//

#include "stdafx.h"

#pragma comment(linker,"/subsystem:\"windows\"  /entry:\"mainCRTStartup\"" )

using namespace std;

FILE *fFile;
time_t now;
char *runname = "Run";

int main()
{
	STARTUPINFO si;
	DWORD returnCode;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	CString cmd;
	cmd.Format(L"SLDaemon.exe ServerLocker.exe");
	LPWSTR lpCmd = cmd.GetBuffer();
	ifstream fin("status.log");
	if (!fin)
	{
		char *fileName = "status.log";
		ifstream fin("status.log");
		if (!fin)
		{
			MessageBox(NULL,_T("Can not create a new log file,Return!"), _T("Error!"), MB_ICONERROR);
			return -1;
		}
		fFile = fopen(fileName, "w");
	}
	// �����ӽ��̣��ж��Ƿ�ִ�гɹ� 
	if (!CreateProcess(NULL, lpCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		now = time(0);
		fprintf(fFile, "%Id[%s] :Failed to create sub-process, Return!", now, runname);
		return -1;
	}
	fprintf(fFile, "%lld[%s] :Started SLDaemon.exe.", now, runname);
	now = time(0);
	// ����ִ�гɹ�����ӡ������Ϣ 
	now = time(0);
	fprintf(fFile, "%Id[%s] :Process is successful and prints the process information.", now);
	//cout << "�������ӽ��̵���Ϣ��" << endl;
	now = time(0);
	fprintf(fFile, "%Id[%s] :Process ID pi.dwProcessID %Id.", now, runname, pi.dwProcessId);
	//cout << "����ID pi.dwProcessID: " << pi.dwProcessId << endl;
	now = time(0);
	fprintf(fFile, "%Id[%s] :dwThread ID pi.dwThreadId %Id.", now, runname, pi.dwThreadId);
	//cout << "�߳�ID pi.dwThreadID : " << pi.dwThreadId << endl;
	now = time(0);
	fprintf(fFile, "%Id[%s] :Output is finished!", now, runname);
	now = time(0);
	fprintf(fFile, "%lld[%s] :Return 0.", now, runname);
	fclose(fFile);
    return 0;
}

