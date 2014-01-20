#include "stdafx.h"
#include "Styler.h"
#include "tinyxml/tinyxml.h"


////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------
CStringW UTF8ToUnicode(const char* utf8)
{
	CStringW uni = L"";
	if (utf8 == 0)
		return uni;

	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, utf8, (int)strlen(utf8), NULL, 0);
	wchar_t * uni16 = new wchar_t[wcsLen + 1];
	::MultiByteToWideChar(CP_UTF8, NULL, utf8, (int)strlen(utf8), uni16, wcsLen);
	uni16[wcsLen] = '\0';
	uni = uni16;
	delete [] uni16;
	return uni;
}
CStringA UnicodeToUTF8(const wchar_t* uni16)
{
	CStringA utf = "";
	if (uni16 == 0)
		return utf;

	int csLen = ::WideCharToMultiByte(CP_UTF8, NULL, uni16, (int)wcslen(uni16), NULL, 0, NULL, NULL);
	char * utf8 =   new char[csLen + 1];
	::WideCharToMultiByte(CP_UTF8, NULL, uni16, (int)wcslen(uni16), utf8, csLen, NULL, NULL);
	utf8[csLen] = '\0';
	utf = utf8;
	delete [] utf8;
	return utf;
}
CStringW ANSIToUnicode(const char* ansi)
{
	CStringW uni = L"";
	if (ansi == 0)
		return uni;

	int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, ansi, (int)strlen(ansi), NULL, 0);
	wchar_t * uni16 = new wchar_t[wcsLen + 1];
	::MultiByteToWideChar(CP_ACP, NULL, ansi, (int)strlen(ansi), uni16, wcsLen);
	uni16[wcsLen] = '\0';
	uni = uni16;
	delete [] uni16;
	return uni;
}
CStringA UnicodeToANSI(const wchar_t* uni16)
{
	CStringA ans = "";
	if (uni16 == 0)
		return ans;

	int csLen = ::WideCharToMultiByte(CP_ACP, NULL, uni16, (int)wcslen(uni16), NULL, 0, NULL, NULL);
	char * ansi =   new char[csLen + 1];
	::WideCharToMultiByte(CP_ACP, NULL, uni16, (int)wcslen(uni16), ansi, csLen, NULL, NULL);
	ansi[csLen] = '\0';
	ans = ansi;
	delete [] ansi;
	return ans;
}
//--------------------------------------------------------------------



CStyler::CStyler(void)
: m_path(_T(""))
{
	m_offset = 0;
}

CStyler::~CStyler(void)
{
	Free();
}

int CStyler::Free()
{
	for (int i=0; i<(int)m_yyers.size(); i++)
	{
		m_yyers[i].lrcs.clear();
	}
	m_yyers.clear();

	return 1;
}

int CStyler::New()
{
	Free();
	SYYer yyer;
	yyer.idx = -1;
	m_yyers.push_back(yyer);

	return 1;
}

int CStyler::New(CString path)
{
	Free();
	SYYer yyer;
	yyer.idx = -1;
	m_yyers.push_back(yyer);

	m_path = path;

	return 1;
}

int CStyler::Open()
{
	Free();
	return Read();
}

int CStyler::Open(CString path)
{
	m_path = path;
	Free();
	return Read();
}

int CStyler::Save()
{
	return Write();
}

int CStyler::SaveAs(CString path)
{
	m_path = path;
	return Write();
}

