#pragma once
#include <memory>
#include <map>

class CUser;

class CUserManager
{
public:
	CUserManager();
	virtual ~CUserManager();
	bool Init();
	std::shared_ptr<CUser> CreateUser(const CString& name, const CString& password);
	std::shared_ptr<CUser> GetUser(const CString& name, const CString& password);
	bool Load();
	bool Save();

private:
	bool UserHasExist(const CString& name, const CString& password);
	void SetWorkingFolder(std::shared_ptr<CUser> user);
	std::map<CString, std::shared_ptr<CUser>> _users;
	std::multimap<CString, CString> _user_name_to_uid;	
	CString _user_info_path;
};

