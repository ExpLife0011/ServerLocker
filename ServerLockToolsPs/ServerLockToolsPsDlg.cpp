
// ServerLockToolsPsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ServerLockToolsPs.h"
#include "ServerLockToolsPsDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <windows.h>


#pragma warning(disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//����ȫ�ֱ���
CBrush m_bkbrush;    //��ˢ
int user = 0;//�ж��û�״̬ ���� ����
CString SetPassword;   //��������
CString ConfirmPassword;   //ȷ������
CString UnlockPassword;   //��������
std::wstring strValue;
int ru=0;
char *ch;
char content[256]; //����ѯע����ֵ������
DWORD dwType = REG_SZ; //�����ȡ��������
DWORD dwLength = 256;
struct HKEY__*RootKey; //ע�����������
TCHAR *SubKey; //����ע�����ĵ�ַ
TCHAR *KeyName; //�������������
TCHAR *ValueName; //������ֵ������
LPBYTE SetContent_S; //�ַ�������
CRect rct; //������Ļ����
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);


int ShowContent(struct HKEY__*ReRootKey, TCHAR *ReSubKey, TCHAR *ReValueName);

//void WindowsManager()
//{
//	HWND    hwnd;
//	int     iItem = 0;
//	LVITEM lvitem, *plvitem;
//	char    ItemBuf[512], *pItem;
//	DWORD   PID;
//	HANDLE hProcess;
//
//	// �������������ListView���ھ��   
//	hwnd = FindWindow("#32770", _T("Windows ���������"));
//	hwnd = FindWindowEx(hwnd, 0, "#32770", 0);
//	hwnd = FindWindowEx(hwnd, 0, "SysListView32", 0);
//
//	// Windows�����������δ�����򷵻�   
//	if (!hwnd)
//		return;
//	else
//	{
//		// û��ָ��Ŀ������򷵻�   
//		iItem = SendMessage(hwnd, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
//		if (iItem == -1)
//			return;
//		else
//		{
//			GetWindowThreadProcessId(hwnd, &PID);
//
//			// ��ȡ���̾������ʧ���򷵻�   
//			hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, PID);
//			if (!hProcess)
//				return;
//			else
//			{
//				plvitem = (LVITEM*)VirtualAllocEx(hProcess, NULL, sizeof(LVITEM), MEM_COMMIT, PAGE_READWRITE);
//				pItem = (char*)VirtualAllocEx(hProcess, NULL, 512, MEM_COMMIT, PAGE_READWRITE);
//
//				// �޷������ڴ��򷵻�   
//				if ((!plvitem) || (!pItem))
//					return;
//				else
//				{
//					lvitem.cchTextMax = 512;
//					//lvitem.iSubItem=1;//PID   
//					lvitem.iSubItem = 0; //ProcessName   
//					lvitem.pszText = pItem;
//					WriteProcessMemory(hProcess, plvitem, &lvitem, sizeof(LVITEM), NULL);
//					SendMessage(hwnd, LVM_GETITEMTEXT, (WPARAM)iItem, (LPARAM)plvitem);
//					ReadProcessMemory(hProcess, pItem, ItemBuf, 512, NULL);
//
//					// �Ƚ��ַ���
//					CString str = (CString)ItemBuf;
//					if (str.CompareNoCase(_T("ServerLockToolsPs.exe")) == 0)
//					{
//						HWND hWnd = FindWindow(NULL, _T("Windows ���������"));
//						SendMessage(hWnd, WM_DESTROY, 0, 0);
//						Sleep(10);
//						MessageBox(NULL, _T("��ֹ�ر�ϵͳ�ؼ����̣�"), _T("��ʾ"), MB_ICONERROR | MB_OK);
//					}
//				}
//			}
//		}
//	}
//
//	//�ͷ��ڴ�   
//	CloseHandle(hwnd);
//	CloseHandle(hProcess);
//	VirtualFreeEx(hProcess, plvitem, 0, MEM_RELEASE);
//	VirtualFreeEx(hProcess, pItem, 0, MEM_RELEASE);
//}
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CServerLockToolsPsDlg �Ի���



CServerLockToolsPsDlg::CServerLockToolsPsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServerLockToolsPsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerLockToolsPsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CServerLockToolsPsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SETLOCK, &CServerLockToolsPsDlg::OnBnClickedSetlock)
	ON_BN_CLICKED(IDC_EXITSYSTEM, &CServerLockToolsPsDlg::OnBnClickedExitsystem)
