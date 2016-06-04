
// PsiAnswerViewerDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CPsiAnswerViewerDlg dialog
class CPsiAnswerViewerDlg : public CDialogEx
{
// Construction
public:
	CPsiAnswerViewerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PSIANSWERVIEWER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


	bool InitialScaleList();
	CString _working_folder;
public:
	CListCtrl answer_table;
	CComboBox _combo_scale;
};
