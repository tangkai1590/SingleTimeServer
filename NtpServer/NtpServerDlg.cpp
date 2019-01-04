
// NtpServerDlg.cpp : ʵ���ļ�
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


// CNtpServerDlg �Ի���
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
	// ����öԻ������Զ���������
	//  ���˴���ָ��öԻ���ĺ���ָ������Ϊ NULL���Ա�
	//  �˴���֪���öԻ����ѱ�ɾ����
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
			strShow.Format(_T(": %s ������ʱ!"), szAddress);
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
			strShow.Format(_T(": %s ������ʱ!"), szAddress);
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


// CNtpServerDlg ��Ϣ�������

BOOL CNtpServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CNtpServerDlg::OnPaint()
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
HCURSOR CNtpServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ���û��ر� UI ʱ������������Ա���������ĳ��
//  �������Զ�����������Ӧ�˳���  ��Щ
//  ��Ϣ�������ȷ����������: �����������ʹ�ã�
//  ������ UI�������ڹرնԻ���ʱ��
//  �Ի�����Ȼ�ᱣ�������

void CNtpServerDlg::OnClose()
{
	if (IDYES == this->MessageBox(_T("�Ƿ��˳�ʱ���������"), _T("��ʾ"), MB_YESNO))
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
	// �����������Ա�����������Զ���
	//  �������Իᱣ�ִ�Ӧ�ó���
	//  ʹ�Ի���������������� UI ����������
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}
	return TRUE;
}

void CNtpServerDlg::OnBnClickedStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_Address.GetWindowText(m_strAddress);
	m_strAddress.Trim();

	SetAppState(ST_STARTING);

	if (m_Server.Start(m_strAddress, PORT))
	{
		::LogServerStart(m_strAddress, PORT);
		SetAppState(ST_STARTED);
		this->m_Info.AddString(_T("������ʱ����ɹ���"));
	}
	else
	{
		::LogServerStartFail(m_Server.GetLastError(), m_Server.GetLastErrorDesc());
		SetAppState(ST_STOPPED);
		this->m_Info.AddString(_T("������ʱ����ʧ�ܣ�"));
	}
}

void CNtpServerDlg::OnBnClickedStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetAppState(ST_STOPPING);
	if (m_Server.Stop())
	{
		::LogServerStop();
		SetAppState(ST_STOPPED);
		this->m_Info.AddString(_T("ֹͣ��ʱ����"));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_bTimeDataConnet)
	{
		//this->_ConnectGPSServer();
		m_bTimeDataConnet = TRUE;
		this->GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("ֹͣ"));
	}
	else
	{
		m_bTimeDataConnet = FALSE;
		this->GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("����"));
	}
}


void CNtpServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == TIMER_ID_CHECKSTATUS)
	{
		if (m_bTimeDataConnet && !this->_CheckSocketStatus())
		{
			// δ����
			this->m_Info.AddString(m_strCurTime2 + _T(": δ����GPS����Դ"));
			if (this->_ConnectGPSServer())
			{
				// ���ӳɹ�
				this->m_Info.AddString(m_strCurTime2 + _T(": ����GPS����Դ�ɹ�"));
			}
			else
			{
				// ����ʧ��
				this->m_Info.AddString(m_strCurTime2 + _T(": ����GPS����Դʧ��"));
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
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)    //���λس���ESC  
			return TRUE;
		if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)  //����ALT+F4
			return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}
