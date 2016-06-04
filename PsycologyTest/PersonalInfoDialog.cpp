// PersonalInfoDialog.cpp : implementation file
//

#include "stdafx.h"
#include "PsycologyTest.h"
#include "PersonalInfoDialog.h"
#include "afxdialogex.h"


// CPersonalInfoDialog dialog

IMPLEMENT_DYNAMIC(CPersonalInfoDialog, CDialogEx)

CPersonalInfoDialog::CPersonalInfoDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PERSONAL_INFO_DIALOG, pParent)
{

}

CPersonalInfoDialog::~CPersonalInfoDialog()
{
}

void CPersonalInfoDialog::SetInfo(const PersonalInfo& info)
{
	_name = info.name;
	_name_pinyin = info.name_pinyin;
	_birth_date = info.birth_date;
	_sex = info.sex;
	_nationality = info.nationality;
	_weight = info.weight;
	_mobile = info.mobile;
	_email = info.email;
}

PersonalInfo CPersonalInfoDialog::GetInfo() const
{
	PersonalInfo info;
	info.name = _name;
	info.name_pinyin = _name_pinyin;
	info.birth_date = _birth_date;
	info.sex = Sex(_sex);
	info.nationality = _nationality;
	info.weight = _weight;
	info.mobile = _mobile;
	info.email = _email;

	return info;
}

void CPersonalInfoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPersonalInfoDialog, CDialogEx)
END_MESSAGE_MAP()


// CPersonalInfoDialog message handlers