END_MESSAGE_MAP()

// CServerLockToolsPsDlg ��Ϣ�������

BOOL CServerLockToolsPsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	
	//��Ȩ

	HANDLE hToken;
	//�򿪵�ǰ���̵ķ�������
	if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		return FALSE;
	}
	TOKEN_PRIVILEGES tkp;
	//�鿴��ǰ����Ȩ��

	if(!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid))
	{
		return FALSE;
	}
	//�޸���Ȩ����
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	//�޸ķ�������
	if (AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0))
	{
		char regname[] = "Software";
		char ValueName[] = "InitPassword";
		HKEY hKey= HKEY_LOCAL_MACHINE;
		HKEY subKey;
		BYTE ValueData[64];
		DWORD Buffer;
		//��       
		if (RegOpenKeyEx(hKey, regname, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		{
			auto resu = RegCreateKey(hKey, "ServerLockToolsPs", &subKey);
		}
		RegCloseKey(hKey);
		if (RegOpenKeyEx(hKey, "Software//ServerLockToolsPs", 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		{
			MessageBox("Ȩ�޲��㣬�޷���ע���", "����", MB_ICONERROR);
			OnOK();
		}
		else
		{
			//��ԭ���ļ�ֵ       
			Buffer = sizeof(ValueData);
			if (RegQueryValueEx(hKey, ValueName, 0, NULL, ValueData, &Buffer) != ERROR_SUCCESS)
			{
				//������       
				//�½�һ��ֵΪ0��SZ       
				char *temp = 0;
				if (RegSetValueEx(hKey, ValueName, 0, REG_SZ, (BYTE *)temp, sizeof(temp)) != ERROR_SUCCESS)
				{
					MessageBox("�޷��½�ע���ֵ��", "����", MB_ICONERROR);
					OnOK();
				}
				ru = 1;
				RegCloseKey(hKey);
			}
			else
			{
			//	//����       
			//	//�ı�ֵ
			//	DWORD temp = 0;
			//	if (RegSetValueEx(hKey, ValueName, 0, REG_DWORD, (LPBYTE)&temp, sizeof(DWORD)) != ERROR_SUCCESS)
			//	{
			//		MessageBox("�޷�����ע���ֵ��", "����", MB_ICONERROR);
			//		RegCloseKey(hKey);
			//		OnOK();
		//    }
				HookLoad();    //    ����HOOK
				ShowContent(hKey, regname, ValueName);
				SetPassword = content;
				ConfirmPassword = content;
				SetDlgItemText(IDC_MESSAGE, "���ϴ�δ�������\n����֤���ϴ����õ�����!");
				SendDlgItemMessage(IDC_SET, EM_SETREADONLY, 1);
				SendDlgItemMessage(IDC_SETAGIN, EM_SETREADONLY, 1);
				SendDlgItemMessage(IDC_UNLOCK, EM_SETREADONLY, 0);
				SetDlgItemText(IDC_SETLOCK, "ϵͳ����");
				SetDlgItemText(IDC_SET, "");
				SetDlgItemText(IDC_SETAGIN, "");
				GetDlgItem(IDC_EXITSYSTEM)->EnableWindow(false);
				user = 1;
				GetWindowRect(rct); //��������Ͻǵ���Ļλ��
									//�����������Ļ�ϵľ�������
				ClipCursor(rct);
				//���µ�ϵͳ��������д���û������ļ�
				SystemParametersInfo(SPI_SETSCREENSAVERRUNNING, true, 0, SPIF_UPDATEINIFILE);
				//������Ǻ���ʵ��
				::ShowWindow(::FindWindow("Shell_TrayWnd", NULL), SW_HIDE);
				GetDlgItem(IDC_UNLOCK)->SetFocus();
				//HANDLE hThread = CreateThread(NULL, 0, Fun, NULL, 0, NULL);
				return TRUE;
			}
		}
	}
	else
	{
		MessageBox("�㲻�߱�����Ա��ݣ�", "����", MB_OK|MB_ICONERROR);
		return FALSE;
		OnOK();
	}

	////����
	//if (!ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0))
	//{
	//	return FALSE;
	//}
	//��ֹ�ؼ�ʹ��
	SendDlgItemMessage(IDC_UNLOCK, EM_SETREADONLY, 1);
	//�ı��Ӵ���
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CServerLockToolsPsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CServerLockToolsPsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CServerLockToolsPsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CServerLockToolsPsDlg::OnBnClickedSetlock()
{
	// TODO:  �ڴ���Ӽ������ܴ���������
	if (user == 0)   //����
	{
		GetDlgItemText(IDC_SET, SetPassword); //�û����������ֱ�Ӵ洢��SetPassword
		GetDlgItemText(IDC_SETAGIN, ConfirmPassword);//���û������ȷ������洢��ConfirmPassword
		if (SetPassword != ConfirmPassword)
		{
			SetDlgItemText(IDC_MESSAGE, "�������벻һ�£�\n�޷��ɹ�����!");
			SetDlgItemText(IDC_SET, "");
			SetDlgItemText(IDC_SETAGIN, "");

			//�ѹ��ֱ�Ӷ�λ������IDC_SET�ؼ�
			GetDlgItem(IDC_SET)->SetFocus();
			return;
		}
		if (SetPassword == "")//�ж��Ƿ�Ϊ������
		{
			SetDlgItemText(IDC_MESSAGE, "���벻��Ϊ�գ�\n���Ƿ�������ģ�");
			//�ѹ��ֱ�Ӷ�λ������IDC_SET�ؼ�
			GetDlgItem(IDC_SET)->SetFocus();
			return;
		}
		char *temp = SetPassword.GetBuffer(SetPassword.GetLength());
		char regname[] = "Software\\ServerLockToolsPs";
		char ValueName[] = "InitPassword";
		HKEY hKey = HKEY_LOCAL_MACHINE;
		if (RegOpenKeyEx(hKey, regname, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		{
			if (RegCreateKeyEx(hKey, regname, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS)
				MessageBox("Ȩ�޲��㣬�޷���ע���", "����", MB_ICONERROR);
			OnOK();
		}
		else
		{
			if (RegSetValueEx(hKey, ValueName, 0, REG_SZ, (BYTE *)temp, sizeof(temp)) != ERROR_SUCCESS)
			{
				MessageBox("�޷�����ע���ֵ��", "����", MB_ICONERROR);
				RegCloseKey(hKey);
				//OnOK();
			}
		}
		SetDlgItemText(IDC_MESSAGE, "����ϵͳ�ѳɹ�������\n����������");
		SendDlgItemMessage(IDC_SET, EM_SETREADONLY, 1); 
		SendDlgItemMessage(IDC_SETAGIN, EM_SETREADONLY, 1);
		SendDlgItemMessage(IDC_UNLOCK, EM_SETREADONLY, 0);
		SetDlgItemText(IDC_SETLOCK, "ϵͳ����");
		SetDlgItemText(IDC_SET, "");
		SetDlgItemText(IDC_SETAGIN, "");
		GetDlgItem(IDC_EXITSYSTEM)->EnableWindow(false);
		user = 1;
		GetWindowRect(rct); //��������Ͻǵ���Ļλ��
		//�����������Ļ�ϵľ�������
		ClipCursor(rct);
		//���µ�ϵͳ��������д���û������ļ�
		SystemParametersInfo(SPI_SETSCREENSAVERRUNNING, true, 0, SPIF_UPDATEINIFILE);
		//������Ǻ���ʵ��
		::ShowWindow(::FindWindow("Shell_TrayWnd", NULL), SW_HIDE);
		GetDlgItem(IDC_UNLOCK)->SetFocus();
		return;
	}

	if (user == 1)   //����
	{
		GetDlgItemText(IDC_UNLOCK, UnlockPassword);
		if (SetPassword != UnlockPassword)
		{
			SetDlgItemText(IDC_MESSAGE, "���������������\n�޷���ȷ������");
			SetDlgItemText(IDC_UNLOCK, "");
			GetDlgItem(IDC_UNLOCK)->SetFocus(); //��궨λ
			return;
		}
		if (SetPassword == "")//�ж��Ƿ�Ϊ������
		{
			SetDlgItemText(IDC_MESSAGE, "�������벻��Ϊ�գ�\n���Ƿ�������ģ�");
			//�ѹ��ֱ�Ӷ�λ������IDC_UNLOCK�ؼ�
			GetDlgItem(IDC_UNLOCK)->SetFocus();
			return;
		}
		SetDlgItemText(IDC_MESSAGE, "�����ɹ���\n�����Ұ�æ��");
		SendDlgItemMessage(IDC_SET, EM_SETREADONLY, 0);
		SendDlgItemMessage(IDC_SETAGIN, EM_SETREADONLY, 0);
		SendDlgItemMessage(IDC_UNLOCK, EM_SETREADONLY, 1);
		SetDlgItemText(IDC_SETLOCK, "ʵ�ּ���");
		SetDlgItemText(IDC_UNLOCK, "");
		GetDlgItem(IDC_EXITSYSTEM)->EnableWindow(true);
		user = 0;
		ClipCursor(NULL);
		SystemParametersInfo(SPI_SETSCREENSAVERRUNNING, false, 0,SPIF_UPDATEINIFILE);
		::ShowWindow(::FindWindow("Shell_TrayWnd", NULL), SW_SHOW);

	}
}
BOOL CServerLockToolsPsDlg::PreTranslateMessage(MSG * pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		//����ָ������  
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
			return FALSE;
		case VK_RETURN:
			return FALSE;
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
void CServerLockToolsPsDlg::OnBnClickedExitsystem()
{
	OnOK();
}
int ShowContent(struct HKEY__*ReRootKey, TCHAR *ReSubKey, TCHAR *ReValueName)
{
	HKEY hKey = HKEY_LOCAL_MACHINE;
	int i = 0; //���������0==succeed
	if (RegOpenKeyEx(hKey, ReSubKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		if (RegQueryValueEx(hKey, ReValueName, NULL, &dwType, (unsigned char *)content, &dwLength) != ERROR_SUCCESS)
		{
			AfxMessageBox("�����޷���ѯ�йص�ע�����Ϣ");
			i = 1;
		}
		RegCloseKey(hKey);
	}
	else
	{
		AfxMessageBox("�����޷����йص�hKEY");
		i = 1;
	}
	return i;
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
void CServerLockToolsPsDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	HookUnload();    // �˳����ڣ�Ҫж��HOOK
	CDialogEx::OnClose();
}

void CServerLockToolsPsDlg::HookLoad()
{
	m_hinstHookDll = LoadLibrary(_T("MonitorDll.dll"));
	CString loginfo;

	if (NULL == m_hinstHookDll)
	{
		loginfo.Format(_T("���� MonitorDll.dllʧ�ܣ�������� = [%d] "), GetLastError());
		AfxMessageBox(loginfo);
		return;
	}


	typedef BOOL(WINAPI* LoadMonitor)(HWND hwnd, DWORD dwProcessId);
	LoadMonitor loadMonitor = NULL;
	loadMonitor = (LoadMonitor)::GetProcAddress(m_hinstHookDll, "HookLoad");
	if (NULL == loadMonitor)
	{
		loginfo.Format(_T("��ȡ���� HookLoad ʧ�ܣ�������� = [%d]"), GetLastError());
		AfxMessageBox(loginfo);
	}
	if (loadMonitor(m_hWnd, GetCurrentProcessId()))
	{
		loginfo.Format(_T("HOOK���سɹ�"));
		AfxMessageBox(loginfo);
	}
	else
	{
		loginfo.Format(_T("HOOK����ʧ��"));
		AfxMessageBox(loginfo);
	}
}
void CServerLockToolsPsDlg::HookUnload()
{
	CString logInfo;
	if (m_hinstHookDll == NULL)
	{
		m_hinstHookDll = LoadLibrary(_T("MonitorDll.dll"));
		if (NULL == m_hinstHookDll)
		{
			logInfo.Format(_T("���� MonitorDll.dllʧ�ܣ�������� = [%d]"), GetLastError());
			AfxMessageBox(logInfo);
			return;
		}
	}

	typedef VOID(WINAPI* UnloadHook)();
	UnloadHook unloadHook = NULL;
	unloadHook = (UnloadHook)::GetProcAddress(m_hinstHookDll, "HookUnload");
	if (NULL == unloadHook)
	{
		logInfo.Format(_T("��ȡ���� HookUnload ʧ�ܣ�������� = [%d]"), GetLastError());
		AfxMessageBox(logInfo);
		return;
	}

	unloadHook();

}