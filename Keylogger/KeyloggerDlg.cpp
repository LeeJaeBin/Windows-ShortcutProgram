
// KeyloggerDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Keylogger.h"
#include "KeyloggerDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include <UserEnv.h>
#include "CSettingDlg.h"

#pragma comment(lib, "Userenv.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_MAIN_TRAY    (WM_USER + 1)

BOOL RegCheck[10] = { FALSE, };
CString RegPath[10];

DWORD nSize = MAX_COMPUTERNAME_LENGTH + 1;
wchar_t szBuffer[MAX_COMPUTERNAME_LENGTH + 1];

CString inipath = L"C:\\Program Files\\setting.txt";

HHOOK m_hook = NULL;
int count[10] = { 0, };

CString INIREADString(CString strAppName, CString strKeyName, CString strFilePath) {
	WCHAR szReturnString[1024] = { 0, };
	memset(szReturnString, NULL, 1024);
	GetPrivateProfileString(strAppName, strKeyName, L"", szReturnString, 1024, strFilePath);

	CString str = szReturnString;
	return str;
}

void INIWriteString(CString strAppName, CString strKeyName, CString strValue, CString strFilePath) {
	WritePrivateProfileString(strAppName, strKeyName, strValue, strFilePath);
}

void Execute(int n) {
	CString ExePath = RegPath[n];
	if (ExePath == L"") {
		return;
	}

	STARTUPINFO StartupInfo = { 0 };
	PROCESS_INFORMATION ProcessInfo;
	StartupInfo.cb = sizeof(STARTUPINFO);
	::CreateProcess(ExePath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInfo);
}

LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam) {
	static int c = 0;
	KBDLLHOOKSTRUCT *pKey = (KBDLLHOOKSTRUCT *)lParam;


	if (nCode < 0)  // do not process message
		return CallNextHookEx(m_hook, nCode, wParam, lParam);

	if (wParam == WM_KEYDOWN) {
		if (GetAsyncKeyState(0x31) || GetAsyncKeyState(VK_NUMPAD1)) {
			count[1]++;
		}
		if (GetAsyncKeyState(0x32) || GetAsyncKeyState(VK_NUMPAD2)) {
			count[2]++;
		}
		if (GetAsyncKeyState(0x33) || GetAsyncKeyState(VK_NUMPAD3)) {
			count[3]++;
		}
		if (GetAsyncKeyState(0x34) || GetAsyncKeyState(VK_NUMPAD4)) {
			count[4]++;
		}
		if (GetAsyncKeyState(0x35) || GetAsyncKeyState(VK_NUMPAD5)) {
			count[5]++;
		}
		if (GetAsyncKeyState(0x36) || GetAsyncKeyState(VK_NUMPAD6)) {
			count[6]++;
		}
		if (GetAsyncKeyState(0x37) || GetAsyncKeyState(VK_NUMPAD7)) {
			count[7]++;
		}
		if (GetAsyncKeyState(0x38) || GetAsyncKeyState(VK_NUMPAD8)) {
			count[8]++;
		}
		if (GetAsyncKeyState(0x39) || GetAsyncKeyState(VK_NUMPAD9)) {
			count[9]++;
		}
		for (int i = 1; i < 10; i++) {
			if (count[i] == 20) {
				Execute(i);
				count[i] = 0;
			}
		}

	}
	if (wParam == WM_KEYUP) {
		for (int i = 1; i < 10; i++) {
			count[i] = 0;
		}
	}
	return CallNextHookEx(m_hook, nCode, wParam, lParam);
}

void CKeyloggerDlg::CreateTray()
{
	NOTIFYICONDATA nid;
	ZeroMemory(&nid, sizeof(nid));

	nid.cbSize = sizeof(nid);
	nid.hWnd = m_hWnd;
	nid.uID = IDI_ICON2;
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	nid.uCallbackMessage = WM_MAIN_TRAY;
	nid.hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);
	_tcscpy_s(nid.szTip, _T("ShortCut"));

	Shell_NotifyIcon(NIM_ADD, &nid);
}


void CKeyloggerDlg::DestroyTray()
{
	NOTIFYICONDATA nid;
	ZeroMemory(&nid, sizeof(nid));

	nid.cbSize = sizeof(nid);
	nid.hWnd = m_hWnd;
	nid.uID = IDI_ICON2;

	Shell_NotifyIcon(NIM_DELETE, &nid);
}

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CKeyloggerDlg 대화 상자



CKeyloggerDlg::CKeyloggerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KEYLOGGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKeyloggerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NUMLIST, m_NumList);
	DDX_Control(pDX, IDC_PATH, m_EditPath);
	DDX_Control(pDX, IDC_REGLIST, m_RegList);
	DDX_Control(pDX, IDC_SETTING, m_btnSetting);
}

