// ServerLockerDlg.h : ͷ�ļ�
//

#include <windows.h>

#pragma once

HINSTANCE m_hinstHookDll;    //    MonitorDll��ʵ�����
void HookLoad();            //    ����HOOK      
BOOL installhook();
void HookUnload();            //    ж��HOOK