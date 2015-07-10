#pragma once

#include "UpdateObserver.h"
#include "UDPObserver.h"


// CDockablePaneChildView dialog

class CDockablePaneChildView : public CDialogEx
							 , public iUpdateObserver
							 , public iUDPObserver
{
public:
	CDockablePaneChildView(UINT id, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDockablePaneChildView();

	virtual void UpdateSerial(char *buffer) {}
	virtual void Update(const float deltaSeconds) {}
	virtual void UpdateUDP(const char *buffer, const int bufferLen) {}


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
