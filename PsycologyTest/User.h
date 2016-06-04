#pragma once

enum Sex
{
	SexMale,
	SexFemale,
	SexUnknown,
};

// CPersonalInfoDialog dialog
struct PersonalInfo
{
	CString name;
	CString name_pinyin;
	COleDateTime birth_date;
	Sex sex;	// 0 male, 1 female
	CString nationality;
	unsigned int weight;
	CString mobile;
	CString email;
};

class CUser
{
public:
	CUser(const CString& name, const CString& password);
	CString CreateUid();

	const CString& GetName() const;
	void SetName(const CString& val);

	const CString& GetPassword() const;
	void SetPassword(const CString& passoword);

	void SetUid(const CString& uid);
	const CString& GetUid();
	void SetWorkingFolder(const CString& folder);
	const CString& GetWorkingFolder() const;
	const PersonalInfo& GetInfo() const;
	void SetInfo(const PersonalInfo& info);
protected:
	CString _name;
	CString _password;
	CString _uid;
	CString _working_folder;

	PersonalInfo _info;
};

