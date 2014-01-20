#include "stdafx.h"
#include "lrc.h"


int GetCodeType(UINT8* str, int len) 
{
	if (!str || (str[0] == 0 && str[1] == 0))
		return 0;
	
	if (str[0] == 0xFF && str[1] == 0xFE)
		return 2; //unicode le

	else if (str[0] == 0xFE && str[1] == 0xFF)
		return 3; //unicode be

	else if (str[0] == 0xEF && str[1] == 0xBB && str[2] == 0xBF)
		return 4; //utf8

	else
		return utf8_7bits_8bits(str, len); // other
}

// Returned value :
// 4 : utf8
// 0 : 7bits
// 1 : 8bits  ansi gb
int utf8_7bits_8bits(UINT8* str, int len)
{
	int rv = 1;
	int ASCII7only = 1;
	UINT8 *sx	= (UINT8 *)str;
	UINT8 *endx	= sx + len;

	while (sx<endx)
	{
		if (!*sx)
		{											// For detection, we'll say that NUL means not UTF8
			ASCII7only = 0;
			rv = 0;
			break;
		} 
		else if (*sx < 0x80)
		{			// 0nnnnnnn If the byte's first hex code begins with 0-7, it is an ASCII character.
			sx++;
		} 
		else if (*sx < (0x80 + 0x40)) 
		{											  // 10nnnnnn 8 through B cannot be first hex codes
			ASCII7only=0;
			rv=0;
			break;
		} 
		else if (*sx < (0x80 + 0x40 + 0x20))
		{					  // 110xxxvv 10nnnnnn  If it begins with C or D, it is an 11 bit character
			ASCII7only=0;
			if (sx>=endx-1) 
				break;
			if (!(*sx & 0x1F) || (sx[1]&(0x80+0x40)) != 0x80) {
				rv=0; break;
			}
			sx+=2;
		} 
		else if (*sx < (0x80 + 0x40 + 0x20 + 0x10))
		{								// 1110qqqq 10xxxxvv 10nnnnnn If it begins with E, it is 16 bit
			ASCII7only=0;
			if (sx>=endx-2) 
				break;
			if (!(*sx & 0xF) || (sx[1]&(0x80+0x40)) != 0x80 || (sx[2]&(0x80+0x40)) != 0x80) {
				rv=0; break;
			}
			sx+=3;
		} 
		else 
		{													  // more than 16 bits are not allowed here
			ASCII7only=0;
			rv=0;
			break;
		}
	}

	if (ASCII7only) 
		return 0;
	if (rv)
		return 4;
	return 1;
}

int GBK2UBE(WCHAR* u_buf, UINT8* g_buf, int len)
{
// 	UINT16 i=0;
// 	UINT16 k=0;      // num of unicode
// 	
// 	UINT16 tmp=0;    // offset in map
// 	UINT8  temp[2];
// 
// 	u_buf[k++] = 0xFEFF;
// 	while (i < len)
// 	{
// 		if (g_buf[i]<0x81)
// 		{
// 			if (g_buf[i]<0x20 && g_buf[i]!=0x0D && g_buf[i]!=0x0A)
// 			{
// 				i++;
// 				continue;
// 			}
// 			else
// 			{
// 				u_buf[k++] = g_buf[i++];
// 			}
// 		}
// 		else
// 		{
// 			tmp = g_buf[i] - 0x81;
// 			tmp = tmp * 0xBF + g_buf[i+1] - 0x40;
// 			tmp = tmp << 1;
// 			if (tmp < 0xBC04)
// 			{
// 				temp[0] = *(UINT8*)(gb2uc_map+tmp);
// 				temp[1] = *(UINT8*)(gb2uc_map+tmp+1);
// 
// 				u_buf[k++] = (temp[1] << 8) | temp[0];
// 				i += 2;
// 			}
// 			else
// 			{
// 				i++;
// 				continue;
// 			}
// 		}
// 	}
// 	return k;

	u_buf[0] = 0xFEFF;
	int wcsLen = ::MultiByteToWideChar(936, 0, (LPCSTR)g_buf, len, NULL, 0);	// GBK
	::MultiByteToWideChar(936, 0, (LPCSTR)g_buf, len, u_buf+1, wcsLen);
	u_buf[wcsLen+1] = '\0';

	return wcsLen+1;
}

