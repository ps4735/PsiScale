#include "stdafx.h"
#include "AnswerManager.h"
#include "..\PsiCommon\PsiScale.h"
#include <algorithm>
#include "..\Utilities\xml.h"
namespace TestInfo
{
	const TCHAR* XML_TEST = _T("Test");
	const TCHAR* XML_SCALE = _T("Scale");
	const TCHAR* XML_PARTICIPANT_UID = _T("PaticipantUid");
	const TCHAR* XML_NAME = _T("Name");
	const TCHAR* XML_FINISHED = _T("Finished");
	const TCHAR* XML_ANSWERS = _T("Answers");
	const TCHAR* XML_ITEM = _T("Item");
	const TCHAR* XML_ID = _T("Id");
	const TCHAR* XML_ANSWER = _T("Answer");
	const TCHAR* XML_SUBSCALES = _T("SubScales");
	const TCHAR* XML_SUBSCALE = _T("SubScale");
	const TCHAR* XML_SCORE = _T("Score");
}
using namespace TestInfo;
CAnswerManager::CAnswerManager()
{
}

CAnswerManager::~CAnswerManager()
{
}

bool CAnswerManager::AddAnswer(const CString& scale_name, 
	unsigned question_id, 
	unsigned answer)
{
	_test_finished_info[scale_name] = false;
	_answers[scale_name][question_id] = answer;
	return true;
}

unsigned CAnswerManager::GetAnswer(const CString& scale_name, unsigned question_id)
{
	auto table = _answers.find(scale_name);
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

bool CAnswerManager::IsAnswered(const CString& scale_name, unsigned question_id)
{
	auto table = _answers.find(scale_name);
	if (table == _answers.end())
	{
		return false;
	}

	return (table->second.find(question_id) != table->second.end());
}

void CAnswerManager::SetSubjectId(const TCHAR* subject_id)
{
	_subject_uid = subject_id;
}

const CString& CAnswerManager::GetSubjectId() const
{
	return _subject_uid;
}

/**
@return 如果所有的问题都已经回答，返回-1；否则返回第一个没有回答的题目的索引
*/

int CAnswerManager::CheckForUnansweredQuestion(CPsiScale& scale)
{
	auto iter = _answers.find(scale.GetName());
	if (iter == _answers.end())
		return 0;

	auto& answers = iter->second;
	if (answers.size() == scale.GetQuestionCount())
		return -1;

	for (int i = 0; i < int(scale.GetQuestionCount()); ++i)
	{
		if (answers.find(i) == answers.end())
			return i;
	}

	return -1;
}

// 暂时以加法运算进行累加
bool CAnswerManager::SetScore(const CString& scale_name, const CString& sub_scale_name, unsigned score)
{
	_scores[scale_name][sub_scale_name] += score;
	return true;
}

unsigned CAnswerManager::GetTotalScore(const CString& scale_name, const CString& sub_scale_name)
{
	unsigned sum = 0;
	auto iter = _scores.find(scale_name);
	if (iter != _scores.end())
	{
		std::for_each(iter->second.begin(), iter->second.end(), [&](std::pair<CString, unsigned> item) {
			sum = item.second; });
	}
	return sum;
}

// 每个scale绑定一个answermanager，暂时的设计
bool CAnswerManager::LoadScaleItem(Utilities::CXmlElement* scale_xml)
{
	CString scale_name = scale_xml->GetAttrib(XML_NAME);
	auto answers_element = scale_xml->GetElement(XML_ANSWERS);
	auto answer_items = answers_element->GetChildElements();
	std::map<unsigned int, unsigned int> answer_item;
	for_each(answer_items.begin(), answer_items.end(), [&](Utilities::CXmlElement* item) {
		answer_item.insert(std::make_pair(item->GetIntegerAttrib(XML_ID), item->GetIntegerAttrib(XML_ANSWER))); });
	_answers[scale_name] = answer_item;
	auto subscales_element = scale_xml->GetElement(XML_SUBSCALES);
	auto subscales_items = subscales_element->GetChildElements();
	std::map<CString, unsigned int> scores;
	for_each(subscales_items.begin(), subscales_items.end(), [&](Utilities::CXmlElement* item) {
		scores.insert(std::make_pair(item->GetAttrib(XML_NAME), item->GetIntegerAttrib(XML_SCORE))); });
	_scores[scale_name] = scores;
	return true;
}

bool CAnswerManager::SaveScaleItem(Utilities::CXmlElement* scale_xml, const CString& scale_name)
{
	auto scale_iter = _answers.find(scale_name);

	if (scale_iter != _answers.end())
	{
		auto answers_xml = scale_xml->AddElement(XML_ANSWERS);
		for (auto iter = scale_iter->second.begin(); iter != scale_iter->second.end(); ++iter)
		{
			auto item = answers_xml->AddElement(XML_ITEM);
			item->SetIntegerAttrib(XML_ID, iter->first);
			item->SetIntegerAttrib(XML_ANSWER, iter->second);
		}
	}

	auto subscale_iter = _scores.find(scale_name);
	if (subscale_iter != _scores.end())
	{
		auto subscales_xml = scale_xml->AddElement(XML_SUBSCALES);
		for (auto iter = subscale_iter->second.begin(); iter != subscale_iter->second.end(); ++iter)
		{
			auto item = subscales_xml->AddElement(XML_SUBSCALE);
			item->SetAttrib(XML_NAME, iter->first);
			item->SetIntegerAttrib(XML_SCORE, iter->second);
		}
	}

	return true;
}

bool CAnswerManager::Load(const CString& test_info_path)
{
	Utilities::CXml xml;
	if (!xml.Load(test_info_path))
	{
		return false;
	}
	_subject_uid = xml.GetAttrib(XML_PARTICIPANT_UID);
	auto scales = xml.GetChildElements();
	for (unsigned int i = 0; i < scales.size(); ++i)
	{
		auto scale_xml = scales[i];
		_test_finished_info.insert(std::make_pair(scale_xml->GetAttrib(XML_NAME), (scale_xml->GetIntegerAttrib(XML_FINISHED) == 0) ? false : true));
		LoadScaleItem(scale_xml);
	}

	return true;
}

bool CAnswerManager::Save(const CString& test_info_path)
{
	Utilities::CXml xml(XML_TEST);
	xml.SetAttrib(XML_PARTICIPANT_UID, _subject_uid);
	for (auto iter = _test_finished_info.begin(); iter != _test_finished_info.end(); ++iter)
	{
		auto scale_xml = xml.AddElement(XML_SCALE);
		scale_xml->SetAttrib(XML_NAME, iter->first);
		scale_xml->SetIntegerAttrib(XML_FINISHED, iter->second);
		this->SaveScaleItem(scale_xml, iter->first);
	}

	return xml.Save(test_info_path);
}

bool CAnswerManager::ScaleFinished(const CString& scale_name)
{
	auto iter = _test_finished_info.find(scale_name);
	if (iter != _test_finished_info.end())
	{
		return iter->second;
	}

	return false;
}

void CAnswerManager::FinishScale(const CString& scale_name)
{
	_test_finished_info[scale_name] = true;
}