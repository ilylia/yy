
#include "stdafx.h"
#include "Config.h"
#include "tinyxml/tinyxml.h"
#include "Styler.h"

#define CONFIG_FILE "syrmaker.xml"

CConfig config;

CConfig::CConfig(void)
{
	m_bEdit = FALSE;
	readConfig();
}

CConfig::~CConfig(void)
{
	if (m_bEdit)
		saveConfig();
}

bool CConfig::readConfig()
{
	// ½âÎöxml
	USES_CONVERSION;

	TiXmlDocument doc;
	if(!doc.LoadFile(CONFIG_FILE))
	{
		return false;
	}

	TiXmlElement* root = doc.FirstChildElement("Config");
	if (root == NULL)
	{
		return false;
	}

	TiXmlElement* syrPathElem = root->FirstChildElement("SyrPath");
	if (syrPathElem != NULL)
	{
		m_syrPath = UTF8ToUnicode(syrPathElem->Attribute("value"));
	}

//	TiXmlElement* keyElem = root->FirstChildElement("Key");
//	if (keyElem != NULL)
//	{
//		const char* puse = keyElem->Attribute("use");
//		if (puse != NULL)
//			m_bUseKey  = atoi(puse);
//
//		TiXmlElement* freshElem = keyElem->FirstChildElement("Fresh");
//		if (freshElem != NULL)
//		{
//			m_key[0].bCtrl  = atoi(freshElem->Attribute("ctrl"));
//			m_key[0].bShift = atoi(freshElem->Attribute("shift"));
//			m_key[0].bAlt   = atoi(freshElem->Attribute("alt"));
//			m_key[0].uCode  = atoi(freshElem->Attribute("code"));
//		}
//		TiXmlElement* beginElem = keyElem->FirstChildElement("Begin");
//		if (beginElem != NULL)
//		{
//			m_key[1].bCtrl  = atoi(beginElem->Attribute("ctrl"));
//			m_key[1].bShift = atoi(beginElem->Attribute("shift"));
//			m_key[1].bAlt   = atoi(beginElem->Attribute("alt"));
//			m_key[1].uCode  = atoi(beginElem->Attribute("code"));
//		}
//		TiXmlElement* stopElem = keyElem->FirstChildElement("Stop");
//		if (stopElem != NULL)
//		{
//			m_key[2].bCtrl  = atoi(stopElem->Attribute("ctrl"));
//			m_key[2].bShift = atoi(stopElem->Attribute("shift"));
//			m_key[2].bAlt   = atoi(stopElem->Attribute("alt"));
//			m_key[2].uCode  = atoi(stopElem->Attribute("code"));
//		}
//		TiXmlElement* forwardElem = keyElem->FirstChildElement("Forward");
//		if (forwardElem != NULL)
//		{
//			m_key[3].bCtrl  = atoi(forwardElem->Attribute("ctrl"));
//			m_key[3].bShift = atoi(forwardElem->Attribute("shift"));
//			m_key[3].bAlt   = atoi(forwardElem->Attribute("alt"));
//			m_key[3].uCode  = atoi(forwardElem->Attribute("code"));
//		}
//		TiXmlElement* backwardElem = keyElem->FirstChildElement("Backward");
//		if (backwardElem != NULL)
//		{
//			m_key[4].bCtrl  = atoi(backwardElem->Attribute("ctrl"));
//			m_key[4].bShift = atoi(backwardElem->Attribute("shift"));
//			m_key[4].bAlt   = atoi(backwardElem->Attribute("alt"));
//			m_key[4].uCode  = atoi(backwardElem->Attribute("code"));
//		}
//	}

	return true;
}

bool CConfig::saveConfig()
{
	USES_CONVERSION;

	TiXmlDocument doc;

	TiXmlDeclaration* declaration = new TiXmlDeclaration("1.0", "utf-8", "");
	doc.LinkEndChild(declaration);

	TiXmlElement* root = new TiXmlElement("Config");
	doc.LinkEndChild(root);

	TiXmlText* text = NULL;
	CString str;

	TiXmlElement* syrPathElem = new TiXmlElement("SyrPath");
	root->LinkEndChild(syrPathElem);
	syrPathElem->SetAttribute("value", UnicodeToUTF8(m_syrPath));

//	TiXmlElement* keyElem = new TiXmlElement("Key");
//	root->LinkEndChild(keyElem);
//	keyElem->SetAttribute("use", m_bUseKey);
//	TiXmlElement* freshElem = new TiXmlElement("Fresh");
//	keyElem->LinkEndChild(freshElem);
//	freshElem->SetAttribute("ctrl", m_key[0].bCtrl);
//	freshElem->SetAttribute("shift", m_key[0].bShift);
//	freshElem->SetAttribute("alt", m_key[0].bAlt);
//	freshElem->SetAttribute("code", m_key[0].uCode);
//	TiXmlElement* beginElem = new TiXmlElement("Begin");
//	keyElem->LinkEndChild(beginElem);
//	beginElem->SetAttribute("ctrl", m_key[1].bCtrl);
//	beginElem->SetAttribute("shift", m_key[1].bShift);
//	beginElem->SetAttribute("alt", m_key[1].bAlt);
//	beginElem->SetAttribute("code", m_key[1].uCode);
//	TiXmlElement* stopElem = new TiXmlElement("Stop");
//	keyElem->LinkEndChild(stopElem);
//	stopElem->SetAttribute("ctrl", m_key[2].bCtrl);
//	stopElem->SetAttribute("shift", m_key[2].bShift);
//	stopElem->SetAttribute("alt", m_key[2].bAlt);
//	stopElem->SetAttribute("code", m_key[2].uCode);
//	TiXmlElement* forwardElem = new TiXmlElement("Forward");
//	keyElem->LinkEndChild(forwardElem);
//	forwardElem->SetAttribute("ctrl", m_key[3].bCtrl);
//	forwardElem->SetAttribute("shift", m_key[3].bShift);
//	forwardElem->SetAttribute("alt", m_key[3].bAlt);
//	forwardElem->SetAttribute("code", m_key[3].uCode);
//	TiXmlElement* backwardElem = new TiXmlElement("Backward");
//	keyElem->LinkEndChild(backwardElem);
//	backwardElem->SetAttribute("ctrl", m_key[4].bCtrl);
//	backwardElem->SetAttribute("shift", m_key[4].bShift);
//	backwardElem->SetAttribute("alt", m_key[4].bAlt);
//	backwardElem->SetAttribute("code", m_key[4].uCode);

	return doc.SaveFile(CONFIG_FILE);
}