int UTF8ToUBE(WCHAR* u_buf, UINT8* u8_buf, int len)
{
	int i=0;
	int k=0;

	while (i < len)
	{
	    if ((u8_buf[i] >> 7) == 0x00)  //µ¥×Ö½Ú±àÂë 0xxxxxxx
		{
	    	u_buf[k++]=u8_buf[i++];
		}
	    else if ((u8_buf[i] >> 5) == 0x06) //Ë«×Ö½Ú±àÂë 110xxxxx 10xxxxxx
		{
	    	u_buf[k++] = ((u8_buf[i] & 0x1F) << 6) | (u8_buf[i+1] & 0x3F);
			i += 2;
		}
	    else if ((u8_buf[i] >> 4) == 0xe)  //Èý×Ö½Ú±àÂë 1110xxxx 10xxxxxx 10xxxxxx
	   	{
	    	u_buf[k++] = ((u8_buf[i] & 0x0F)<<12) | ((u8_buf[i+1] & 0x3F)<<6) | (u8_buf[i+2] & 0x3F);
	    	i += 3;
	    }
	    else
	    {
	    	i++;
	    	continue;
	    }
	}
	return k;

// 	u_buf[0] = 0xFEFF;
// 	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, (LPCSTR)u_buf, len, NULL, 0);	// UTF-8
// 	::MultiByteToWideChar(CP_UTF8, NULL, (LPCSTR)u_buf, len, u_buf+1, wcsLen);
// 	u_buf[wcsLen+1] = '\0';
// 
// 	return wcsLen+1;
}

int ULEToUBE(WCHAR* ube_buf, UINT8* ule_buf, int len)
{
	int i=0,j=0;
	for(i=0; i<len; i+=2)
	{
		ube_buf[j++] = (ule_buf[i+1] << 8) | ule_buf[i];
	}
	return j;
}

int UBEToUBE(WCHAR* ube_buf, UINT8* buf, int len)
{
	int i=0,j=0;
	for(i=0; i<len; i+=2)
	{
		ube_buf[j++] = (buf[i] << 8) | buf[i+1];
	}
	return j;
}

int LRC_SortByTime(LRC_TIME_T* tlrc, int num)
{
	int i,j;
	int tmp16;
	int tmp32;
	for (i=0; i<num-1; i++)
	{
		for (j=i+1; j<num; j++)
		{
			if (tlrc[i].time > tlrc[j].time)
			{
				tmp32         = tlrc[i].time;
				tlrc[i].time  = tlrc[j].time;
				tlrc[j].time  = tmp32;
				tmp16         = tlrc[i].index;
				tlrc[i].index = tlrc[j].index;
				tlrc[j].index = tmp16;
			}
		}
	}
	return OK;
}

void DESTROY_LRC(LRC_INFO_T** lrc)
{
	UINT16 i;
	
	if ((*lrc) != NULL)
	{
		if ((*lrc)->title != NULL)
		{
			free((*lrc)->title);
			(*lrc)->title = NULL;
		}
		if ((*lrc)->artist != NULL)
		{
			free((*lrc)->artist);
			(*lrc)->artist = NULL;
		}
		if ((*lrc)->album != NULL)
		{
			free((*lrc)->album);
			(*lrc)->album = NULL;
		}
		if ((*lrc)->by != NULL)
		{
			free((*lrc)->by);
			(*lrc)->by = NULL;
		}
		if ((*lrc)->tlrc != NULL)
		{
			free((*lrc)->tlrc);
			(*lrc)->tlrc = NULL;
		}
		if ((*lrc)->slrc != NULL)
		{
			for (i=0; i<(*lrc)->num_s; i++)
			{
				if ((*lrc)->slrc[i].txt != NULL)
				{
					free((*lrc)->slrc[i].txt);
					(*lrc)->slrc[i].txt = NULL;
				}
			}
			free((*lrc)->slrc);
			(*lrc)->slrc = NULL;
		}
		free((*lrc));
		(*lrc) = NULL;
	}
}

