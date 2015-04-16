#pragma once

#include <WinSock2.h>
#include "ZBase64.h"
#pragma comment(lib,"ws2_32")

#include <string>
using namespace std;

class CMail
{
public:
	CMail() { sSock = INVALID_SOCKET;};

	~CMail()
	{
		FreeSocket();
	}
	
public:
	BOOL SendMail(string SMTP,string UserName,string PassWorld,string DestMailUserName,string Subjuect,string Data);
private:
	//��Ϊ��������϶��ǰ�˳��������������Ҳ����Ӱ��Ч��
	BOOL ConnectSMTPServer(string SMTP);
	BOOL MailCmd(char* MailCmd,char* lparam,string &RetStr);
protected:

	//���׽��ֽ��в���
	BOOL InitSocket();

	BOOL CreateSocket();

	BOOL Listen();

	BOOL Bind(int nPort);

	void FreeSocket();

	BOOL Connect(LPSTR IP,int nPort = 25);

	//TCP �����׽���
	BOOL Send(const char *data,int nLen);

	BOOL Recv(string & buffer);
	
private:
	SOCKET sSock;

public:
	string LastError;
};