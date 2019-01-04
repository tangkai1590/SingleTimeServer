
// NtpServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NtpServer.h"
#include "NtpServerDlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"
#include "NtpProtocl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNtpServerDlg 对话框
#include "NtpServerDlg.h"
#include "time.h"

const LPCTSTR CNtpServerDlg::ADDRESS = _T("192.168.1.100");
const USHORT CNtpServerDlg::PORT = 123;
CNtpServerDlg* CNtpServerDlg::m_spThis = nullptr;

IMPLEMENT_DYNAMIC(CNtpServerDlg, CDialogEx);

CNtpServerDlg::CNtpServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NTPSERVER_DIALOG, pParent)
	, m_Server(this)
	, m_strIpServer(_T(""))
	, m_strPortServer(_T(""))
	, m_strCurTime(_T(""))
	, m_strCurTime2(_T(""))
	, m_strLast(_T(""))
	, m_bAutoConnect(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
	m_nSetTimeCount = 0;
	m_spThis = this;
}

CNtpServerDlg::~CNtpServerDlg()
{
	// 如果该对话框有自动化代理，则
	//  将此代理指向该对话框的后向指针设置为 NULL，以便
	//  此代理知道该对话框已被删除。
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CNtpServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INFO, m_Info);
	DDX_Control(pDX, IDC_START, m_Start);
	DDX_Control(pDX, IDC_STOP, m_Stop);
	DDX_Control(pDX, IDC_ADDRESS, m_Address);
	DDX_Text(pDX, IDC_ADDRESS2, m_strIpServer);
	DDX_Text(pDX, IDC_EDIT_PORT, m_strPortServer);
	DDX_Control(pDX, IDC_BTN_CONNECT, m_btnConnect);
	DDX_Text(pDX, IDC_EDIT_CURTIME, m_strCurTime);
	DDX_Text(pDX, IDC_EDIT_CURTIME2, m_strCurTime2);
	DDX_Text(pDX, IDC_EDIT_LASTTIME, m_strLast);
	DDX_Check(pDX, IDC_CHECK_AUTOREC, m_bAutoConnect);
}

void CNtpServerDlg::SetAppState(EnAppState state)
{
	m_enState = state;

	if (this->GetSafeHwnd() == nullptr)
		return;

	m_Start.EnableWindow(m_enState == ST_STOPPED);
	m_Stop.EnableWindow(m_enState == ST_STARTED);
}

EnHandleResult CNtpServerDlg::OnPrepareListen(IUdpServer* pSender, SOCKET soListen)
{
	TCHAR szAddress[50];
	int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
	USHORT usPort;

	pSender->GetListenAddress(szAddress, iAddressLen, usPort);
	::PostOnPrepareListen(szAddress, usPort);

	VERIFY(::SSO_RecvBuffSize(soListen, 1 * 1024 * 1024) == NO_ERROR);

	return HR_OK;
}

EnHandleResult CNtpServerDlg::OnAccept(IUdpServer* pSender, CONNID dwConnID, UINT_PTR pSockAddr)
{
	BOOL bPass = TRUE;
	TCHAR szAddress[50];
	int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
	USHORT usPort;

	pSender->GetRemoteAddress(dwConnID, szAddress, iAddressLen, usPort);

// 	if (!m_strRejectAddress.IsEmpty())
// 	{
// 		if (m_strRejectAddress.CompareNoCase(szAddress) == 0)
// 			bPass = FALSE;
// 	}

	::PostOnAccept(dwConnID, szAddress, usPort, bPass);

	return bPass ? HR_OK : HR_ERROR;
}