int CStyler::Read()
{
	USES_CONVERSION;

	TiXmlDocument doc;
#ifdef UNICODE
	if(!doc.LoadFile(W2A((LPCTSTR)m_path)))
#else
	if(!doc.LoadFile((LPCTSTR)m_path))
#endif
	{
		return 0;
	}

	Free();

	std::string strVersion;
	TiXmlNode* pXmlFirst = doc.FirstChild();
	if (NULL != pXmlFirst)
	{
		TiXmlDeclaration* pXmlDec = pXmlFirst->ToDeclaration();
		if (NULL != pXmlDec)
		{
			strVersion = pXmlDec->Version();
		}
	}
	if (!strVersion.compare("1.0"))
	{
		TiXmlElement* root = doc.FirstChildElement("styler");
		if (root == NULL)
		{
			return 0;
		}

		TiXmlElement* titleElem = root->FirstChildElement("title");
		if (titleElem != NULL)
		{
			TiXmlNode* node = titleElem->FirstChild();
			if (node != NULL)
				m_title = UTF8ToUnicode(node->Value());
		}

		TiXmlElement* artistElem = root->FirstChildElement("artist");
		if (artistElem != NULL)
		{
			TiXmlNode* node = artistElem->FirstChild();
			if (node != NULL)
				m_artist = UTF8ToUnicode(node->Value());
		}

		TiXmlElement* madebyElem = root->FirstChildElement("madeby");
		if (madebyElem != NULL)
		{
			TiXmlNode* node = madebyElem->FirstChild();
			if (node != NULL)
				m_madeby = UTF8ToUnicode(node->Value());
		}

		TiXmlElement* offsetElem = root->FirstChildElement("offset");
		if (offsetElem != NULL)
		{
			TiXmlNode* node = offsetElem->FirstChild();
			if (node != NULL)
				m_offset = atoi(node->Value());
		}

		TiXmlElement* yyerElem = root->FirstChildElement("yyers");
		while(yyerElem != NULL)
		{
			SYYer yyer;
			yyer.idx = -1;
			TiXmlElement* lyricElem = yyerElem->FirstChildElement("lyric");
			while(lyricElem != NULL)
			{
				SLyric lrc;
				lrc.time = atoi(lyricElem->Attribute("time")) - m_offset;
				TiXmlNode* node = lyricElem->FirstChild();
				if (node != NULL)
					lrc.str = UTF8ToUnicode(node->Value());

				yyer.lrcs.push_back(lrc);
				lyricElem = lyricElem->NextSiblingElement("lyric");
			}
			if (yyer.lrcs.size() != 0)
			{
				SLyric lrc;
				lrc.time = yyer.lrcs.back().time + 5000;
				yyer.lrcs.push_back(lrc);
			}

			m_yyers.push_back(yyer);
			yyerElem = yyerElem->NextSiblingElement("yyers");
		}
	}
	else if (!strVersion.compare("2.0"))
	{
		TiXmlElement* root = doc.FirstChildElement("styler");
		if (root == NULL)
		{
			return 0;
		}

		TiXmlElement* titleElem = root->FirstChildElement("title");
		if (titleElem != NULL)
		{
			m_title = UTF8ToUnicode(titleElem->Attribute("value"));
		}

		TiXmlElement* artistElem = root->FirstChildElement("artist");
		if (artistElem != NULL)
		{
			m_artist = UTF8ToUnicode(artistElem->Attribute("value"));
		}

		TiXmlElement* madebyElem = root->FirstChildElement("madeby");
		if (madebyElem != NULL)
		{
			m_madeby = UTF8ToUnicode(madebyElem->Attribute("value"));
		}

		TiXmlElement* offsetElem = root->FirstChildElement("offset");
		if (offsetElem != NULL)
		{
			offsetElem->Attribute("value", &m_offset);
		}

		TiXmlElement* yyerElem = root->FirstChildElement("yyers");
		while(yyerElem != NULL)
		{
			SYYer yyer;
			yyer.idx = -1;
			TiXmlElement* lyricElem = yyerElem->FirstChildElement("lyric");
			while(lyricElem != NULL)
			{
				SLyric lrc;
				lrc.time = atoi(lyricElem->Attribute("time")) - m_offset;
				lrc.str  = UTF8ToUnicode(lyricElem->Attribute("value"));

				yyer.lrcs.push_back(lrc);
				lyricElem = lyricElem->NextSiblingElement("lyric");
			}
			if (yyer.lrcs.size() != 0)
			{
				SLyric lrc;
				lrc.time = yyer.lrcs.back().time + 5000;
				yyer.lrcs.push_back(lrc);
			}

			m_yyers.push_back(yyer);
			yyerElem = yyerElem->NextSiblingElement("yyers");
		}
	}

	return 1;
}

