
// DlgProxy.h: ͷ�ļ�
//

#pragma once

class CNtpServerDlg;


// CNtpServerDlgAutoProxy ����Ŀ��

class CNtpServerDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CNtpServerDlgAutoProxy)

	CNtpServerDlgAutoProxy();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��

// ����
public:
	CNtpServerDlg* m_pDialog;

// ����
public:

// ��д
	public:
	virtual void OnFinalRelease();

// ʵ��
protected:
	virtual ~CNtpServerDlgAutoProxy();

	// ���ɵ���Ϣӳ�亯��

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CNtpServerDlgAutoProxy)

	// ���ɵ� OLE ����ӳ�亯��

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