EnHandleResult CNtpServerDlg::OnSend(IUdpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{

	::PostOnSend(dwConnID, pData, iLength);
	return HR_OK;
}

EnHandleResult CNtpServerDlg::OnReceive(IUdpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
	::PostOnReceive(dwConnID, pData, iLength);
	NtpProtocl ntpPro;
	int nType = ntpPro.CheckPacket((NTPPacket*)pData, iLength);
	if (0 == nType)
	{
		return HR_ERROR;
	}

	NTPPacket packet;
	ZeroMemory(&packet, sizeof(packet));
	memcpy_s(&packet, sizeof(packet), pData, iLength);
	
	if (nType == 3)
	{
		ntpPro.GetNTPReplyPacket(&packet);
		if (pSender->Send(dwConnID, (const BYTE*)&packet, iLength))
		{
			///////////
			TCHAR szAddress[50];
			int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
			USHORT usPort;
			pSender->GetRemoteAddress(dwConnID, szAddress, iAddressLen, usPort);
			CString strShow = _T("");
			strShow.Format(_T(": %s 请求授时!"), szAddress);
			m_Info.AddString(m_strCurTime2 + strShow);
			///////////
			return HR_OK;
		}
		else
		{
			///////////
			TCHAR szAddress[50];
			int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
			USHORT usPort;
			pSender->GetRemoteAddress(dwConnID, szAddress, iAddressLen, usPort);
			CString strShow = _T("");
			strShow.Format(_T(": %s 请求授时!"), szAddress);
			m_Info.AddString(m_strCurTime2 + strShow);
			///////////
			return HR_ERROR;
		}
	}
	else if (nType == 4)
	{
		return HR_OK;
	}

	return HR_OK;
}

EnHandleResult CNtpServerDlg::OnClose(IUdpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
{
	iErrorCode == SE_OK ? ::PostOnClose(dwConnID) :
	::PostOnError(dwConnID, enOperation, iErrorCode);

	return HR_OK;
}

EnHandleResult CNtpServerDlg::OnShutdown(IUdpServer* pSender)
{
	::PostOnShutdown();
	return HR_OK;
}

BEGIN_MESSAGE_MAP(CNtpServerDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CNtpServerDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &CNtpServerDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CNtpServerDlg::OnBnClickedBtnConnect)
	ON_MESSAGE(UM_MSG_RECVTIME, &CNtpServerDlg::OnRecvTime)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CNtpServerDlg 消息处理程序

BOOL CNtpServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CString strTitle;
	CString strOriginTitle;
	GetWindowText(strOriginTitle);
	strTitle.Format(_T("%s"), strOriginTitle);
	SetWindowText(strTitle);

	::SetMainWnd(this);
	//::SetInfoList(&m_Info);
	SetAppState(ST_STOPPED);

	SetTimer(TIMER_ID_CHECKSTATUS, 5000, NULL);
	SetTimer(TIMER_ID_CHECKTIME, 50, NULL);

	m_GpsClient.m_hShow = this->GetSafeHwnd();
	m_bTimeDataConnet = FALSE;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNtpServerDlg::OnPaint()
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
HCURSOR CNtpServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 当用户关闭 UI 时，如果控制器仍保持着它的某个
//  对象，则自动化服务器不应退出。  这些
//  消息处理程序确保如下情形: 如果代理仍在使用，
//  则将隐藏 UI；但是在关闭对话框时，
//  对话框仍然会保留在那里。

void CNtpServerDlg::OnClose()
{
	if (IDYES == this->MessageBox(_T("是否退出时间服务器？"), _T("提示"), MB_YESNO))
	{
		if (CanExit())
			CDialogEx::OnClose();
	}
}

void CNtpServerDlg::OnOK()
{
	if (CanExit())
		CDialogEx::OnOK();
}

void CNtpServerDlg::OnCancel()
{
	if (CanExit())
		CDialogEx::OnCancel();
}

BOOL CNtpServerDlg::CanExit()
{
	// 如果代理对象仍保留在那里，则自动化
	//  控制器仍会保持此应用程序。
	//  使对话框保留在那里，但将其 UI 隐藏起来。
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}
	return TRUE;
}

void CNtpServerDlg::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Address.GetWindowText(m_strAddress);
	m_strAddress.Trim();

	SetAppState(ST_STARTING);

	if (m_Server.Start(m_strAddress, PORT))
	{
		::LogServerStart(m_strAddress, PORT);
		SetAppState(ST_STARTED);
		this->m_Info.AddString(_T("启动授时服务成功！"));
	}
	else
	{
		::LogServerStartFail(m_Server.GetLastError(), m_Server.GetLastErrorDesc());
		SetAppState(ST_STOPPED);
		this->m_Info.AddString(_T("启动授时服务失败！"));
	}
}

