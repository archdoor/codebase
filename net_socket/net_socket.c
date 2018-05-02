#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>

#include "net.h"
#include "log.h"

// 获取TCP套接字
int GetTcpSocket()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if ( sockfd == -1 ) 
    {
        return -1;
    }

    return sockfd;
}

// 获取TCP绑定套接字
int GetTcpBindSocket(const char *ip, unsigned short port)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if ( sockfd == -1 ) 
    {
        return -1;
    }

    if ( Bind(sockfd, ip, port) < 0 )
    {
        return -1;
    }

    return sockfd;
}

// 设置套接字超时
int SetSockTimeout(int sockfd, int sec)
{
    struct timeval tv_out;
    tv_out.tv_sec = sec;
    tv_out.tv_usec = 0;
    if ( setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out)) < 0 )
    {
        return -1;
    }
    return 0;
}

// 套接字关联
int Bind(int sockfd, const char *ip, unsigned short port)
{
    int on = 1;	
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

    struct sockaddr_in addr={0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    socklen_t size = sizeof(struct sockaddr_in);

    return bind(sockfd, (struct sockaddr *)&addr, size);
}

// 连接请求监听
int Listen(int sockfd, int backlog)
{
    return listen(sockfd, backlog);
}

// 接收连接请求
int Accept(int sockfd, struct sockaddr_in *addr)
{
	socklen_t size = sizeof(struct sockaddr_in);

    return accept(sockfd, (struct sockaddr *)addr, &size);
}

// 连接
int Connect(int sockfd, const char *ip, unsigned short port)
{
	struct sockaddr_in addr={0};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);

	socklen_t size = sizeof(struct sockaddr_in);

    if ( connect(sockfd, (struct sockaddr *)&addr, size) < 0 ) 
    {
        return -1;
    }

    return 0;
}

// 接收数据
int Recv(int sockfd, void *buff, int len)
{
    return recv(sockfd, buff, len, 0);
}

// 非阻塞接收数据
int NBlockRecv(int sockfd, void *buff, int len)
{
    return recv(sockfd, buff, len, MSG_DONTWAIT);
}

// 接收固定长度数据
int PersistRecv(int sockfd, void *buff, int len)
{
    int recv_len = 0;

    // TODO:增加超时或次数限制，防止死循环
    while ( recv_len < len )
    {
        int ret = recv(sockfd, ((unsigned char *)buff + recv_len), (len - recv_len), MSG_DONTWAIT);
        if ( ret <= 0 )
        {
            return ret;
        }
        recv_len += len;
    }

    return recv_len;
}

// 接收固定长度数据
int RecvAll(int sockfd, void *buff, int len)
{
    return recv(sockfd, (unsigned char *)buff, len, MSG_WAITALL);
}

// 接收数据，但不从缓存中取走
int FackRecv(int sockfd, void *buff, int len)
{
    int recv_len = 0;

    while ( recv_len < len )
    {
        recv_len = recv(sockfd, buff, len, MSG_PEEK);
        if ( recv_len <= 0 )
        {
            return recv_len;
        }
    }

    return recv_len;
}

// 清除Recv缓存
int ClearRecv(int sockfd)
{
    unsigned char buff[128] = {0};

    while (1)
    {
        int ret = recv(sockfd, buff, 127, MSG_DONTWAIT);
        if ( ret < 127 )
        {
            return -1;
        }
    }

    return 0;
}

// 发送数据
int Send(int sockfd, void *buff, int len)
{
    return send(sockfd, buff, len, 0);
}

// 获取UDP套接字
int GetUdpSocket()
{
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if ( sockfd == -1 ) 
    {
        return -1;
    }

    return sockfd;
}

// 获取UDP绑定套接字
int GetUdpBindSocket(const char *ip, unsigned short port)
{
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if ( sockfd == -1 ) 
    {
        return -1;
    }

    if ( Bind(sockfd, ip, port) < 0 )
    {
        return -1;
    }

    return sockfd;
}

// 接收UDP数据
int RecvFrom(int sockfd, void *buff, int len)
{
    return recvfrom(sockfd, buff, len, 0, NULL, NULL);
}

int FackRecvFrom(int sockfd, void *buff, int len)
{
    int recv_len = 0;

    while ( recv_len < len )
    {
        recv_len = recvfrom(sockfd, buff, len, MSG_PEEK, NULL, NULL);
        if ( recv_len <= 0 )
        {
            return recv_len;
        }
    }

    return recv_len;
}

// 接收固定长度数据
int PersistRecvFrom(int sockfd, void *buff, int len)
{
    int recv_len = 0;

    // TODO:增加超时或次数限制，防止死循环
    while ( recv_len < len )
    {
        int ret = recvfrom(sockfd, ((unsigned char *)buff + recv_len), (len - recv_len), MSG_DONTWAIT, NULL, NULL);
        if ( ret <= 0 )
        {
            return ret;
        }
        recv_len += len;
    }

    return recv_len;
}

// 发送UDP数据
int SendTo(int sockfd, const char *ip, unsigned short port, void *buff, int len)
{
	struct sockaddr_in addr={0};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);

	socklen_t size = sizeof(struct sockaddr_in);

    return sendto(sockfd, buff, len, 0, (struct sockaddr *)&addr, size);
}

// 关闭套接字
int CloseSock(int sockfd)
{
    return close(sockfd);
}

