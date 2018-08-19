#include <iostream>
#include <stdio.h>
#include "pcap.h"

using namespace std;

void func(unsigned char *data, int datalen)
{
    for ( int i = 0; i < datalen; ++i )    
    {
        printf("%x", data[i]);
    }
    printf("\n");
}

int main()
{
    CPcapFile file("./Abis_gate.pcap");

    file.parsePcapFile(func);

    return 0;
}
