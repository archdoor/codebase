#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

const char BASE_CODE[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";  

int base64_encode(char *lpString, char *lpBuffer, int sLen)   
{   
    register int vLen = 0;  //寄存器局部变量，提速  
    while(sLen > 0)      //处理整个字符串  
    {   *lpBuffer++ = BASE_CODE[(lpString[0] >> 2 ) & 0x3F];  //右移两位，与00111111是防止溢出，自加  
        if(sLen > 2) //够3个字符  
        {   *lpBuffer++ = BASE_CODE[((lpString[0] & 3) << 4) | (lpString[1] >> 4)];  
            *lpBuffer++ = BASE_CODE[((lpString[1] & 0xF) << 2) | (lpString[2] >> 6)];  
            *lpBuffer++ = BASE_CODE[lpString[2] & 0x3F];  
        }else  
        {   switch(sLen)    //追加“=”  
            {   case 1:  
                *lpBuffer ++ = BASE_CODE[(lpString[0] & 3) << 4 ];  
                *lpBuffer ++ = '=';  
                *lpBuffer ++ = '=';  
                break;  
                case 2:  
                *lpBuffer ++ = BASE_CODE[((lpString[0] & 3) << 4) | (lpString[1] >> 4)];  
                *lpBuffer ++ = BASE_CODE[((lpString[1] & 0x0F) << 2) | (lpString[2] >> 6)];  
                *lpBuffer ++ = '=';  
                break;  
            }  
        }  
        lpString += 3;  
        sLen -= 3;  
        vLen +=4;  
    }  
    *lpBuffer = 0;  
    return vLen;  
}  

inline char GetCharIndex(char c)
{   
    if((c >= 'A') && (c <= 'Z'))  
    {   return c - 'A';  
    }else if((c >= 'a') && (c <= 'z'))  
    {   return c - 'a' + 26;  
    }else if((c >= '0') && (c <= '9'))  
    {   return c - '0' + 52;  
    }else if(c == '+')  
    {   return 62;  
    }else if(c == '/')  
    {   return 63;  
    }else if(c == '=')  
    {   return 0;  
    }  
    return 0;  
}  

//解码，参数：结果，密文，密文长度  
int fnBase64Decode(char *lpString, char *lpSrc, int sLen)   //解码函数  
{   static char lpCode[4];  
    register int vLen = 0;  
    if(sLen % 4)        //Base64编码长度必定是4的倍数，包括'='  
    {   lpString[0] = '\0';  
        return -1;  
    }  
    while(sLen > 2)      //不足三个字符，忽略  
    {   lpCode[0] = GetCharIndex(lpSrc[0]);  
        lpCode[1] = GetCharIndex(lpSrc[1]);  
        lpCode[2] = GetCharIndex(lpSrc[2]);  
        lpCode[3] = GetCharIndex(lpSrc[3]);  

        *lpString++ = (lpCode[0] << 2) | (lpCode[1] >> 4);  
        *lpString++ = (lpCode[1] << 4) | (lpCode[2] >> 2);  
        *lpString++ = (lpCode[2] << 6) | (lpCode[3]);  

        lpSrc += 4;  
        sLen -= 4;  
        vLen += 3;  
    }  
    return vLen;  
}  

int main()
{   
    char s[] = "dd8fc1fe0a78a70d98772f3a0163ba6c55b94376"; 
    char str[64] = {0};
    int l = strlen(s);  
    int i = base64_encode(s, str, l);  
    cout << str << endl;
    l = fnBase64Decode(s, str, i);  
    cout << s << endl;

    return 0;
}  
