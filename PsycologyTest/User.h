#pragma once

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

protected:
	CString _name;
	CString _password;
	CString _uid;
	CString _working_folder;
};

