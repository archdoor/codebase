#include <fstream>
#include <iostream>

using namespace std;

#define  PCAP_FILE_MAGIC   0XA1B2C3D4
#define  PCAP_PCAKET_MAX_LEN  2048 

// 协议类型
typedef enum
{
    E_TCP = 0x06,
    E_UDP = 0x11,
}e_protocol_type;

typedef  void (*pcap_func)(unsigned char *data, int datalen);

// 文件头
typedef struct pcapFileHeader
{
    unsigned int   magic;
    unsigned short   version_major;
    unsigned short   version_minor;
    int    thiszone;      // 时区修正
    unsigned int   sigfigs;       // 精确时间戳
    unsigned int   snaplen;       // 抓包最大长度
    unsigned int   linktype;      // 链路类型
} __attribute__ ((packed)) pcapFileHeader_t;

// 数据包头
typedef struct pcapPkthdr
{
    unsigned int   seconds;     // 秒数
    unsigned int   u_seconds;   // 毫秒数
    unsigned int   caplen;      // 数据包长度
    unsigned int   len;         // 文件数据包长度
} __attribute__ ((packed)) pcapPkthdr_t;

// 物理层
typedef struct Ethernet
{
    unsigned char DstMac[6];
    unsigned char SrcMac[6];
    unsigned short FrameType;
} __attribute__ ((packed)) ethernet_t;

// IP数据报头
typedef struct ipHeader
{
    unsigned char   Ver_HLen;       //版本+报头长度
    unsigned char   TOS;            //服务类型
    unsigned short  TotalLen;       //总长度
    unsigned short  ID;     //标识
    unsigned short  Flag_Segment;   //标志+片偏移
    unsigned char   TTL;            //生存周期
    unsigned char   Protocol;       //协议类型
    unsigned short  Checksum;       //头部校验和
    unsigned int  SrcIP;  //源IP地址
    unsigned int  DstIP;  //目的IP地址
} __attribute__ ((packed)) ip_header_t;

// TCP数据报头
typedef struct tcp_header
{
    unsigned short  SrcPort;    //源端口
    unsigned short  DstPort;    //目的端口
    unsigned int  SeqNO;  //序号
    unsigned int  AckNO;  //确认号
    unsigned char   HeaderLen;  //数据报头的长度(4 bit) + 保留(4 bit)
    unsigned char   Flags;  //标识TCP不同的控制消息
    unsigned short  Window;     //窗口大小
    unsigned short  Checksum;   //校验和
    unsigned short  UrgentPointer;  //紧急指针
} __attribute__ ((packed)) tcp_header_t;

// UDP数据报头
typedef struct udp_header
{
    unsigned short SrcPort;    //源端口
    unsigned short DstPort;    //目的端口
    unsigned short uhl;      //udp头部长度   
    unsigned short chk_sum;  //16位udp检验和  
} __attribute__ ((packed)) udp_header_t;


class CPcapFile
{
public:
    CPcapFile(const char* fileName);
    ~CPcapFile();
    void  parsePcapFile(pcap_func function);

private:
    ifstream  fileHandler;
    const char*    fileName;
    char m_buff[PCAP_PCAKET_MAX_LEN];
};

