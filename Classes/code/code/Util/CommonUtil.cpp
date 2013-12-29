#include "Util/CommonUtil.h"

void WCharArrToString(wchar_t* src, std::string& str)
{
	CCAssert(src != NULL,"CharArrToString--- src is NULL!");
	wchar_t* p = src;
	char dest[128];
	int len = wcslen(p);
	for(int i = 0;i<len;i++)
	{
		dest[i] = src[i];
	}
	dest[len] = '\0';
	str = dest;
}