int CStyler::Write()
{
	USES_CONVERSION;

	TiXmlDocument doc;

	TiXmlDeclaration* declaration = new TiXmlDeclaration("2.0", "utf-8", "");
	doc.LinkEndChild(declaration);

	TiXmlElement* root = new TiXmlElement("styler");

	TiXmlText* text = NULL;

	TiXmlElement* titleElem = new TiXmlElement("title");
	titleElem->SetAttribute("value", UnicodeToUTF8((LPCTSTR)m_title));
	root->LinkEndChild(titleElem);

	TiXmlElement* artistElem = new TiXmlElement("artist");
	artistElem->SetAttribute("value", UnicodeToUTF8((LPCTSTR)m_artist));
	root->LinkEndChild(artistElem);

	TiXmlElement* madebyElem = new TiXmlElement("madeby");
	madebyElem->SetAttribute("value", UnicodeToUTF8((LPCTSTR)m_madeby));
	root->LinkEndChild(madebyElem);

	TiXmlElement* offsetElem = new TiXmlElement("offset");
	offsetElem->SetAttribute("value", m_offset);
	root->LinkEndChild(offsetElem);

	vector<SYYer>::iterator itYYer = m_yyers.begin();
	for (; itYYer!=m_yyers.end(); itYYer++)
	{
		TiXmlElement* yyerElem = new TiXmlElement("yyers");

		vector<SLyric>::iterator itLrc = itYYer->lrcs.begin();
		for (; itLrc!=itYYer->lrcs.end(); itLrc++)
		{
			TiXmlElement* lrcElem = new TiXmlElement("lyric");
			lrcElem->SetAttribute("time", itLrc->time + m_offset);
			lrcElem->SetAttribute("value", UnicodeToUTF8((LPCTSTR)itLrc->str));
			yyerElem->LinkEndChild(lrcElem);
		}
		root->LinkEndChild(yyerElem);
	}
	doc.LinkEndChild(root);

#ifdef UNICODE
	return doc.SaveFile(W2A((LPCTSTR)m_path));
#else
	return doc.SaveFile((LPCTSTR)m_path);
#endif
}

CString CStyler::getPath()
{
	return m_path;
}

int CStyler::getYYerCount()
{
	return (int)m_yyers.size();
}

int CStyler::getYYerLrcsCount(int idx)
{
	return (int)(getLyrics(idx).size());
}

vector<SLyric>& CStyler::getLyrics(int idx)
{
// 	if (idx < 0 || idx >= getYYerCount())
// 		return 
	return m_yyers[idx].lrcs;
}

int CStyler::calcYYerLrc(int iYY, int time)
{
	SYYer& yyer = m_yyers[iYY];

	vector<SLyric>::iterator it = yyer.lrcs.begin();
	for (; it!=yyer.lrcs.end(); it++)
	{
		if (time < it->time)
			break;
	}

	int idx = (int)(it - yyer.lrcs.begin()) - 1;
	if (idx >= (int)yyer.lrcs.size()-1)
	{
		return 2;
	}
	else if (idx == yyer.idx)
		return 0;
	else
	{
		yyer.idx = idx;
		return 1;
	}
}

int CStyler::getYYerLrcTime(int idxYY, int idxLrc)
{
	return m_yyers[idxYY].lrcs[idxLrc].time;
}

int CStyler::getYYerLrcIdx(int idxYY, int time)
{
	int cn = (int)m_yyers[idxYY].lrcs.size();
	for (int i=0; i<cn; i++)
	{
		if (time == m_yyers[idxYY].lrcs[i].time)
			return i;
	}
	return -1;
}

int CStyler::getYYerCurIdx(int iYY)
{
	return m_yyers[iYY].idx;
}

CString CStyler::getYYerCurLrc(int iYY)
{
	SYYer& yyer = m_yyers[iYY];
	if (yyer.idx < 0 || yyer.idx >= (int)yyer.lrcs.size()-1)
		return _T("");
	return yyer.lrcs[yyer.idx].str;
}

int CStyler::getYYerCurTime(int iYY)
{
	SYYer& yyer = m_yyers[iYY];
	if (yyer.idx < 0 || yyer.idx >= (int)yyer.lrcs.size()-1)
		return -1;
	return yyer.lrcs[yyer.idx].time;
}

int CStyler::getNextTimePoint(int time)
{
	int next = getMaxTime();
	vector<SYYer>::iterator itYYer = m_yyers.begin();
	for (; itYYer!=m_yyers.end(); itYYer++)
	{
		vector<SLyric>::iterator itLrc = itYYer->lrcs.begin();
		for (; itLrc!=itYYer->lrcs.end(); itLrc++)
		{
			if (time < itLrc->time)
			{
				if (next > itLrc->time)
					next = itLrc->time;
				break;
			}
		}
	}

	return next;
}

