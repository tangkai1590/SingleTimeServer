
// NtpServerDlg.h : 头文件
//

#pragma once
#include "../HPSocketSrc/UdpServer.h"
#include "../Global/helper.h"
#include "afxwin.h"
#include "GpsClient.h"
#include "afxcmn.h"

class CNtpServerDlgAutoProxy;
#define TIMER_ID_CHECKSTATUS   1001
#define TIMER_ID_CHECKTIME     1002

#define UM_MSG_RECVTIME WM_USER+1 
// CNtpServerDlg 对话框
class CNtpServerDlg : public CDialogEx, public CUdpServerListener
{
	DECLARE_DYNAMIC(CNtpServerDlg);
	friend class CNtpServerDlgAutoProxy;

// 构造
public:
	CNtpServerDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CNtpServerDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NTPSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
// 实现
protected:
	CNtpServerDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	TPkgInfo* FindPkgInfo(HP_CONNID dwConnID);
	void RemovePkgInfo(HP_CONNID dwConnID);

	BOOL CanExit();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

public:
	void SetAppState(EnAppState state);

	virtual EnHandleResult OnPrepareListen(IUdpServer* pSender, SOCKET soListen);
	virtual EnHandleResult OnAccept(IUdpServer* pSender, CONNID dwConnID, UINT_PTR pSockAddr);
	virtual EnHandleResult OnSend(IUdpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
	virtual EnHandleResult OnReceive(IUdpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
	virtual EnHandleResult OnClose(IUdpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode);
	virtual EnHandleResult OnShutdown(IUdpServer* pSender);

public:
	static const USHORT PORT;
	static const LPCTSTR ADDRESS;

	EnAppState m_enState;
	CString m_strAddress;

	static CNtpServerDlg* m_spThis;

	CUdpServer m_Server;
	//HP_TcpServerListener m_pListener;
	CGpsClient m_GpsClient;

public:
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg LRESULT OnRecvTime(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	BOOL _ConnectGPSServer();
	BOOL _CheckSocketStatus();
	BOOL _GetCurrentTime();

public:
	CListBox m_Info;
	CButton m_Start;
	CButton m_Stop;
	CEdit m_Address;
	CString m_strIpServer;
	CString m_strPortServer;
	CButton m_btnConnect;
	CString m_strCurTime;
	CString m_strCurTime2;
	CString m_strLast;
	int m_nSetTimeCount;
	BOOL m_bAutoConnect;
	BOOL m_bTimeDataConnet;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
