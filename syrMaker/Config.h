#pragma once

class CConfig
{
public:
	CConfig(void);
	~CConfig(void);

	bool readConfig();					// ��ʼ�����ã�ǰ��׺�����˹ؼ���
	bool saveConfig();					// ��ʼ�����ã�ǰ��׺�����˹ؼ���

public:
	BOOL m_bEdit;				// 
	CString m_syrPath;			// syr·��
};

extern CConfig config;
