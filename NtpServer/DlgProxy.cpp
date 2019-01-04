
// DlgProxy.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NtpServer.h"
#include "DlgProxy.h"
#include "NtpServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNtpServerDlgAutoProxy

IMPLEMENT_DYNCREATE(CNtpServerDlgAutoProxy, CCmdTarget)

CNtpServerDlgAutoProxy::CNtpServerDlgAutoProxy()
{
	EnableAutomation();
	
	// ΪʹӦ�ó������Զ��������ڻ״̬ʱһֱ���� 
	//	���У����캯������ AfxOleLockApp��
	AfxOleLockApp();

	// ͨ��Ӧ�ó����������ָ��
	//  �����ʶԻ���  ���ô�����ڲ�ָ��
	//  ָ��Ի��򣬲����öԻ���ĺ���ָ��ָ��
	//  �ô���
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CNtpServerDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CNtpServerDlg)))
		{
			m_pDialog = reinterpret_cast<CNtpServerDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CNtpServerDlgAutoProxy::~CNtpServerDlgAutoProxy()
{
	// Ϊ������ OLE �Զ����������ж������ֹӦ�ó���
	//	������������ AfxOleUnlockApp��
	//  ���������������⣬�⻹���������Ի���
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CNtpServerDlgAutoProxy::OnFinalRelease()
{
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease��  ���ཫ�Զ�
	// ɾ���ö���  �ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CNtpServerDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CNtpServerDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// ע��: ��������˶� IID_INtpServer ��֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡�  �� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {7D3339F8-DC8C-46D5-B834-D4428498636D}
static const IID IID_INtpServer =
{ 0x7D3339F8, 0xDC8C, 0x46D5, { 0xB8, 0x34, 0xD4, 0x42, 0x84, 0x98, 0x63, 0x6D } };

BEGIN_INTERFACE_MAP(CNtpServerDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CNtpServerDlgAutoProxy, IID_INtpServer, Dispatch)
END_INTERFACE_MAP()

// IMPLEMENT_OLECREATE2 ���ڴ���Ŀ�� StdAfx.h �ж���
// {4203E43E-C4A3-4D52-9D30-81E566B1CDD0}
IMPLEMENT_OLECREATE2(CNtpServerDlgAutoProxy, "NtpServer.Application", 0x4203e43e, 0xc4a3, 0x4d52, 0x9d, 0x30, 0x81, 0xe5, 0x66, 0xb1, 0xcd, 0xd0)


// CNtpServerDlgAutoProxy ��Ϣ�������
