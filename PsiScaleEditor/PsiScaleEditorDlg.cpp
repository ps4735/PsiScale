
// PsiScaleEditorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PsiScaleEditor.h"
#include "PsiScaleEditorDlg.h"
#include "afxdialogex.h"
#include "../PsiCommon/PsiScale.h"
#include "InputStringDialog.h"
#include "../Utilities/FileSystem.h"
#include "QuestionEditorDlg.h"
#include <algorithm>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace FileSystem;

bool IsShort(const CString& s1, const CString& s2)
{
	return (_ttoi(s1.Left(s1.Find(_T("."))).GetBuffer()) < _ttoi(s2.Left(s2.Find(_T("."))).GetBuffer()));
}
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

CPsiScaleEditorDlg::CPsiScaleEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PSI_SCALE_EDITOR, pParent)
	, _scale_id(0)
	, _scale_name(_T(""))
	, _prologue_text(_T(""))
	, _use_same_choices(FALSE)
	, _working_folder(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPsiScaleEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ID, _scale_id);
	DDX_Text(pDX, IDC_NAME, _scale_name);
	DDX_Text(pDX, IDC_EDIT_PROLOGUE, _prologue_text);
	DDX_Check(pDX, IDC_CHECK_SAME_CHOICE, _use_same_choices);
	DDX_Control(pDX, IDC_ID, _scale_id_edit);
	DDX_Control(pDX, IDC_NAME, _scale_name_edit);
	DDX_Control(pDX, IDC_EDIT_PROLOGUE, _prologue_text_edit);
	DDX_Control(pDX, IDC_CHECK_SAME_CHOICE, _shared_choices_checkbox);
	DDX_Control(pDX, IDC_NEW_SCALE, _new_scale_table_button);
	DDX_Control(pDX, IDC_BUTTON_SAVE, _save_scale_button);
	DDX_Control(pDX, IDCANCEL, _exit_button);
	DDX_Control(pDX, IDC_QUESTION_LIST, _question_list);
	DDX_Control(pDX, IDC_GROUP_LIST, _group_list);
	DDX_Control(pDX, IDC_CHOICE_LIST, _choice_list);
	DDX_Control(pDX, IDC_EDIT_WORKING_FOLDER, _working_folder_edit);
	DDX_Control(pDX, IDC_COMBO_SCALES, _scales_combo);
	DDX_Text(pDX, IDC_EDIT_WORKING_FOLDER, _working_folder);
}

BEGIN_MESSAGE_MAP(CPsiScaleEditorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK_SAME_CHOICE, &CPsiScaleEditorDlg::OnBnClickedCheckSameChoice)
	ON_BN_CLICKED(IDC_NEW_SCALE, &CPsiScaleEditorDlg::OnBnClickedButtonNew)
	ON_LBN_SELCHANGE(IDC_LIST_QUESTIONS, &CPsiScaleEditorDlg::OnLbnSelchangeListQuestions)
	ON_EN_CHANGE(IDC_NAME, &CPsiScaleEditorDlg::OnEnChangeName)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CPsiScaleEditorDlg::OnBnClickedButtonSave)
	ON_EN_CHANGE(IDC_EDIT_WORKING_FOLDER, &CPsiScaleEditorDlg::OnEnChangeEditWorkingFolder)
	ON_CBN_SELCHANGE(IDC_COMBO_SCALES, &CPsiScaleEditorDlg::OnCbnSelchangeComboScales)
	ON_BN_CLICKED(IDC_EDIT_QUESTIONS, &CPsiScaleEditorDlg::OnBnClickedEditQuestions)
	ON_BN_CLICKED(IDCANCEL, &CPsiScaleEditorDlg::OnBnClickedExit)
END_MESSAGE_MAP()


// CPsiScaleEditorDlg ��Ϣ�������

BOOL CPsiScaleEditorDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	_working_folder_edit.EnableFolderBrowseButton();
	CRegKey regkey;
	if (regkey.Open(HKEY_CURRENT_USER, _T("Software\\SKMR\\PsiScale"), KEY_READ) == ERROR_SUCCESS)
	{
		static TCHAR buffer[512];
		ULONG count = 512;
		if (regkey.QueryStringValue(_T("WorkingFolder"), buffer, &count) == ERROR_SUCCESS)
		{
			if (::FileExists(buffer))
			{
				_working_folder_edit.SetWindowText(buffer);
			}
// 			DWORD selected_scale = 0;
// 			if (regkey.QueryDWORDValue(_T("LastScale"), selected_scale) == ERROR_SUCCESS &&
// 				_scales_combo.GetCount() > selected_scale)
// 			{
// 				_scales_combo.SetCurSel(selected_scale);
// 			}
		}
		regkey.Close();
	}

	if (!_scale)
	{
		OnBnClickedButtonNew();
	}
	else
	{
		OnCbnSelchangeComboScales();
	}
	// _question_list.SetStandardButtons(AFX_VSLISTBOX_BTN_UP | AFX_VSLISTBOX_BTN_DOWN);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPsiScaleEditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		__super::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPsiScaleEditorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		__super::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPsiScaleEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPsiScaleEditorDlg::OnBnClickedCheckSameChoice()
{
	UpdateData(TRUE);
	_choice_list.EnableWindow(_use_same_choices);
}

CString CPsiScaleEditorDlg::GetScalePath(const CPsiScale& scale)
{
	CString path = _working_folder + _T("\\");
	path.Format(path + _T("%d.%s.scale"), scale.GetId(), scale.GetName());

	return path;
}

void CPsiScaleEditorDlg::OnBnClickedButtonNew()
{
	if (_scale)
	{
		UpdateScaleComboCurrentItem();
		_scale->Save(GetScalePath(*_scale));
	}
	_scale = shared_ptr<CPsiScale>(new CPsiScale);
	_scales_combo.AddString(_T("�½�����"));
	_scales_combo.SelectString(0, _T("�½�����"));

	UpdateUi();

	UpdateData(FALSE);
}


void CPsiScaleEditorDlg::UpdateScaleComboCurrentItem()
{
	CString file_path = GetScalePath(*_scale);
	if (_scales_combo.GetCurSel() != LB_ERR)
	{
		CString combo_text;
		_scales_combo.GetLBText(_scales_combo.GetCurSel(), combo_text);
		if (combo_text != GetFileNameFromPath(file_path))
		{
			_scales_combo.DeleteString(_scales_combo.GetCurSel());
			auto index = _scales_combo.AddString(GetFileNameFromPath(file_path));
			_scales_combo.SetCurSel(index);
		}

	}
}

void CPsiScaleEditorDlg::OnBnClickedButtonAddQuestion()
{
	ASSERT(_scale);

	CPsiScaleQuestion new_question;
	_scale->AddQuestion(new_question);
	_question_list.AddItem(_T("����Ŀ"));

	UpdateUi();
}

void CPsiScaleEditorDlg::UpdateScale()
{
	if (!_scale)
		return;

	UpdateData();
	_scale->SetId(_scale_id);
	_scale->SetName(_scale_name);
	_scale->SetPrologue(_prologue_text);

	_scale->SetSameChoice(_use_same_choices != FALSE);
	if (_use_same_choices != FALSE)
	{
		auto& choices = _scale->Choices();
		choices.resize(_choice_list.GetCount());
		for (int i = 0; i < _choice_list.GetCount(); ++i)
		{
			choices[i].id = i + 1;
			choices[i].text = _choice_list.GetItemText(i);
		}
	}

	auto& groups = _scale->Groups();
	groups.resize(_group_list.GetCount());
	for (int i = 0; i < _group_list.GetCount(); ++i)
	{
		groups[i] = _group_list.GetItemText(i);
	}
}

void CPsiScaleEditorDlg::UpdateUi()
{
	_scale_id = _scale->GetId();
	_scale_name = _scale->GetName();
	_prologue_text = _scale->GetPrologue();
	_use_same_choices = _scale->IsSameChoice();

	_choice_list.EnableWindow(_use_same_choices);

	ClearLists();

	for (unsigned int i = 0; i < _scale->GetQuestionCount(); ++i)
	{
		auto question = _scale->GetQuestion(i);
		_question_list.AddItem(question.GetText(), question.GetId());
	}

	for (unsigned int i = 0; i < _scale->GetGroupCount(); ++i)
	{
		auto group = _scale->GetGroup(i);
		_group_list.AddItem(group);
	}

	for (auto choice : _scale->Choices())
	{
		_choice_list.AddItem(choice.text, choice.id);
	}

	OnQuestionChange();

	UpdateData(FALSE);
}

