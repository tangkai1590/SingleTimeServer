#include "stdafx.h"
#include "GpsClient.h"
#include "Nema0183Protocl.h"
#include "time.h"
#include "NtpServerDlg.h"

CGpsClient::CGpsClient():m_Client(this)
{
	m_hShow = INVALID_HANDLE_VALUE;
}


CGpsClient::~CGpsClient()
{
}

BOOL CGpsClient::ConnectServer()
{
	USHORT usPort = (USHORT)_ttoi(m_strPort);

	::LogClientStarting(m_strAddress, usPort);

	if (m_Client.Start(m_strAddress, usPort, m_bAsyncConn, ::GetAnyAddress(m_strAddress)))
	{
		return TRUE;
	}
	else
	{
		::LogClientStartFail(m_Client.GetLastError(), m_Client.GetLastErrorDesc());
		return FALSE;
	}
}

BOOL CGpsClient::IsConnect()
{
	BOOL bConnect = m_Client.Send((const BYTE*)("test"), 5);
	if (!bConnect)
	{
		m_Client.Stop();
	}
	return bConnect;
}
	
EnHandleResult CGpsClient::OnSend(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
	::PostOnSend(dwConnID, pData, iLength);
	return HR_OK;
}

EnHandleResult CGpsClient::OnReceive(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
	// 接收数据
	Nema0183Protocl pro;
	char szLeftData[2048] = { 0 };
	int nLeftLen = 0;
	pro.ParseData((void*)pData, iLength, nullptr, nLeftLen);
	
	SYSTEMTIME st;
	for (int i = 0; i < pro.MessageNumber(); i++)
	{
		Nema0183Protocl::NemaMessage msg;
		msg = pro.GetMessage(i);
		if (msg[0][3] == 'G' && msg[0][4] == 'G' && msg[0][5] == 'A')
		{
			// 修改时间
			::GetSystemTime(&st);
			st.wHour = (msg[1][0] - 48) * 10 + (msg[1][1] - 48);
			st.wMinute = (msg[1][2] - 48) * 10 + (msg[1][3] - 48);
			st.wSecond = (msg[1][4] - 48) * 10 + (msg[1][5] - 48);
			st.wMilliseconds = 0;
			if (msg[1].size() >= 8 && msg[1][7] != ',')
			{
				st.wMilliseconds += (msg[1][7] - 48) * 100;
			}
			if (msg[1].size() >= 9 && msg[1][8] != ',')
			{
				st.wMilliseconds += (msg[1][8] - 48) * 10;
			}
			if (msg[1].size() >= 10 && msg[1][9] != ',')
			{
				st.wMilliseconds += (msg[1][9] - 48);
			}
			
			//::SetSystemTime(&st);
			LPSYSTEMTIME lpst = new SYSTEMTIME;
			ZeroMemory(lpst, sizeof(SYSTEMTIME));
			memcpy(lpst, &st, sizeof(SYSTEMTIME));
			if (m_hShow != INVALID_HANDLE_VALUE)
			{
				::PostMessageA((HWND)m_hShow, UM_MSG_RECVTIME, (WPARAM)lpst, NULL);
			}

			break;
		}
	}


	::PostOnReceive(dwConnID, pData, iLength);
	return HR_OK;
}

EnHandleResult CGpsClient::OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
{
	iErrorCode == SE_OK ? ::PostOnClose(dwConnID) :
	::PostOnError(dwConnID, enOperation, iErrorCode);
	return HR_OK;
} 

EnHandleResult CGpsClient::OnConnect(ITcpClient* pSender, CONNID dwConnID)
{
	TCHAR szAddress[50];
	int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
	USHORT usPort;

	pSender->GetLocalAddress(szAddress, iAddressLen, usPort);

	::PostOnConnect(dwConnID, szAddress, usPort);
	return HR_OK;
}