LRC_INFO_T* INIT_LRC()
{
	LRC_INFO_T* lrc = NULL;
	lrc = (LRC_INFO_T*)malloc(sizeof(LRC_INFO_T));
	if (lrc == NULL)
	{
		DESTROY_LRC(&lrc);
		return NULL;
	}
	lrc->title = NULL;
	lrc->title = (WCHAR*)malloc(sizeof(WCHAR));
	if (lrc->title == NULL)
	{
		DESTROY_LRC(&lrc);
		return NULL;
	}
	lrc->title[0] = 0;
	lrc->artist = NULL;
	lrc->artist = (WCHAR*)malloc(sizeof(WCHAR));
	if (lrc->artist == NULL)
	{
		DESTROY_LRC(&lrc);
		return NULL;
	}
	lrc->artist[0] = 0;
	lrc->album = NULL;
	lrc->album = (WCHAR*)malloc(sizeof(WCHAR));
	if (lrc->album == NULL)
	{
		DESTROY_LRC(&lrc);
		return NULL;
	}
	lrc->album[0] = 0;
	lrc->by = NULL;
	lrc->by = (WCHAR*)malloc(sizeof(WCHAR));
	if (lrc->by == NULL)
	{
		DESTROY_LRC(&lrc);
		return NULL;
	}
	lrc->by[0] = 0;
	lrc->offset = 0;
	lrc->num_t = 0;
	lrc->num_s = 0;
	lrc->tlrc = NULL;
	lrc->slrc = NULL;

	return lrc;
}