BEGIN_MESSAGE_MAP(CKeyloggerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_DELETE, &CKeyloggerDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_ADD, &CKeyloggerDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_SETTING, &CKeyloggerDlg::OnBnClickedSetting)
	ON_MESSAGE(WM_MAIN_TRAY, &CKeyloggerDlg::OnMainTray)
	ON_BN_CLICKED(IDC_HIDE, &CKeyloggerDlg::OnBnClickedHide)
END_MESSAGE_MAP()


// CKeyloggerDlg 메시지 처리기

BOOL CKeyloggerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	CButton *button1 = (CButton*)GetDlgItem(IDOK);
	CButton *button2 = (CButton*)GetDlgItem(IDCANCEL);
	button1->DestroyWindow();
	button2->DestroyWindow();
	
	m_btnSetting.LoadBitmaps(IDB_BITMAP2, NULL, NULL, NULL);
	m_btnSetting.SizeToContent();

	::GetUserName(szBuffer, &nSize);
	CString pathtemp = szBuffer;
	inipath = L"C:\\Users\\" + pathtemp + L"\\Documents\\setting.txt";

	m_RegList.InsertColumn(0, _T("#"), LVCFMT_CENTER, 25, -1);
	m_RegList.InsertColumn(1, _T("Path"), LVCFMT_CENTER, 400, -1);

	for (int i = 1; i <= 9; i++) {
		int listitem = m_RegList.GetItemCount();
		CString str;
		str.Format(L"%d", i);

		m_NumList.AddString(str);
		CString SetPath = INIREADString(str, L"Path", inipath);
		CString MainExe;
		CString MainExe2;

		if (SetPath != L"") {
			RegCheck[i] = TRUE;
			RegPath[i] = SetPath;

			int cnt = 0;

			while (AfxExtractSubString(MainExe, SetPath, cnt, L'\\')){
				if (MainExe != L"") {
					MainExe2 = MainExe;
				}
				cnt++;
			}

			m_RegList.InsertItem(listitem, str);
			m_RegList.SetItem(listitem, 1, LVIF_TEXT, MainExe2, 0, 0, 0, NULL);
		}
	}
	m_NumList.SetCurSel(0);

	CreateTray();

	m_hook = SetWindowsHookEx(WH_KEYBOARD_LL, GetMsgProc, theApp.m_hInstance, NULL);

	if (m_hook == NULL) {
		AfxMessageBox(L"Hooking Err");
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CKeyloggerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CKeyloggerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CKeyloggerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CKeyloggerDlg::OnBnClickedDelete()
{
	int idx = ListView_GetNextItem(m_RegList, -1, LVNI_SELECTED);

	if (idx == -1) {
		AfxMessageBox(L"Select the number first");
		return;
	}
	CString cidx = m_RegList.GetItemText(idx, 0);
	INIWriteString(cidx, L"Path", L"", inipath);
	RegPath[idx] = L"";

	while (idx != -1) {
		ListView_DeleteItem(m_RegList, idx);
		idx = ListView_GetNextItem(m_RegList, idx - 1, LVNI_ALL | LVNI_SELECTED);
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CKeyloggerDlg::OnBnClickedAdd()
{
	int listitem = m_RegList.GetItemCount();
	int nSelected = m_NumList.GetCurSel() + 1;
	CString str;
	str.Format(L"%d", nSelected);

	CString path;
	m_EditPath.GetWindowText(path);

	if (path == L"") {
		AfxMessageBox(L"Write a path");
		return;
	}

	CString dbcheck = INIREADString(str, L"Path", inipath);
	if (dbcheck != L"") {
		AfxMessageBox(L"Already Exsist!");
		return;
	}

	INIWriteString(str, L"Path", path, inipath);

	CString MainExe, MainExe2;
	int cnt = 0;
	while (AfxExtractSubString(MainExe, path, cnt, L'\\')) {
		if (MainExe != L"") {
			MainExe2 = MainExe;
		}
		cnt++;
	}

	m_RegList.InsertItem(listitem, str);
	m_RegList.SetItem(listitem, 1, LVIF_TEXT, MainExe2, 0, 0, 0, NULL);

	RegCheck[nSelected] = TRUE;
	RegPath[nSelected] = path;

	UpdateData(TRUE);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CKeyloggerDlg::OnBnClickedSetting()
{
	CSettingDlg asdf = new CSettingDlg;
	asdf.DoModal();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

afx_msg LRESULT CKeyloggerDlg::OnMainTray(WPARAM wParam, LPARAM lParam)
{
	switch (lParam) {
	case WM_LBUTTONDBLCLK:
		ShowWindow(SW_SHOW);
		SetForegroundWindow();
		break;
	/*case WM_RBUTTONUP:
		CMenu menu, *pMenu = NULL;
		CPoint pt;

		menu.LoadMenu(IDR_MENU_TRAY_POPUP);
		pMenu = menu.GetSubMenu(0);

		GetCursorPos(&pt);

		SetForegroundWindow();

		pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);
		::PostMessage(m_hWnd, WM_NULL, NULL, NULL);

		ReleaseCapture();

		break;*/
	}

	return 0;
}

void CKeyloggerDlg::OnBnClickedHide()
{
	PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);
}
