// ServerLockerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ServerLocker.h"
#include "ServerLockerDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <windows.h>
#include "openssl/sha.h"
#include <string>

#pragma comment(lib,"libcrypto64MT.lib")
#pragma comment(lib,"libssl64MT.lib")

using namespace std;

#pragma warning(disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//Declare global variables
CBrush m_bkbrush;
int user = 0;		//Determine the user status 0=Unlocked 1=Locked
CString SetPassword;
CString ConfirmPassword;
CString UnlockPassword;
std::wstring strValue;
int ru=0;
char *ch;
char content[256];     //Query the contents of the registry key
DWORD dwType = REG_SZ; //Define data type
DWORD dwLength = 256;
struct HKEY__*RootKey; //注册表主键名称
TCHAR *SubKey;         //欲打开注册表项的地址
TCHAR *KeyName;        //欲设置项的名字
TCHAR *ValueName;      //欲设置值的名称
LPBYTE SetContent_S;   //字符串类型
CRect rct;             //控制屏幕区域
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CServerLockerDlg 对话框



CServerLockerDlg::CServerLockerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServerLockerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerLockerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CServerLockerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SETLOCK, &CServerLockerDlg::OnBnClickedSetlock)
	ON_BN_CLICKED(IDC_EXITSYSTEM, &CServerLockerDlg::OnBnClickedExitsystem)
END_MESSAGE_MAP()

// CServerLockToolsPsDlg 消息处理程序


