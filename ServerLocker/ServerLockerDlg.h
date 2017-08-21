// ServerLockerDlg.h : ͷ�ļ�
//
#include <iostream>
#include <string>
#include <windows.h>
#include "thirdparty/openssl/include/sha.h"

#pragma comment(lib,"thirdparty/openssl/lib/libcrypto64MT.lib")

using namespace std;
#pragma once


// CServerLockerDlg �Ի���
class CServerLockerDlg : public CDialogEx
{
// ����
public:
	CServerLockerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SERVERLOCKTOOLSPS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	HINSTANCE m_hinstHookDll;    //    MonitorDll��ʵ�����
	void HookLoad();            //    ����HOOK      
	BOOL installhook();
	void HookUnload();            //    ж��HOOK
	int ShowContent(struct HKEY__*ReRootKey, TCHAR *ReSubKey, TCHAR *ReValueName);// ע����ѯ
	string sha512(const string str);// SHA512ת��
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();        //    �رճ����ʱ��ж��DLL !!!!!
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSetlock();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedExitsystem();
};
