#pragma once

#include "afxwin.h"
#include "afxcmn.h"

#include <vector>
using namespace std;


CStringW UTF8ToUnicode(const char* utf8);
CStringA UnicodeToUTF8(const wchar_t* uni16);

struct SLyric
{
	int state;		// 状态	0：正常  1：被选中
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
	int  idx;	// 当前播放到第几句
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
	int New(CString path);		// 新建任务工程
	int Open();					// 打开任务工程
	int Open(CString path);
	int Save();					// 保存任务工程
	int SaveAs(CString path);				// 另存为任务工程

	CString getPath();

	int getYYerCount();			// 需要几个yy
	int getYYerLrcsCount(int idx);	//
	vector<SLyric>& getLyrics(int idx);	// 获取某yy对应的lyric

	int calcYYerLrc(int iYY, int time);	// 获取iYY号当前时间的歌词  返回值0:无须改变 1:需要改变 2:需要停止 3:无时间点
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

	int addYYerTime(int idx, int time, CString str = _T(""));	// ret: 0.取消添加 1.插入 2.修改
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
	int Read();			// 读取
	int Write();		// 写入

protected:
	CString		m_path;	// 任务文件路径

private:
	CString m_title;
	CString m_artist;
	CString m_madeby;
	int m_offset;
	vector<SYYer> m_yyers;
};