LRC_INFO_T* GetLRCFromBuf(UINT8* buf, int size, int*iRet)
{
	LRC_INFO_T     *lrc = NULL;
	WCHAR          *u_buf = NULL;
	int             lenth = 0;
	int             i,j;
	int             temp;
	LRC_TIME_T      lrc_time[MAX_CACHE_SIZE];
	int             n_time = 0;
	int             lrc_txt_off[MAX_CACHE_SIZE];
	int             lrc_txt_len[MAX_CACHE_SIZE];
	int             n_txt  = 0;
	int             time_offset_flag = 1;
	
	u_buf = (WCHAR*)malloc((size+1)*sizeof(WCHAR));
	if (u_buf == NULL)
	{
		*iRet = NO_MEM;
		return NULL;
	}

	i = 1;
	
	switch(GetCodeType(buf, size))
	{
	case 1:
		lenth = GBK2UBE(u_buf, buf, size);
		break;
	case 2:
		lenth = ULEToUBE(u_buf, buf, size);
		break;
	case 3:
		lenth = UBEToUBE(u_buf, buf, size);
		break;
	case 4:
		lenth = UTF8ToUBE(u_buf, buf, size);
		break;
	case 0:
	default:
		*iRet = CODE_ERROR;
		return NULL;
	}
	if (lenth < 1)
	{
		*iRet = FILE_ERROR;
		return NULL;
	}
	
	if (u_buf[lenth-1]!=0x000D && u_buf[lenth-1]!=0x000A)
	{
		u_buf[lenth] = 0x000A;
	}
	
//	INIT_LRC(&lrc);
	lrc = INIT_LRC();

	while(i < lenth && n_time < MAX_CACHE_SIZE)
	{
		if (u_buf[i]==0x005B)
		{
			// lrc time9
			if (isNumber(u_buf[i+1]))
			{
				while(u_buf[i]==0x005B)
				{
					if(isNumber(u_buf[i+1]))	// [
					{
						i++;
						temp = 0;
						while(isNumber(u_buf[i]))
						{
							temp *= 10;
							temp += u_buf[i++]-48;
						}
						lrc_time[n_time].time = temp*60000;
						if(u_buf[i]==0x003A)
							i++;
						else
						{
							DESTROY_LRC(&lrc);
							*iRet = FILE_ERROR;
							return NULL;
						}
						temp = 0;
						while(isNumber(u_buf[i]))
						{
							temp *= 10;
							temp += u_buf[i++]-48;
						}
						lrc_time[n_time].time += temp*1000;
						if(u_buf[i]==0x002E)
						{
							i++;
							if (isNumber(u_buf[i]))
								lrc_time[n_time].time += (u_buf[i++]-0x0030)*100;
							if (isNumber(u_buf[i]))
								lrc_time[n_time].time += (u_buf[i++]-0x0030)*10;
							if (isNumber(u_buf[i]))
								lrc_time[n_time].time += u_buf[i++]-0x0030;
						}
						lrc_time[n_time].index = n_txt;
						n_time++;
						i++;
					}
					else
						break;
				}
				
				// lrc txt
				lrc_txt_off[n_txt] = i;
				while(u_buf[i]!=0x000D && u_buf[i]!=0x000A)
					i++;
				lrc_txt_len[n_txt] = i - lrc_txt_off[n_txt];
				n_txt++;
		
				i++;
				while ( isSkip(u_buf[i]) )
					i++;
			}
			
			// song title
//			if (u_buf[i]==0x0074 && u_buf[i+1]==0x0069 && u_buf[i+2]==0x003A)	// [ti:
			else if (wcsncmp(&u_buf[i], L"[ti:", 4)==0)
			{
				i += 4;
				j = 0;
				if (u_buf[i]==0x005D)
				{
					i++;
					j = i;
					while(u_buf[i]!=0xD && u_buf[i]!=0xA)
						i++;
					j = i - j;
				}
				else
				{
					j = i;
					while(u_buf[i]!=0x005D)
						i++;
					j = i - j;
				}
				if (lrc->title != NULL)
				{
					free(lrc->title);
					lrc->title = NULL;
				}
				lrc->title = (WCHAR*)malloc((j+1) * sizeof(WCHAR));
				if (lrc->title == NULL)
				{
					DESTROY_LRC(&lrc);
					*iRet = NO_MEM;
					return NULL;
				}
				memcpy(lrc->title, &u_buf[i-j], j * sizeof(WCHAR));
				lrc->title[j] = 0;
				while ( isSkip(u_buf[i]) )
					i++;
			}
			// song artist
//			if (u_buf[i]==0x0061 && u_buf[i+1]==0x0072 && u_buf[i+2]==0x003A)	// [ar:
			else if (wcsncmp(&u_buf[i], L"[ar:", 4)==0)
			{
				i += 4;
				j = 0;
				if (u_buf[i]==0x005D)
				{
					i++;
					j = i;
					while(u_buf[i]!=0xD && u_buf[i]!=0xA)
						i++;
					j = i - j;
				}
				else
				{
					j = i;
					while(u_buf[i]!=0x005D)
						i++;
					j = i - j;
				}
				if (lrc->artist != NULL)
				{
					free(lrc->artist);
					lrc->artist = NULL;
				}
				lrc->artist = (WCHAR*)malloc((j+1) * sizeof(WCHAR));
				if (lrc->artist == NULL)
				{
					DESTROY_LRC(&lrc);
					*iRet = NO_MEM;
					return NULL;
				}
				memcpy(lrc->artist, &u_buf[i-j], j * sizeof(WCHAR));
				lrc->artist[j] = 0;
				while ( isSkip(u_buf[i]) )
					i++;
			}
			// song album
//			if (u_buf[i]==0x0061 && u_buf[i+1]==0x006C && u_buf[i+2]==0x003A)	// [al:
			else if (wcsncmp(&u_buf[i], L"[al:", 4)==0)
			{
				i += 4;
				j = 0;
				if (u_buf[i]==0x005D)
				{
					i++;
					j = i;
					while(u_buf[i]!=0xD && u_buf[i]!=0xA)
						i++;
					j = i - j;
				}
				else
				{
					j = i;
					while(u_buf[i]!=0x005D)
						i++;
					j = i - j;
				}
				if (lrc->album != NULL)
				{
					free(lrc->album);
					lrc->album = NULL;
				}
				lrc->album = (WCHAR*)malloc((j+1) * sizeof(WCHAR));
				if (lrc->album == NULL)
				{
					DESTROY_LRC(&lrc);
					*iRet = NO_MEM;
					return NULL;
				}
				memcpy(lrc->album, &u_buf[i-j], j * sizeof(WCHAR));
				lrc->album[j] = 0;
				while ( isSkip(u_buf[i]) )
					i++;
			}
			// lrc by
//			if (u_buf[i]==0x0062 && u_buf[i+1]==0x0079 && u_buf[i+2]==0x003A)	// [by:
			else if (wcsncmp(&u_buf[i], L"[by:", 4)==0)
			{
				i += 4;
				j = 0;
				if (u_buf[i]==0x005D)
				{
					i++;
					j = i;
					while(u_buf[i]!=0xD && u_buf[i]!=0xA)
						i++;
					j = i - j;
				}
				else
				{
					j = i;
					while(u_buf[i]!=0x005D)
						i++;
					j = i - j;
				}
				if (lrc->by != NULL)
				{
					free(lrc->by);
					lrc->by = NULL;
				}
				lrc->by = (WCHAR*)malloc((j+1) * sizeof(WCHAR));
				if (lrc->by == NULL)
				{
					DESTROY_LRC(&lrc);
					*iRet = NO_MEM;
					return NULL;
				}
				memcpy(lrc->by, &u_buf[i-j], j * sizeof(WCHAR));
				lrc->by[j] = 0;
				while ( isSkip(u_buf[i]) )
					i++;
			}
			// lrc offset
//			if (u_buf[i]==0x006F && u_buf[i+1]==0x0066 && u_buf[i+2]==0x0066
//				&& u_buf[i+3]==0x0073 && u_buf[i+4]==0x0065 && u_buf[i+5]==0x0074 && u_buf[i+6]==0x003A)
			else if (wcsncmp(&u_buf[i], L"[offset:", 8)==0)
			{
				i += 8;
				if (u_buf[i]==0x002D) // -
				{
					time_offset_flag = -1;
					i++;
				}
				lrc->offset = 0;
				while (u_buf[i]!=0x005D)
				{
					if (u_buf[i]>=0x0030 && u_buf[i]<=0x0039)
					{
						lrc->offset *= 10;
						lrc->offset += u_buf[i++]-0x0030;
					}
				}
				lrc->offset	*= time_offset_flag;
				
				i++;
				while ( isSkip(u_buf[i]) )
					i++;
			}
			// lrc key
//			else if (u_buf[i+1]==0x006B && u_buf[i+1]==0x0065 && u_buf[i+1]==0x0079 && u_buf[i+1]==0x003A)
			else if (wcsncmp(&u_buf[i], L"[key:", 5)==0)
			{
				DESTROY_LRC(&lrc);
				*iRet = MD_FILE;
				return NULL;
			}
			else
			{
				i++;
				while(u_buf[i]!=0x005B && i<lenth && n_time < MAX_CACHE_SIZE)
					i++;
			}
		}
		else
			while(u_buf[i]!=0x005B && i<lenth && n_time < MAX_CACHE_SIZE)
				i++;
	}
	
	lrc->num_t = n_time;
	lrc->num_s = n_txt;
	lrc->tlrc = NULL;
	lrc->tlrc = (LRC_TIME_T*)malloc((lrc->num_t+1) * sizeof(LRC_TIME_T));
	if (lrc->tlrc == NULL)
	{
		DESTROY_LRC(&lrc);
		*iRet = NO_MEM;
		return NULL;
	}
	memcpy(lrc->tlrc, lrc_time, lrc->num_t*sizeof(LRC_TIME_T));
	lrc->tlrc[lrc->num_t].time  = 0x7FFFFFFF;
	lrc->tlrc[lrc->num_t].index = 0;

	lrc->slrc = NULL;
	lrc->slrc = (LRC_TEXT_T*)malloc(lrc->num_s * sizeof(LRC_TEXT_T));
	if (lrc->slrc == NULL)
	{
		DESTROY_LRC(&lrc);
		*iRet = NO_MEM;
		return NULL;
	}
	for (i=0; i<lrc->num_s; i++)
	{
		lrc->slrc[i].txt = NULL;
		lrc->slrc[i].txt = (WCHAR*)malloc((lrc_txt_len[i]+1) * sizeof(WCHAR));
		if (lrc->slrc[i].txt == NULL)
		{
			DESTROY_LRC(&lrc);
			*iRet = NO_MEM;
			return NULL;
		}
		lrc->slrc[i].size = lrc_txt_len[i];
		memcpy(lrc->slrc[i].txt, &u_buf[lrc_txt_off[i]], lrc->slrc[i].size * sizeof(WCHAR));
		lrc->slrc[i].txt[lrc->slrc[i].size] = 0;
	}
	
	LRC_SortByTime(lrc->tlrc, lrc->num_t);
	
	free(u_buf);
	u_buf = NULL;
	
	*iRet = 0;
	return lrc;
}

