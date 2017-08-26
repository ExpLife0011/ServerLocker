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
	system("SLDaemon.exe ServerLocker.exe");
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
	now = time(0);
	fprintf(fFile, "%d[%s] :Started SLDaemon.exe.",now,runname);
	now = time(0);
	fprintf(fFile, "%d[%s] :Return 0.", now, runname);
	fclose(fFile);
    return 0;
}

