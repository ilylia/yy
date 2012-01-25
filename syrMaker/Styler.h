#pragma once

#include "afxwin.h"
#include "afxcmn.h"

#include <vector>
using namespace std;


CStringW UTF8ToUnicode(const char* utf8);
CStringA UnicodeToUTF8(const wchar_t* uni16);

struct SLyric
{
	int state;		// ״̬	0������  1����ѡ��
	int time;
	CString str;

	SLyric()
	{
		state = 0;
		time = 0;
		str = _T("");
	}
};

struct SYYer
{
	int  idx;	// ��ǰ���ŵ��ڼ���
	vector<SLyric> lrcs;

	SYYer()
	{
		idx = -1;
	}
};

class CStyler
{
public:
public:
	CStyler(void);
	~CStyler(void);

public:
	int New();
	int New(CString path);		// �½����񹤳�
	int Open();					// �����񹤳�
	int Open(CString path);
	int Save();					// �������񹤳�
	int SaveAs(CString path);				// ���Ϊ���񹤳�

	CString getPath();

	int getYYerCount();			// ��Ҫ����yy
	int getYYerLrcsCount(int idx);	//
	vector<SLyric>& getLyrics(int idx);	// ��ȡĳyy��Ӧ��lyric

	int calcYYerLrc(int iYY, int time);	// ��ȡiYY�ŵ�ǰʱ��ĸ��  ����ֵ0:����ı� 1:��Ҫ�ı� 2:��Ҫֹͣ 3:��ʱ���
	int getYYerLrcTime(int idxYY, int idxLrc);
	int getYYerLrcState(int idxYY, int idxLrc);
	int getYYerLrcIdx(int idxYY, int time);
	int getYYerCurIdx(int iYY);
	CString getYYerCurLrc(int iYY);
	int getYYerCurTime(int iYY);
	int getMaxTime();
	int getNextTimePoint(int time);
	int getPrevTimePoint(int time);

	int getOffset();
	void reset();

	int addYYerTime(int idx, int time, CString str = _T(""));	// ret: 0.ȡ����� 1.���� 2.�޸�
	int addYYersTime(int time);
	int editYYersTime(int oldtime, int newtime);
	int editTimeLrc(int idxYY, int idxLrc, int time);
	int editTimeLrc(int idxYY, int idxLrc, CString lrc);
	int editTimeLrc(int idxYY, int idxLrc, int time, CString lrc);
	int setTimeStateByIdx(int idxYY, int idxLrc, int state);
	int setTimeState(int idxYY, int time, int state);
	int setTimesState(int state);
	int delYYerTime(int idxYY, int idxLrc);
	int delYYerTimes();
	int copySelTimesTo(int timeTo);
	int copySelTimesOffset(int timeOffset);

	int addYYer(int num=1);
	int delYYer(int index);

protected:
	int Free();
	int Read();			// ��ȡ
	int Write();		// д��

protected:
	CString		m_path;	// �����ļ�·��

private:
	CString m_title;
	CString m_artist;
	CString m_madeby;
	int m_offset;
	vector<SYYer> m_yyers;
};
