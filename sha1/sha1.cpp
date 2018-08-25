#include <iostream>
#include <stdio.h>
#include <arpa/inet.h>
#include <bitset>
#include <string.h>
#include "zlog_log.h"

using namespace std;

unsigned int circular_shift(int n, unsigned int num)
{
    unsigned int mask = (0xFFFFFFFF << n) ^ (0xFFFFFFFF);
    return (num << n) | ((num >> (32 - n)) & mask);
}


long htonll(long num)
{
    int a = num & 0xFFFFFFFF;
    int b = num >> 32 & 0xFFFFFFFF;

    num = htonl(a);
    num <<= 32;
    num += htonl(b);

    return num;
}

int mysha1(unsigned char *str, unsigned char output[20])
{
    int raw_len = strlen((char *)str);
    HLogDebug(str, raw_len);
    
    int bytenum = raw_len + 1;
    int zore_bytenum = 56 < (bytenum % 64) ? (bytenum % 64) : (56 - (bytenum % 64));
    int length = raw_len + 1 + zore_bytenum;

    unsigned char buff[length + 8] = {0};
    memcpy(buff, str, raw_len);
    buff[raw_len] = 0x80;

    long len = raw_len * 8;
    len = htonll(len);
    memcpy(buff + length, &len, 8);

    HLogDebug(buff, length + 8);

    unsigned int kvalue[] = { 0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6 };

    unsigned int W[80] = {0};
    for ( int t = 0; t < 16; ++t )
    {
        unsigned int *word = (unsigned int *)(buff + 4 * t);
        W[t] = ntohl(*word);
        // printf("%d:%02x\n", t+1, W[t]);
    }
    for ( int t = 16; t < 80; ++t )
    {
        unsigned int word = W[t-3]^W[t-8]^W[t-14]^W[t-16];
        W[t] = circular_shift(1, word);
    }

    unsigned int A = 0x67452301;
    unsigned int B = 0xEFCDAB89;
    unsigned int C = 0x98BADCFE;
    unsigned int D = 0x10325476;
    unsigned int E = 0xC3D2E1F0;

    unsigned int temp;
    for ( int t = 0; t < 80; t++ )
    {
        switch ( t / 20 )
        {
            case 0: temp = ((B & C) | ((~B) & D)); break;
            case 1: temp = (B ^ C ^ D); break;
            case 2: temp = ((B & C) | (B & D) | (C & D)); break;
            case 3: temp = (B ^ C ^ D); break;
        }
        temp += circular_shift(5,A) + E + W[t] + kvalue[t/20];
        E = D;
        D = C;
        C = circular_shift(30,B);
        B = A;
        A = temp;
    }

    unsigned int MessageDigest[5] = {0};
    MessageDigest[0] = 0x67452301 + A;
    MessageDigest[1] = 0xEFCDAB89 + B;
    MessageDigest[2] = 0x98BADCFE + C;
    MessageDigest[3] = 0x10325476 + D;
    MessageDigest[4] = 0xC3D2E1F0 + E;

    printf("%02x\n", MessageDigest[0]);
    printf("%02x\n", MessageDigest[1]);
    printf("%02x\n", MessageDigest[2]);
    printf("%02x\n", MessageDigest[3]);
    printf("%02x\n", MessageDigest[4]);

    for ( int i = 0; i < 20; ++i)
    {
        output[i] = MessageDigest[i/4] >> 8 * ( 3 - ( i & 0x03 ) );
    }

    return 0;
}

int main()
{
    //日志设置
    if ( LogInit("server", "../config/zlog.conf") < 0 )
    {
        printf("log init error!\n");
        return -1;
    }

    unsigned char input[90] = "sN9cRrP/n9NdMgdcy2VJFQ==";
    unsigned char output[21] = {0};

    mysha1(input, output);

    for(int i = 0; i < 20 ; ++i)
    {
        printf("%02x", output[i]);
    }
    printf("\n");

    return 0;
}
