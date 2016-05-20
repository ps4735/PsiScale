
// PsiScaleEditorDlg.h : ͷ�ļ�
//

#pragma once

#include <memory>
#include "afxwin.h"
#include "afxvslistbox.h"
#include "QuestionListBox.h"
#include "afxeditbrowsectrl.h"
#include "ChoiceList.h"
#include "GroupList.h"

class CPsiScale;
class CPsiScaleQuestion;


// CPsiScaleEditorDlg �Ի���
class CPsiScaleEditorDlg : public CDialogEx
{
// ����
public:
	CPsiScaleEditorDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PSI_SCALE_EDITOR };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;
	std::shared_ptr<CPsiScale> _scale;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckSameChoice();
	afx_msg void OnBnClickedButtonNew();

	void UpdateScaleComboCurrentItem();

	UINT _scale_id;
	CString _scale_name;
	CString _prologue_text;
	BOOL _use_same_choices;
	
	void UpdateUi();
	void OnQuestionChange();

	void UpdateScale();
	void ClearLists();
	CString GetScalePath(const CPsiScale& scale);

	afx_msg void OnBnClickedButtonAddQuestion();
	afx_msg void OnLbnSelchangeListQuestions();
	afx_msg void OnEnChangeName();
	afx_msg void OnBnClickedButtonSave();

	CButton _add_question_button;
	CButton _delete_question_button;
	CEdit _scale_id_edit;
	CEdit _scale_name_edit;
	CEdit _prologue_text_edit;
	CButton _shared_choices_checkbox;
	CButton _new_scale_table_button;
	CButton _save_scale_button;
	CButton _exit_button;

	CQuestionListBox _question_list;
	CGroupList _group_list;
	CChoiceList _choice_list;

	CMFCEditBrowseCtrl _working_folder_edit;
	CComboBox _scales_combo;
	CString _working_folder;

	afx_msg void OnEnChangeEditWorkingFolder();
	afx_msg void OnCbnSelchangeComboScales();
	afx_msg void OnBnClickedEditQuestions();
	afx_msg void OnBnClickedExit();
};
