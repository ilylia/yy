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
	int m_iYYType;				// yy������ͣ�0:YY������  1:YY���ý���
	BOOL m_bUseSBox;			// �Ƿ�ʹ��ɳ��
	CString m_strSBoxName;		// ɳ����
	CString m_syrPath;			// syr·��
};

extern CConfig config;
