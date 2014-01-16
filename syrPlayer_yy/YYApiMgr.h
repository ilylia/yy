#pragma once

class CYYApiMgr
{
private:
	CYYApiMgr(void);
	~CYYApiMgr(void);

public:
	static CYYApiMgr* GetInstance();
	static void DelInstance();

public:
	bool Init();
	void Exit();

public:
	CString GetUserNick();
	BOOL Rename(CString nick);
};
