
// MotionMonitorDoc.cpp : implementation of the CMotionMonitorDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MotionMonitor.h"
#endif

#include "MotionMonitorDoc.h"
#include "MainFrm.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMotionMonitorDoc

IMPLEMENT_DYNCREATE(CMotionMonitorDoc, CDocument)

BEGIN_MESSAGE_MAP(CMotionMonitorDoc, CDocument)
END_MESSAGE_MAP()


// CMotionMonitorDoc construction/destruction

CMotionMonitorDoc::CMotionMonitorDoc()
{
	// TODO: add one-time construction code here

}

CMotionMonitorDoc::~CMotionMonitorDoc()
{
}

// CMotionMonitorDoc serialization

void CMotionMonitorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CMotionMonitorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CMotionMonitorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMotionMonitorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMotionMonitorDoc diagnostics

#ifdef _DEBUG
void CMotionMonitorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMotionMonitorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMotionMonitorDoc commands
BOOL CMotionMonitorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		static bool isFirst = true;
		string fileName;
		if (isFirst)
		{
#ifdef MG_STAND_RELEASE
			fileName = "../media/machinegun/motionmonitor_mg_stand.json";
#else
			fileName = "../media/machinegun/motionmonitor_mg.json";
#endif
			isFirst = false;
		}
		else
		{
			fileName = "motionmonitor_new.json";
		}

		cMotionMonitorConfig &config = cMotionController::Get()->m_config;
		config.ReadConfigFile(fileName);
		pFrm->UpdateConfig();
		pFrm->SetWindowTextW(str2wstr(fileName).c_str());
	}

	return TRUE;
}


BOOL CMotionMonitorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		cMotionMonitorConfig &config = cMotionController::Get()->m_config;

		pFrm->UpdateConfig(false);
		config.WriteConfigFile(config.m_fileName);

		if (config.ReadConfigFile(wstr2str(lpszPathName)))
			pFrm->UpdateConfig();
	}

	return TRUE;
}


BOOL CMotionMonitorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	CDocument::OnSaveDocument(lpszPathName);

	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		cMotionMonitorConfig &config = cMotionController::Get()->m_config;

		pFrm->UpdateConfig(false);
		config.WriteConfigFile(wstr2str(lpszPathName));
	}

	return TRUE;
}