int CStyler::getPrevTimePoint(int time)
{
	int prev = 0;
// 	vector<SYYer>::iterator itYYer = m_yyers.end()-1;
// 	for (; itYYer!=m_yyers.begin(); itYYer--)
// 	{
// 		vector<SLyric>::iterator itLrc = itYYer->lrcs.end()-1;
// 		for (; itLrc!=itYYer->lrcs.begin(); itLrc--)
// 		{
// 			if (time > itLrc->time)
// 			{
// 				if (prev < itLrc->time)
// 					prev = itLrc->time;
// 				break;
// 			}
// 		}
// 	}
	int iYYer = m_yyers.size()-1;
	for (; iYYer>=0; iYYer--)
	{
		int iLrc = m_yyers[iYYer].lrcs.size()-1;
		for (; iLrc>=0; iLrc--)
		{
			int tst = m_yyers[iYYer].lrcs[iLrc].time;
			if (time > tst)
			{
				if (prev < tst)
					prev = tst;
				break;
			}
		}
	}

	return prev;
}

int CStyler::getMaxTime()
{
	int max = 0;
	int tmp = 0;
	vector<SYYer>::iterator it = m_yyers.begin();
	for (; it!=m_yyers.end(); it++)
	{
		if (!it->lrcs.empty())
		{
			tmp = it->lrcs.back().time;
			if (max < tmp)
				max = tmp;
		}
	}

	return max;
}

int CStyler::getOffset()
{
	return m_offset;
}

void CStyler::reset()
{
	vector<SYYer>::iterator it = m_yyers.begin();
	for (; it!=m_yyers.end(); it++)
	{
		it->idx = -1;
	}
}

int CStyler::addYYerTime(int idx, int time)
{
	SYYer& yyer = m_yyers[idx];
	vector<SLyric>::iterator it = yyer.lrcs.begin();
	for (; it!=yyer.lrcs.end(); it++)
	{
		if (time < it->time)
		{
			break;
		}
	}
	SLyric lyric;
	lyric.time = time;
	lyric.str  = _T("");
	yyer.lrcs.insert(it, lyric);
	return 1;
}

int CStyler::addYYersTime(int time)
{
	vector<SYYer>::iterator it1 = m_yyers.begin();
	for (; it1!=m_yyers.end(); it1++)
	{
		vector<SLyric>::iterator it = it1->lrcs.begin();
		for (; it!=it1->lrcs.end(); it++)
		{
			if (time < it->time)
			{
				break;
			}
		}
		SLyric lyric;
		lyric.time = time;
		lyric.str  = _T("");
		it1->lrcs.insert(it, lyric);
	}

	return 1;
}

int CStyler::editTimeLrc(int idxYY, int idxLrc, int time)
{
	if (idxYY < 0 || idxYY > getYYerCount()-1)
		return 0;
	if (idxLrc < 0 || idxLrc > getYYerLrcsCount(idxYY)-1)
		return 0;

	SLyric& lyric = m_yyers[idxYY].lrcs[idxLrc];
	if (time > 0)
		lyric.time = time;

	return 1;
}

int CStyler::editTimeLrc(int idxYY, int idxLrc, CString lrc)
{
	if (idxYY < 0 || idxYY > getYYerCount()-1)
		return 0;
	if (idxLrc < 0 || idxLrc > getYYerLrcsCount(idxYY)-1)
		return 0;

	SLyric& lyric = m_yyers[idxYY].lrcs[idxLrc];
	lyric.str  = lrc;

	return 1;
}

int CStyler::editTimeLrc(int idxYY, int idxLrc, int time, CString lrc)
{
	if (idxYY < 0 || idxYY > getYYerCount()-1)
		return 0;
	if (idxLrc < 0 || idxLrc > getYYerLrcsCount(idxYY)-1)
		return 0;

	SLyric& lyric = m_yyers[idxYY].lrcs[idxLrc];
	if (time > 0)
		lyric.time = time;
	lyric.str  = lrc;

	return 1;
}

int CStyler::delYYerTime(int idxYY, int idxLrc)
{
	m_yyers[idxYY].lrcs.erase(m_yyers[idxYY].lrcs.begin() + idxLrc);
	return 1;
}

int CStyler::addYYer(int num/* =1 */)
{
// 	if (getYYerCount() == 0)
// 	{
// 		SYYer yyer;
// 		yyer.idx = -1;
// 		m_yyers.push_back(yyer);
// 	}
	for (int i=0; i<num; i++)
	{
		SYYer yyer;
		yyer.idx = -1;
		m_yyers.push_back(yyer);
	}
	return 1;
}

int CStyler::delYYer(int num/* =1 */)
{
	for (int i=0; i<num; i++)
	{
		m_yyers.pop_back();
	}
	return 1;
}
