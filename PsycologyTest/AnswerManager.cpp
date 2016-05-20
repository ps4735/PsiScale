#include "stdafx.h"
#include "AnswerManager.h"


CAnswerManager::CAnswerManager()
{
}

CAnswerManager::~CAnswerManager()
{
}

bool CAnswerManager::AddAnswer(unsigned table_id, 
	unsigned question_id, 
	unsigned answer)
{
	_answers[table_id][question_id] = answer;
	return true;
}

unsigned CAnswerManager::GetAnswer(unsigned table_id, unsigned question_id)
{
	auto table = _answers.find(table_id);
	if (table == _answers.end())
	{
		throw _T("Table not found.");
	}
	auto question = table->second.find(question_id);
	if (question == table->second.end())
	{
		throw _T("Question not found.");
	}

	return question->second;
}

bool CAnswerManager::IsAnswered(unsigned table_id, unsigned question_id)
{
	auto table = _answers.find(table_id);
	if (table == _answers.end())
	{
		return false;
	}

	return (table->second.find(question_id) != table->second.end());
}

void CAnswerManager::SetSubjectId(const TCHAR* subject_id)
{
	_subject_id = subject_id;
}

const CString& CAnswerManager::GetSubjectId() const
{
	return _subject_id;
}
