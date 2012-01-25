#ifndef LRC_H_
#define LRC_H_

#include <vector>
using namespace std;


#define OK         0
#define NO_MEM     1
#define FILE_ERROR 2
#define CODE_ERROR 3
#define NO_FILE    4
#define MD_FILE    5

// extern WCHAR cfg_lrc[];
// extern UINT8* gb2uc_map;
// extern BOOL bPatch;
#define MAX_CACHE_SIZE 1024

#define isSkip(u) ((u)==0x000A || (u)==0x000D || (u)==0x0020)  // is or not need skip
#define isBLetter(u) ((u)>64 && (u)<91)  // Big Letter
#define isLLetter(u) ((u)>97 && (u)<122)  // Little Letter
#define isNumber(u)  ((u)>47 && (u)<58)   // Number
#define isLetter(u) (isBLetter(u) || isLLetter(u))

// lrc time info
struct LRC_TIME_T
{
    int     time;       // �����ʵ�ʱ��(ms)
    int     index;
};

// lrc text info
struct LRC_TEXT_T
{
	WCHAR   *txt;
	UINT16   size;
};

// lrc file info
struct LRC_INFO_T
{
	WCHAR       *title;    //����
	WCHAR       *artist;   //������
	WCHAR       *album;    //��Ƭ
	WCHAR       *by;       //�������
	int          offset;   //���ʱ��ƫ��
	
	int          num_t;    //���ʱ����
	int          num_s;    //��ʾ���
	
	LRC_TIME_T  *tlrc;     //���ʱ����Ϣ
	LRC_TEXT_T  *slrc;     //�������
};


int GetCodeType(UINT8* str, int len);
int utf8_7bits_8bits(UINT8* str, int len);
int GBK2UBE(WCHAR* u_buf, UINT8* g_buf, int len);
int UTF8ToUBE(WCHAR* u_buf, UINT8* u8_buf, int len);
int ULEToUBE(WCHAR* ube_buf, UINT8* ule_buf, int len);
int UBEToUBE(WCHAR* ube_buf, UINT8* buf, int len);

int LRC_SortByTime(LRC_TIME_T* tlrc, int num);

void DESTROY_LRC(LRC_INFO_T** lrc);
LRC_INFO_T* INIT_LRC(void);

LRC_INFO_T* GetLRCFromBuf(UINT8* buf, int size, int* iRet);
int GetTXTFromBuf(UINT8* buf, int size, vector<CString>& txt);

LRC_INFO_T* GetLRCFile(TCHAR* filename, int* iRet);

int GetTXTFile(TCHAR* filename, vector<CString>& txt);

#endif /*LRC_H_*/
