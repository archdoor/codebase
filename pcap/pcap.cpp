#include <stdio.h>
#include <arpa/inet.h>
#include "pcap.h"

CPcapFile::CPcapFile(const char* fileName)
{
    this->fileName = fileName;
}

void CPcapFile::parsePcapFile(pcap_func function)
{
    int offset = 0;
    pcapFileHeader_t  pcapFileHeader = {0};
    pcapPkthdr_t  packetHeader = {0};
    char srcip[20] = {0};
    char dstip[20] = {0};

    fileHandler.open(fileName);
    if (!fileHandler)
    {
        cout << "The file does not exits or file name is error" << endl;
        return;
    }

    //读取pcap文件头部长度
    fileHandler.read((char *)&pcapFileHeader, sizeof(pcapFileHeader));
    if ( pcapFileHeader.magic != PCAP_FILE_MAGIC )
    {
        cout << "The file is not a pcap file" << endl;
        return;
    }

    while (fileHandler.read((char *)&packetHeader, sizeof(packetHeader)))
    {
        // 数据包头
        if ( packetHeader.caplen != packetHeader.len )
        {
            cout << "It is a invalid packet" << endl;
            fileHandler.seekg(packetHeader.caplen, ios::cur);
            continue;
        }
        if ( packetHeader.caplen > PCAP_PCAKET_MAX_LEN )
        {
            cout << "buff not enough:%d/%d" << packetHeader.caplen << PCAP_PCAKET_MAX_LEN << endl;
            return;
        }

        fileHandler.read(m_buff, packetHeader.caplen);
        offset = 0;

        // 物理层
        ethernet_t *ethernet = (ethernet_t *)(m_buff + offset);
        printf("SrcMac:%02x:%02x:%02x:%02x:%02x:%02x\n", ethernet->SrcMac[0], ethernet->SrcMac[1], ethernet->SrcMac[2], ethernet->SrcMac[3], ethernet->SrcMac[4], ethernet->SrcMac[5]);
        printf("DstMac:%02x:%02x:%02x:%02x:%02x:%02x\n", ethernet->DstMac[0], ethernet->DstMac[1], ethernet->DstMac[2], ethernet->DstMac[3], ethernet->DstMac[4], ethernet->DstMac[5]);
        offset += sizeof(ethernet_t);

        // 数据报
        ip_header_t *iphdr = (ip_header_t *)(m_buff + offset);
        inet_ntop(AF_INET, (void *)&iphdr->SrcIP, srcip, 16);
        inet_ntop(AF_INET, (void *)&iphdr->DstIP, dstip, 16);
        cout << srcip << " --> " << dstip << endl;
        offset += sizeof(ip_header_t);

        printf("Protocol:%u.\n", iphdr->Protocol);
        if ( iphdr->Protocol == E_TCP )
        {
            tcp_header_t *tcphdr = (tcp_header_t *)(m_buff + offset);
            cout << ntohs(tcphdr->SrcPort) << " --> " << ntohs(tcphdr->DstPort) << endl;
            offset += sizeof(tcp_header_t);
        }
        else if ( iphdr->Protocol == E_UDP )
        {
            udp_header_t *udphdr = (udp_header_t *)(m_buff + offset);
            cout << ntohs(udphdr->SrcPort) << " --> " << ntohs(udphdr->DstPort) << endl;
            offset += sizeof(udp_header_t);
        }

        // 外部应用数据处理函数
        function((unsigned char *)(m_buff + offset), packetHeader.caplen - offset);
    }
}

CPcapFile::~CPcapFile()
{
    fileHandler.close();
}
