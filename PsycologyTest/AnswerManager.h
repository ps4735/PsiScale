#pragma once
#include <map>

class CPsiScale;

class CAnswerManager
{
public:
	CAnswerManager();
	~CAnswerManager();

	bool AddAnswer(unsigned table_id, unsigned question_id, unsigned answer);
	unsigned GetAnswer(unsigned table_id, unsigned question_id);

	bool IsAnswered(unsigned table_id, unsigned question_id);

	void SetSubjectId(const TCHAR* subject_id);
	const CString& GetSubjectId() const;
	int CheckForUnansweredQuestion(CPsiScale& scale);
private:
	std::map<unsigned, std::map<unsigned, unsigned>> _answers;

	CString _subject_id;
};