int GetTXTFromBuf(UINT8* buf, int size, vector<CString>& txt)
{
	WCHAR    *u_buf = NULL;
	int       lenth = 0;
	int       i=0, j=0;
	int       n_time = 0;
	int       n_txt  = 0;
	int       time_offset_flag = 1;

	u_buf = (WCHAR*)malloc((size+1)*sizeof(WCHAR));
	if (u_buf == NULL)
	{
		return NO_MEM;
	}

	switch(GetCodeType(buf, size))
	{
	case 1:
		lenth = GBK2UBE(u_buf, buf, size);
		break;
	case 2:
		lenth = ULEToUBE(u_buf, buf, size);
		break;
	case 3:
		lenth = UBEToUBE(u_buf, buf, size);
		break;
	case 4:
		lenth = UTF8ToUBE(u_buf, buf, size);
		break;
	case 0:
	default:
		return CODE_ERROR;
	}
	if (lenth < 1)
	{
		return FILE_ERROR;
	}

	if (u_buf[lenth-1]!=0x000D && u_buf[lenth-1]!=0x000A)
	{
		u_buf[lenth] = 0x000A;
	}

	while(i < lenth)
	{
		// txt
		while(u_buf[i]!=0x000D && u_buf[i]!=0x000A)
			i++;
		WCHAR* tmp = new WCHAR[i - j + 2];
		memcpy(tmp, u_buf+j, (i-j+1)*sizeof(WCHAR));
		tmp[i-j+1] = 0;
		CString temp(tmp);
		delete [] tmp;
		txt.push_back(temp);

		i++;
		while ( isSkip(u_buf[i]) )
			i++;

		j = i;
	}

	free(u_buf);
	u_buf = NULL;

	return 0;
}

