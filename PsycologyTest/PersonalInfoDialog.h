#pragma once
#include "User.h"


class CPersonalInfoDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CPersonalInfoDialog)

public:
	CPersonalInfoDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPersonalInfoDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PERSONAL_INFO_DIALOG };
#endif

	void SetInfo(const PersonalInfo& info);
	PersonalInfo GetInfo() const;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString _name;
	CString _name_pinyin;
	COleDateTime _birth_date;
	int _sex;	// 0 male, 1 female
	CString _nationality;
	unsigned int _weight;
	CString _mobile;
	CString _email;

	DECLARE_MESSAGE_MAP()
};
