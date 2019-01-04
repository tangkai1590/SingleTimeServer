#pragma once

#include "../HPSocketSrc/TcpClient.h"
#include "../Global/helper.h"

class CGpsClient : public CTcpClientListener
{
public:
	CGpsClient();
	~CGpsClient();

	BOOL m_bAsyncConn;
	EnAppState m_enState;
	CTcpClient m_Client;
	CString m_strAddress;
	CString m_strPort;
	HANDLE m_hShow;

public:
	BOOL ConnectServer();

	BOOL IsConnect();
private:
	virtual EnHandleResult OnSend(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
	virtual EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
	virtual EnHandleResult OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode);
	virtual EnHandleResult OnConnect(ITcpClient* pSender, CONNID dwConnID);
};

