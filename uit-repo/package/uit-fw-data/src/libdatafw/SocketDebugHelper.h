#pragma once
#include <uit/core/Def.h>
#include <Poco/Thread.h>
#include <Poco/Net/TCPServer.h>
#include <Poco/Net/TCPServerConnectionFactory.h>

namespace uit { namespace datafw {

class Model_Internal;

class Connection : public Poco::Net::TCPServerConnection
{
public:
	Connection(const Poco::Net::StreamSocket &s) : Poco::Net::TCPServerConnection(s) {}

	virtual void run();
};

class ConnectionFactory : public Poco::Net::TCPServerConnectionFactory
{
public:
	virtual Poco::Net::TCPServerConnection* createConnection(const Poco::Net::StreamSocket& socket)
	{
		return new Connection(socket);
	}
};

class SocketDebugHelper
{
public:
	SocketDebugHelper(Model_Internal *model);
	~SocketDebugHelper();

	void startup();
	void stop();

private:
	//获取etho0和mlan0的ip
	std::string getDeviceIp(const std::string &sDev);
	void getLoacalIp(std::string &eth0, std::string &mlan0);
	
private:
	Poco::SharedPtr<Poco::Net::TCPServer>	m_tcpServer;
};

}}
