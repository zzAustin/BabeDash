#include "Util/CommonUtil.h"
#include "cocos2d.h"
USING_NS_CC;

void WCharArrToString(unsigned short* src, std::string& str)
{
	CCAssert(src != NULL,"CharArrToString--- src is NULL!");
	unsigned short* p = src;
	char dest[128];
	int len = GetWCharArrLen(p);
	for(int i = 0;i<len;i++)
	{
		dest[i] = src[i];
	}
	dest[len] = '\0';
	for(int i = 0;i<strlen(dest);i++)
		CCLog("%c,",dest[i]);
	str = dest;
}


int GetWCharArrLen(unsigned short* src)
{
	int len = 0,i=0; 
	char* p = (char*)src;
	while(1)
	{
		if(p[i] == '\0' && p[i-1] == '\0')
			break;
		else if(p[i] != '\0')
			len++;

		i++;
	}

	return len;
}