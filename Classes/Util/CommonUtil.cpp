#include "Util/CommonUtil.h"
#include "cocos2d.h"

USING_NS_CC;

float* GetDoubleArrayJSVal(const char* field, JSObject* defObj, JSContext* cx, const int size)
{
	jsval vp = JSVAL_VOID;

	JS_GetProperty(cx,defObj,field,&vp);
	if(!JSVAL_IS_VOID(vp))
	{
		JSObject* arrObj = JSVAL_TO_OBJECT(vp); 

		float* fArr = new float[size];

		for(int i = 0;;i++)
		{
			JS_GetElement(cx,arrObj,i,&vp);
			if(!JSVAL_IS_VOID(vp))
			{ 
				if(JSVAL_IS_DOUBLE(vp))
				{
					fArr[i] = (float)(JSVAL_TO_DOUBLE(vp));
				}
				else
				{
					fArr[i] = (float)(JSVAL_TO_INT(vp));
				}
			}
			else
			{
				break;
			}
				
		}

		return fArr;
	}

	return NULL;
}

std::string& GetStrJSVal(const char* field, JSObject* defObj, JSContext* cx)
{
	jsval vp = JSVAL_VOID;

	std::string str = NULL;

	JS_GetProperty(cx,defObj,field,&vp);

	if(!JSVAL_IS_VOID(vp))
	{
		JSString* jsstr;
		if(JSVAL_IS_STRING(vp))
		{
			jsstr=JSVAL_TO_STRING(vp);
		}
		else
		{
			jsstr=JS_ValueToString(cx,vp);
		}

		size_t len;

		const jschar* p = JS_GetStringCharsAndLength(cx,jsstr,&len);

		WCharArrToString((unsigned short*)p,str);
	}

	return str;
}

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

float SpeedLogicToScreen(float logicX,float refX)
{
	float result;
	
	result = logicX - refX;

	return result;
}