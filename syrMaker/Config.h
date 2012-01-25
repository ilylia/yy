#pragma once

class CConfig
{
public:
	CConfig(void);
	~CConfig(void);

	bool readConfig();					// 初始化设置，前后缀，过滤关键词
	bool saveConfig();					// 初始化设置，前后缀，过滤关键词

public:
	BOOL m_bEdit;				// 
	int m_iYYType;				// yy句柄类型，0:YY主界面  1:YY设置界面
	BOOL m_bUseSBox;			// 是否使用沙盘
	CString m_strSBoxName;		// 沙盘名
	CString m_syrPath;			// syr路径
};

extern CConfig config;
