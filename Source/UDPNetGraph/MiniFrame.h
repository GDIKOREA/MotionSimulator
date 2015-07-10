#pragma once


// CMiniFrame frame

class CMiniFrame : public CMiniFrameWnd
{
public:
	CMiniFrame();           // protected constructor used by dynamic creation
	virtual ~CMiniFrame();

protected:
	DECLARE_MESSAGE_MAP()
};