LRC_INFO_T* GetLRCFile(TCHAR* filepath, int* iRet)
{
	LRC_INFO_T     *lrc  = NULL;
	UINT8          *bufR = NULL;
	FILE*           f = NULL;
	int             fSize = 0;
	

	f = _tfopen(filepath, _T("rb"));
	if (f == NULL)
	{
		*iRet = FILE_ERROR;
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	fSize = ftell(f);
	fseek(f, 0, SEEK_SET);

	bufR = (UINT8*)malloc( fSize * sizeof(UINT8) );
	if (bufR == NULL)
	{
		*iRet = NO_MEM;
		return NULL;
	}
	memset(bufR, 0, fSize);
	fread( bufR, fSize, 1, f );
	fclose(f);
	
	lrc = GetLRCFromBuf(bufR, fSize, iRet);
	free(bufR);
	
	return lrc;
}

int GetTXTFile(TCHAR* filename, vector<CString>& txt)
{
	UINT8  *bufR = NULL;
	FILE   *f = NULL;
	int     fSize = 0;

	f = _tfopen(filename, _T("rb"));
	if (f == NULL)
	{
		return FILE_ERROR;
	}

	fseek(f, 0, SEEK_END);
	fSize = ftell(f);
	fseek(f, 0, SEEK_SET);

	bufR = (UINT8*)malloc( fSize * sizeof(UINT8) );
	if (bufR == NULL)
	{
		return NO_MEM;
	}
	memset(bufR, 0, fSize);
	fread( bufR, fSize, 1, f );
	fclose(f);

	int iRet = GetTXTFromBuf(bufR, fSize, txt);
	free(bufR);

	return 0;
}