void CNtpServerDlg::OnBnClickedStop()
{
	// TODO: 在此添加控件通知处理程序代码
	SetAppState(ST_STOPPING);
	if (m_Server.Stop())
	{
		::LogServerStop();
		SetAppState(ST_STOPPED);
		this->m_Info.AddString(_T("停止授时服务！"));
	}
	else
	{
		ASSERT(FALSE);
	}
}

LRESULT CNtpServerDlg::OnRecvTime(WPARAM wParam, LPARAM lParam)
{
	if (wParam != NULL)
	{
		LPSYSTEMTIME lpst = (LPSYSTEMTIME)wParam;
		if (m_nSetTimeCount > 200)
		{
			::SetSystemTime(lpst);
			m_nSetTimeCount = 0;

			m_strLast = _T("");
			m_strLast.Format(_T("%02d:%02d:%02d.%03d"), lpst->wHour, lpst->wMinute, lpst->wSecond, lpst->wMilliseconds);

			((CEdit*)this->GetDlgItem(IDC_EDIT_LASTTIME))->SetWindowTextW(m_strLast);
		}
		
		delete lpst;
		lpst = NULL;
	}
	return SE_OK;
}

BOOL CNtpServerDlg::_ConnectGPSServer()
{
	m_GpsClient.m_strAddress = m_strIpServer;// "127.0.0.1";
	m_GpsClient.m_strPort = m_strPortServer;//"10086";
	BOOL  bConnectSucces = m_GpsClient.ConnectServer();

	return bConnectSucces;
}

BOOL CNtpServerDlg::_CheckSocketStatus()
{
	return  m_GpsClient.IsConnect();
}


BOOL CNtpServerDlg::_GetCurrentTime()
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	m_strCurTime = _T("");
	m_strCurTime.Format(_T("%02d:%02d:%02d.%03d"), st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	((CEdit*)this->GetDlgItem(IDC_EDIT_CURTIME))->SetWindowTextW(m_strCurTime);

	GetLocalTime(&st);
	m_strCurTime2 = _T("");
	m_strCurTime2.Format(_T("%02d:%02d:%02d.%03d"), st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	((CEdit*)this->GetDlgItem(IDC_EDIT_CURTIME2))->SetWindowTextW(m_strCurTime2);
	//this->GetDlgItem(IDC_EDIT_CURTIME2)->UpdateData(FALSE);

	//->UpdateData(FALSE);
	return TRUE;
}

void CNtpServerDlg::OnBnClickedBtnConnect()
{
	this->UpdateData(TRUE);
	// TODO: 在此添加控件通知处理程序代码
	if (!m_bTimeDataConnet)
	{
		//this->_ConnectGPSServer();
		m_bTimeDataConnet = TRUE;
		this->GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("停止"));
	}
	else
	{
		m_bTimeDataConnet = FALSE;
		this->GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("连接"));
	}
}


void CNtpServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == TIMER_ID_CHECKSTATUS)
	{
		if (m_bTimeDataConnet && !this->_CheckSocketStatus())
		{
			// 未连接
			this->m_Info.AddString(m_strCurTime2 + _T(": 未连接GPS数据源"));
			if (this->_ConnectGPSServer())
			{
				// 连接成功
				this->m_Info.AddString(m_strCurTime2 + _T(": 连接GPS数据源成功"));
			}
			else
			{
				// 连接失败
				this->m_Info.AddString(m_strCurTime2 + _T(": 连接GPS数据源失败"));
			}
		}

		int nCount = m_Info.GetCount();
		if (nCount >= 105)
		{
			for (int i = 0; i < nCount - 100; i++)
			{
				m_Info.DeleteString(m_Info.GetCount() - 1);
			}
		}
	}

	else if (nIDEvent == TIMER_ID_CHECKTIME)
	{
		this->_GetCurrentTime();
		m_nSetTimeCount++;
	}

	__super::OnTimer(nIDEvent);
}


BOOL CNtpServerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)    //屏蔽回车和ESC  
			return TRUE;
		if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)  //屏蔽ALT+F4
			return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}
