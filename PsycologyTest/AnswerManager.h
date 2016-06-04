#pragma once
#include <map>

class CPsiScale;
namespace Utilities
{
	class CXmlElement;
}

struct Answer
{
	//Answer(unsigned a, unsigned t): answer(a), time(t){}
	unsigned answer;
	long time;
};

class CAnswerManager
{
public:
	CAnswerManager();
	~CAnswerManager();

	bool AddAnswer(const CString& scale_name, unsigned question_id, unsigned answer, unsigned time);
	unsigned GetAnswer(const CString& scale_name, unsigned question_id);
	bool SetScore(const CString& scale_name, const CString& sub_scale_name, unsigned score);
	bool IsAnswered(const CString& scale_name, unsigned question_id);

	void SetSubjectId(const TCHAR* subject_id);
	const CString& GetSubjectId() const;
	int CheckForUnansweredQuestion(CPsiScale& scale);
	unsigned GetTotalScore(const CString& scale_name, const CString& sub_scale_name);
	bool LoadScaleItem(Utilities::CXmlElement* scale_xml);
	bool SaveScaleItem(Utilities::CXmlElement* scale_xml, const CString& scale_name);
	bool Load(const CString& test_info_path);
	bool Save(const CString& test_info_path);
	bool ScaleFinished(const CString& scale_name);
	void FinishScale(const CString& scale_name);
private:
	std::map<CString, std::map<unsigned, Answer>> _answers;
	std::map<CString, std::map<CString, unsigned>> _scores; // 用结构是不是更好
	std::map<CString, bool> _test_finished_info;
	
	// 被试uid
	CString _subject_uid;
};

