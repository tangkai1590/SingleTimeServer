
// DlgProxy.cpp : 实现文件
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
	
	// 为使应用程序在自动化对象处于活动状态时一直保持 
	//	运行，构造函数调用 AfxOleLockApp。
	AfxOleLockApp();

	// 通过应用程序的主窗口指针
	//  来访问对话框。  设置代理的内部指针
	//  指向对话框，并设置对话框的后向指针指向
	//  该代理。
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
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	//	析构函数调用 AfxOleUnlockApp。
	//  除了做其他事情外，这还将销毁主对话框
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CNtpServerDlgAutoProxy::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。  基类将自动
	// 删除该对象。  在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CNtpServerDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CNtpServerDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// 注意: 我们添加了对 IID_INtpServer 的支持
//  以支持来自 VBA 的类型安全绑定。  此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {7D3339F8-DC8C-46D5-B834-D4428498636D}
static const IID IID_INtpServer =
{ 0x7D3339F8, 0xDC8C, 0x46D5, { 0xB8, 0x34, 0xD4, 0x42, 0x84, 0x98, 0x63, 0x6D } };

BEGIN_INTERFACE_MAP(CNtpServerDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CNtpServerDlgAutoProxy, IID_INtpServer, Dispatch)
END_INTERFACE_MAP()

// IMPLEMENT_OLECREATE2 宏在此项目的 StdAfx.h 中定义
// {4203E43E-C4A3-4D52-9D30-81E566B1CDD0}
IMPLEMENT_OLECREATE2(CNtpServerDlgAutoProxy, "NtpServer.Application", 0x4203e43e, 0xc4a3, 0x4d52, 0x9d, 0x30, 0x81, 0xe5, 0x66, 0xb1, 0xcd, 0xd0)


// CNtpServerDlgAutoProxy 消息处理程序