BOOL CServerLockerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	
	//提权

	HANDLE hToken;
	//打开当前进程的访问令牌
	if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		return FALSE;
	}
	TOKEN_PRIVILEGES tkp;
	//查看当前进程权限

	if(!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid))
	{
		return FALSE;
	}
	//修改特权数组
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	//修改访问令牌
	if (AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0))
	{
		char regname[] = "Software\\ServerLockToolsPs";
		char ValueName[] = "InitPassword";
		HKEY hKey= HKEY_LOCAL_MACHINE;
		HKEY subKey,subCKey;
		BYTE ValueData[64];
		DWORD Buffer;
		//打开       
		//if (RegOpenKeyEx(hKey, regname, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		//{
		//	auto resu = RegCreateKey(hKey, "ServerLockToolsPs", &subKey);
		//	CString mess;
		//	mess.Format("%d", resu);
		//	MessageBox(mess, "Message", MB_OK);
		//	RegCloseKey(hKey);
		//}	
		if(RegOpenKeyEx(hKey, regname, 0, KEY_ALL_ACCESS, &subKey) != ERROR_SUCCESS)
		{
			MessageBox("权限不足，无法打开注册表！", "错误", MB_ICONERROR);
			OnOK();
		}
		else
		{
			//读原来的键值       
			Buffer = sizeof(ValueData);
			if (RegQueryValueEx(hKey, ValueName, 0, NULL, ValueData, &Buffer) != ERROR_SUCCESS)
			{
				//不存在       
				//新建一个值为0的SZ       
				if (RegCreateKey(hKey, regname, &subCKey) != ERROR_SUCCESS)
				{
					MessageBoxEx(NULL, "无法新建注册表值", "错误", MB_OK | MB_ICONERROR, NULL);
					OnOK();
				}
				else
				{
					//SUCCESS
					char *temp = "";
					if (RegSetValueEx(subCKey, ValueName, 0, REG_SZ, (const unsigned char *)temp, sizeof(temp)) != ERROR_SUCCESS)
					{
						MessageBoxEx(NULL, "未知错误(1)", "错误", MB_OK | MB_ICONERROR, NULL);
						OnOK();
					}
				}
				ru = 1;
				RegCloseKey(hKey);
			}
			else
			{
			//	//存在       
			//	//改变值
			//	DWORD temp = 0;
			//	if (RegSetValueEx(hKey, ValueName, 0, REG_DWORD, (LPBYTE)&temp, sizeof(DWORD)) != ERROR_SUCCESS)
			//	{
			//		MessageBox("无法更改注册表值！", "错误", MB_ICONERROR);
			//		RegCloseKey(hKey);
			//		OnOK();
		    //}
				HookLoad();    //    加载HOOK
				ShowContent(hKey, regname, ValueName);
				string stemp = sha256(content);
				ConfirmPassword = SetPassword = stemp.c_str();
				SetDlgItemText(IDC_MESSAGE, "因上次未解除锁定\n请验证您上次设置的密码!");
				SendDlgItemMessage(IDC_SET, EM_SETREADONLY, 1);
				SendDlgItemMessage(IDC_SETAGIN, EM_SETREADONLY, 1);
				SendDlgItemMessage(IDC_UNLOCK, EM_SETREADONLY, 0);
				SetDlgItemText(IDC_SETLOCK, "系统解锁");
				SetDlgItemText(IDC_SET, "");
				SetDlgItemText(IDC_SETAGIN, "");
				GetDlgItem(IDC_EXITSYSTEM)->EnableWindow(false);
				user = 1;
				GetWindowRect(rct); //相对于左上角的屏幕位置
									//鼠标限制在屏幕上的矩形区域
				ClipCursor(rct);
				//把新的系统设置内容写入用户配置文件
				SystemParametersInfo(SPI_SETSCREENSAVERRUNNING, true, 0, SPIF_UPDATEINIFILE);
				//操作外壳函数实现
				::ShowWindow(::FindWindow("Shell_TrayWnd", NULL), SW_HIDE);
				GetDlgItem(IDC_UNLOCK)->SetFocus();
				//HANDLE hThread = CreateThread(NULL, 0, Fun, NULL, 0, NULL);
				return TRUE;
			}
		}
	}
	else
	{
		MessageBox("你不具备管理员身份！", "错误", MB_OK|MB_ICONERROR);
		return FALSE;
		OnOK();
	}

	////重启
	//if (!ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0))
	//{
	//	return FALSE;
	//}
	//禁止控件使用
	SendDlgItemMessage(IDC_UNLOCK, EM_SETREADONLY, 1);
	//改变子窗口
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CServerLockerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServerLockerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CServerLockerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CServerLockerDlg::OnBnClickedSetlock()
{
	// TODO:  在此添加加锁功能处理程序代码
	if (user == 0)   //加锁
	{
		GetDlgItemText(IDC_SET, SetPassword); //用户输入的密码直接存储至SetPassword
		GetDlgItemText(IDC_SETAGIN, ConfirmPassword);//把用户输入的确认密码存储至ConfirmPassword
		if (SetPassword != ConfirmPassword)
		{
			SetDlgItemText(IDC_MESSAGE, "两次密码不一致！\n无法成功加锁!");
			SetDlgItemText(IDC_SET, "");
			SetDlgItemText(IDC_SETAGIN, "");

			//把光标直接定位到输入IDC_SET控件
			GetDlgItem(IDC_SET)->SetFocus();
			return;
		}
		if (SetPassword == "")//判断是否为空密码
		{
			SetDlgItemText(IDC_MESSAGE, "密码不能为空！\n这是否是有意的？");
			//把光标直接定位到输入IDC_SET控件
			GetDlgItem(IDC_SET)->SetFocus();
			return;
		}
		char *temp = SetPassword.GetBuffer(SetPassword.GetLength());
		string stemp = sha256(temp);
		ConfirmPassword = SetPassword = stemp.c_str();
		const char *ctemp = stemp.c_str();
		strcpy(temp, ctemp);
		char regname[] = "Software\\ServerLockToolsPs";
		char ValueName[] = "InitPassword";
		HKEY hKey = HKEY_LOCAL_MACHINE;
		if (RegOpenKeyEx(hKey, regname, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		{
			if (RegCreateKeyEx(hKey, regname, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS)
			{
				MessageBox("权限不足，无法打开注册表！", "错误", MB_ICONERROR);
				OnOK();
			}
		}
		else
		{
			if (RegSetValueEx(hKey, ValueName, 0, REG_SZ, (const unsigned char *)temp, sizeof(temp)) != ERROR_SUCCESS)
			{
				MessageBox("无法更改注册表值！", "错误", MB_ICONERROR);
				RegCloseKey(hKey);
				OnOK();
			}
		}
		SetDlgItemText(IDC_MESSAGE, "操作系统已成功加锁！\n请早点回来！");
		SendDlgItemMessage(IDC_SET, EM_SETREADONLY, 1); 
		SendDlgItemMessage(IDC_SETAGIN, EM_SETREADONLY, 1);
		SendDlgItemMessage(IDC_UNLOCK, EM_SETREADONLY, 0);
		SetDlgItemText(IDC_SETLOCK, "系统解锁");
		SetDlgItemText(IDC_SET, "");
		SetDlgItemText(IDC_SETAGIN, "");
		GetDlgItem(IDC_EXITSYSTEM)->EnableWindow(false);
		user = 1;
		GetWindowRect(rct); //相对于左上角的屏幕位置
		//鼠标限制在屏幕上的矩形区域
		ClipCursor(rct);
		//把新的系统设置内容写入用户配置文件
		SystemParametersInfo(SPI_SETSCREENSAVERRUNNING, true, 0, SPIF_UPDATEINIFILE);
		//操作外壳函数实现
		::ShowWindow(::FindWindow("Shell_TrayWnd", NULL), SW_HIDE);
		GetDlgItem(IDC_UNLOCK)->SetFocus();
		return;
	}

	if (user == 1)   //解锁
	{
		GetDlgItemText(IDC_UNLOCK, UnlockPassword);
		char *temp = UnlockPassword.GetBuffer(UnlockPassword.GetLength());
		string stemp = sha256(temp);
		UnlockPassword = stemp.c_str();
		if (SetPassword != UnlockPassword)
		{
			SetDlgItemText(IDC_MESSAGE, "解锁密码输入错误！\n无法正确解锁！");
			SetDlgItemText(IDC_UNLOCK, "");
			GetDlgItem(IDC_UNLOCK)->SetFocus(); //鼠标定位
			return;
		}
		if (SetPassword == "")//判断是否为空密码
		{
			SetDlgItemText(IDC_MESSAGE, "解锁密码不能为空！\n这是否是有意的？");
			//把光标直接定位到输入IDC_UNLOCK控件
			GetDlgItem(IDC_UNLOCK)->SetFocus();
			return;
		}
		SetDlgItemText(IDC_MESSAGE, "解锁成功！\n不用我帮忙？");
		SendDlgItemMessage(IDC_SET, EM_SETREADONLY, 0);
		SendDlgItemMessage(IDC_SETAGIN, EM_SETREADONLY, 0);
		SendDlgItemMessage(IDC_UNLOCK, EM_SETREADONLY, 1);
		SetDlgItemText(IDC_SETLOCK, "实现加锁");
		SetDlgItemText(IDC_UNLOCK, "");
		GetDlgItem(IDC_EXITSYSTEM)->EnableWindow(true);
		user = 0;
		ClipCursor(NULL);
		SystemParametersInfo(SPI_SETSCREENSAVERRUNNING, false, 0,SPIF_UPDATEINIFILE);
		::ShowWindow(::FindWindow("Shell_TrayWnd", NULL), SW_SHOW);

	}
}
BOOL CServerLockerDlg::PreTranslateMessage(MSG * pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		//屏蔽指定按键  
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
void CServerLockerDlg::OnBnClickedExitsystem()
{
	OnOK();
}
int ShowContent(struct HKEY__*ReRootKey, TCHAR *ReSubKey, TCHAR *ReValueName)
{
	HKEY hKey = HKEY_LOCAL_MACHINE;
	int i = 0; //操作结果：0==succeed
	if (RegOpenKeyEx(hKey, ReSubKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		if (RegQueryValueEx(hKey, ReValueName, NULL, &dwType, (unsigned char *)content, &dwLength) != ERROR_SUCCESS)
		{
			AfxMessageBox("错误：无法查询有关的注册表信息");
			i = 1;
		}
		RegCloseKey(hKey);
	}
	else
	{
		AfxMessageBox("错误：无法打开有关的hKEY");
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
void CServerLockerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	HookUnload();    // 退出窗口，要卸载HOOK
	CDialogEx::OnClose();
}

void CServerLockerDlg::HookLoad()
{
	m_hinstHookDll = LoadLibrary(_T("MonitorDll.dll"));
	CString loginfo;

	if (NULL == m_hinstHookDll)
	{
		loginfo.Format(_T("加载 MonitorDll.dll失败，错误代码 = [%d] "), GetLastError());
		AfxMessageBox(loginfo);
		return;
	}


	typedef BOOL(WINAPI* LoadMonitor)(HWND hwnd, DWORD dwProcessId);
	LoadMonitor loadMonitor = NULL;
	loadMonitor = (LoadMonitor)::GetProcAddress(m_hinstHookDll, "HookLoad");
	if (NULL == loadMonitor)
	{
		loginfo.Format(_T("获取函数 HookLoad 失败，错误代码 = [%d]"), GetLastError());
		AfxMessageBox(loginfo);
	}
	if (loadMonitor(m_hWnd, GetCurrentProcessId()))
	{
		loginfo.Format(_T("HOOK加载成功"));
		AfxMessageBox(loginfo);
	}
	else
	{
		loginfo.Format(_T("HOOK加载失败"));
		AfxMessageBox(loginfo);
	}
}
void CServerLockerDlg::HookUnload()
{
	CString logInfo;
	if (m_hinstHookDll == NULL)
	{
		m_hinstHookDll = LoadLibrary(_T("MonitorDll.dll"));
		if (NULL == m_hinstHookDll)
		{
			logInfo.Format(_T("加载 MonitorDll.dll失败，错误代码 = [%d]"), GetLastError());
			AfxMessageBox(logInfo);
			return;
		}
	}

	typedef VOID(WINAPI* UnloadHook)();
	UnloadHook unloadHook = NULL;
	unloadHook = (UnloadHook)::GetProcAddress(m_hinstHookDll, "HookUnload");
	if (NULL == unloadHook)
	{
		logInfo.Format(_T("获取函数 HookUnload 失败，错误代码 = [%d]"), GetLastError());
		AfxMessageBox(logInfo);
		return;
	}

	unloadHook();

}

string sha256(const string str)
{
	char buf[65];
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, str.c_str(), str.size());
	SHA256_Final(hash, &sha256);
	string NewString = "";
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
		sprintf(buf, "%02x", hash[i]);
		NewString = NewString + buf;
	}
	//CString ctemp;
	//ctemp = NewString.c_str();
	//MessageBox(NULL, ctemp, "Message", MB_OK);
	return NewString;
}
