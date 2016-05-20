
// PsycologyTestDlg.h : header file
//

#pragma once

#include "../PsiCommon/TestManager.h"
#include "afxwin.h"
#include <memory>
#include "AnswerManager.h"

// CPsycologyTestDlg dialog
class CPsycologyTestDlg : public CDialogEx
{
// Construction
public:
	CPsycologyTestDlg(std::shared_ptr<PsiScale> scale, CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PSYCOLOGYTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CTestManager _test_manager;
	CAnswerManager _answer_manager;
	std::shared_ptr<PsiScale> _psi_scale;
	unsigned _current_question_index;

	// Generated message map functions
	bool ShowQuestion(unsigned question_index);
	bool ShowButtons(unsigned choice_count);
	void MoveButtonUp(CWnd& button, unsigned int y_pos);

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedPrev();
	afx_msg void OnBnClickedNext();
	CString _question;

	void ProcessAnswer(unsigned int answer);
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	void Check(int button_to_check);
	void UncheckAll();

	void AdjustSize(int last_button);
	CString _question_number;
};
