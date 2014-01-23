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
	CString m_syrPath;			// syr路径
};

extern CConfig config;
