#ifndef __NET_SOCKET_H__
#define __NET_SOCKET_H__

// TCP
int GetTcpSocket();
int GetTcpBindSocket(const char *ip, unsigned short port);
int SetSockTimeout(int sockfd, int sec);
int Connect(int sockfd, const char *ip, unsigned short port);
int Listen(int sockfd, int backlog);
int Accept(int sockfd, struct sockaddr_in *addr);

int Recv(int sockfd, void *buff, int len);
int NBlockRecv(int sockfd, void *buff, int len);
int PersistRecv(int sockfd, void *buff, int len);
int RecvAll(int sockfd, void *buff, int len);
int FackRecv(int sockfd, void *buff, int len);
int ClearRecv(int sock);

int Send(int sockfd, void *buff, int len);

// UDP
int GetUdpSocket();
int GetUdpBindSocket(const char *ip, unsigned short port);
int RecvFrom(int sockfd, void *buff, int len);
int FackRecvFrom(int sockfd, void *buff, int len);
int PersistRecvFrom(int sockfd, void *buff, int len);
int SendTo(int sockfd, const char *ip, unsigned short port, void *buff, int len);

int Bind(int sockfd, const char *ip, unsigned short port);
int CloseSock(int sock);

#endif
