#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "udpstream.h"
#include "udpstreamplayer.h"

// CUDPPlayerView dialog

class CUDPPlayerView : public CDockablePaneChildView
{
public:
	CUDPPlayerView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUDPPlayerView();

// Dialog Data
	enum { IDD = IDD_DIALOG_UDP_PLAYER };

	virtual void Update(const float deltaSeconds) override;
	virtual void UpdateUDP(const char *buffer, const int bufferLen) override;


protected:
	enum STATE {
		STOP,
		PLAY,
		PAUSE,
		RECORD,
	};
	STATE m_state;

	cUDPStream m_udpStream;
	cUDPStreamPlayer m_udpPlayer;
	network::cUDPClient m_udpClient;
	
	char m_buffer[128];


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	STATE ChangeState(const STATE nextState);
	void UpdateUDPFileInfo(const string &fileName, const cUDPStream &udpStream);
	void Play(const string &fileName);
	void RecordEnd();

	DECLARE_ANCHOR_MAP();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonFileRefresh();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonRecord();
	afx_msg void OnSelchangedTreeFile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderPlay(NMHDR *pNMHDR, LRESULT *pResult);
	CFileTreeCtrl m_FileTree;
	CTreeCtrl m_FileInfoTree;
	CButton m_PlayButton;
	CButton m_StopButton;
	CButton m_RecordButton;
	CSliderCtrl m_PlaySlider;
	CIPAddressCtrl m_UDPIP;
	CEdit m_UDPPort;
	CButton m_RefreshButton;
	CStatic m_PlayPos;
	afx_msg void OnBnClickedCheckRepeat();
	BOOL m_IsRepeat;
};