void CPsiScaleEditorDlg::OnQuestionChange()
{
	if (!_scale)
		return;

	int selected = _question_list.GetSelItem();
	if (selected != -1)
	{
		auto question = _scale->GetQuestion(selected);
		if (_use_same_choices == FALSE)
		{
			// ���µ�ǰ�����ѡ��
			while (_choice_list.GetCount() > 0)
			{
				_choice_list.RemoveItem(0);
			}
			for (auto choice : question.Choices())
			{
				_choice_list.AddItem(choice.text, choice.id);
			}
		}
		_group_list.SelectString(question.GetGroup());
	}

	UpdateData(FALSE);
}

void CPsiScaleEditorDlg::ClearLists()
{
	while (_question_list.GetCount() > 0)
	{
		_question_list.RemoveItem(0);
	}
	while (_choice_list.GetCount() > 0)
	{
		_choice_list.RemoveItem(0);
	}
	while (_group_list.GetCount() > 0)
	{
		_group_list.RemoveItem(0);
	}
}

void CPsiScaleEditorDlg::OnLbnSelchangeListQuestions()
{
	if (_question_list.GetSelItem() == LB_ERR)
		return;

	OnQuestionChange();
}


void CPsiScaleEditorDlg::OnEnChangeName()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CPsiScaleEditorDlg::OnBnClickedButtonSave()
{
	if (!_scale)
		return;

	UpdateScale();

	CString scale_combo_text;
	if (_scales_combo.GetCurSel() != LB_ERR)
	{
		_scales_combo.GetLBText(_scales_combo.GetCurSel(), scale_combo_text);
	}

	if (!scale_combo_text.IsEmpty())
	{
		CString old_path = _working_folder + _T("\\") + scale_combo_text + _T(".scale");
		if (::FileExists(old_path))
		{
			CFile::Remove(old_path);
		}
	}

	_scale->Save(GetScalePath(*_scale));
	UpdateScaleComboCurrentItem();
}


void CPsiScaleEditorDlg::OnEnChangeEditWorkingFolder()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateData();
	if (::FileExists(_working_folder))
	{
		std::vector<CString> files;
		::ForEachFile(_working_folder, _T("*.scale"), false, [&](const CString& file) {
			CString filename = ::GetFileNameFromPath(file);
			files.push_back(filename);
		});

		std::sort(files.begin(), files.end(), IsShort);
		std::for_each(files.begin(), files.end(), [this](CString item){
			_scales_combo.AddString(item); });

		if (_scales_combo.GetCount() > 0)
		{
			_scales_combo.SetCurSel(0);
			OnCbnSelchangeComboScales();
		}

		CRegKey regkey;
		if (regkey.Open(HKEY_CURRENT_USER, _T("Software\\SKMR\\PsiScale"), KEY_WRITE) == ERROR_SUCCESS ||
			regkey.Create(HKEY_CURRENT_USER, _T("Software\\SKMR\\PsiScale")) == ERROR_SUCCESS)
		{
			regkey.SetStringValue(_T("WorkingFolder"), _working_folder);
			regkey.Close();
		}
	}
}


void CPsiScaleEditorDlg::OnCbnSelchangeComboScales()
{
	if (_scales_combo.GetCurSel() == LB_ERR)
		return;

	// ����ScaleCombo��ѡ������ݴ���ص�����
	CString selected_text;
	_scales_combo.GetLBText(_scales_combo.GetCurSel(), selected_text);
	CString file_path = _working_folder + _T("\\") + selected_text + _T(".scale");

	if (!_scale)
	{
		try
		{
			_scale = shared_ptr<CPsiScale>(new CPsiScale);
		}
		catch (CMemoryException* )
		{
			return;
		}
	}

	_scale->Load(file_path);

	if (!_scale)
	{
		CString error_message;
		error_message.Format(_T("�޷����������ļ���%s"), file_path);
		AfxMessageBox(error_message);
		return;
	}

	UpdateUi();
}


void CPsiScaleEditorDlg::OnBnClickedEditQuestions()
{
	if (!_scale)
	{
		AfxMessageBox(_T("���ȴ�һ��������ߴ���һ���µ�����"));
		return;
	}

	UpdateScale();

	CQuestionEditorDlg dlg(_scale, this);
	if (dlg.DoModal() == IDOK)
	{
		UpdateUi();
	}
}

void CPsiScaleEditorDlg::OnBnClickedExit()
{
	if (AfxMessageBox(_T("�Ƿ񱣴������޸ģ�"), MB_OKCANCEL) == IDOK)
	{
		OnBnClickedButtonSave();
	}
	CDialogEx::OnCancel();
}


