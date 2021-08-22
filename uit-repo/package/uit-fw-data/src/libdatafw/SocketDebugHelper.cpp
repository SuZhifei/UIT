#include "SocketDebugHelper.h"
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Parser.h>
#include <uit/core/Log.h>
#include "Share.h"
#include "Model_Internal.h"

#define MAX_BUFFER_SIZE		2048
#define SOCKET_SERVER_PORT	8888

using namespace uit::datafw;

static Model_Internal	*g_model;

void Connection::run()
{
	std::string sPeerIp = socket().peerAddress().host().toString();
	Log::info(DATAFW_SOCKETDEBUGGER_LOG_TAG, "new peer:%s", sPeerIp.data());
	while (1)
	{
		Poco::FIFOBuffer buffer(MAX_BUFFER_SIZE);
		int n = socket().receiveBytes(buffer);
		if (n == 0 || buffer.isEmpty())
			break;

		Log::info(DATAFW_SOCKETDEBUGGER_LOG_TAG, "socket debug json:\r\n%s", buffer.begin());
		std::string msg(buffer.begin(), buffer.size());
		Poco::JSON::Object::Ptr root;
		try {
			Poco::JSON::Parser parser;
			root = parser.parse(msg).extract<Poco::JSON::Object::Ptr>();
		}
		catch (...) {
			Log::error(DATAFW_SOCKETDEBUGGER_LOG_TAG, "parse error when get a cmd, ignore", msg.data());
			return;
		}
		const std::string &sFuncName = root->begin()->first;
		try {
			Poco::JSON::Object::Ptr params = root->begin()->second.extract<Poco::JSON::Object::Ptr>();
			if (sFuncName == "setBoolean")
			{
				std::string sPath = params->get("path");
				Poco::Dynamic::Var value = params->get("value");
				g_model->setBoolean(sPath, value.convert<bool>(), uit::getTickCount());
			}
			else if (sFuncName == "setInteger")
			{
				std::string sPath = params->get("path");
				Poco::Dynamic::Var value = params->get("value");
				g_model->setInteger(sPath, value.convert<int>(), uit::getTickCount());
			}
			else if (sFuncName == "setReal")
			{
				std::string sPath = params->get("path");
				Poco::Dynamic::Var value = params->get("value");
				g_model->setReal(sPath, value.convert<double>(), uit::getTickCount());
			}
			else if (sFuncName == "setString")
			{
				std::string sPath = params->get("path");
				Poco::Dynamic::Var value = params->get("value");
				g_model->setString(sPath, value.convert<std::string>(), uit::getTickCount());
			}
			else if (sFuncName == "setEnumStringValue")
			{
				std::string sPath = params->get("path");
				Poco::Dynamic::Var value = params->get("value");
				g_model->setEnumStringValue(sPath, value.convert<std::string>(), uit::getTickCount());
			}
			else if (sFuncName == "resizeList")
			{
				std::string sPath = params->get("path");
				Poco::Dynamic::Var size = params->get("size");
				g_model->resizeList(sPath, size.convert<int>());
			}
			else if (sFuncName == "insertListItems")
			{
				std::string sPath = params->get("path");
				Poco::Dynamic::Var beg = params->get("beg");
				Poco::Dynamic::Var count = params->get("count");
				g_model->insertListItems(sPath, beg.convert<int>(), count.convert<int>());
			}
			else if (sFuncName == "removeListItems")
			{
				std::string sPath = params->get("path");
				Poco::Dynamic::Var beg = params->get("beg");
				Poco::Dynamic::Var count = params->get("count");
				g_model->removeListItems(sPath, beg.convert<int>(), count.convert<int>());
			}
			else if (sFuncName == "updateListItems")
			{
				std::string sPath = params->get("path");
				Poco::JSON::Array::Ptr indexs = params->getArray("indexs");
				Poco::JSON::Array::Ptr items = params->getArray("items");

				std::vector<uint32_t> ids;
				for (auto iter = indexs->begin(); iter != indexs->end(); ++iter)
					ids.push_back((*iter).convert<int>());
				std::string sJson = Poco::Dynamic::Var(items).toString();
				std::vector<Types::Image> images;
				g_model->updateListItems(sPath, ids, sJson, images);
			}
			else if (sFuncName == "updateListViewport")
			{
				std::string sPath = params->get("path");
				Poco::Dynamic::Var first = params->get("first");
				g_model->updateListViewport(sPath, first.convert<int>());
			}
			else if (sFuncName == "checkListItem")
			{
				std::string sPath = params->get("path");
				Poco::Dynamic::Var index = params->get("index");
				g_model->checkListItem(sPath, index.convert<int>());
			}
			else if (sFuncName == "focusListItem")
			{
				std::string sPath = params->get("path");
				Poco::Dynamic::Var index = params->get("index");
				g_model->focusListItem(sPath, index.convert<int>());
			}
			else if (sFuncName == "focusListItem")
			{
				std::string sPath = params->get("path");
				Poco::Dynamic::Var index = params->get("index");
				g_model->focusListItem(sPath, index.convert<int>());
			}
			else if (sFuncName == "vkey")
			{
				Poco::Dynamic::Var key = params->get("key");
				Poco::Dynamic::Var state = params->get("state");
				g_model->vkey(key.convert<int>(), state.convert<int>());
			}
			else if (sFuncName == "postEvent")
			{
				Poco::Dynamic::Var eventName = params->get("path");
				Poco::Dynamic::Var args = params->get("args");
				g_model->postEvent(eventName.toString(), args.toString());
			}
			else
			{
				Log::error(DATAFW_SOCKETDEBUGGER_LOG_TAG, "unknown function when call [%s]", __FUNCTION__);
			}
		}
		catch (...) {
			Log::error(DATAFW_SOCKETDEBUGGER_LOG_TAG, "parse params error when call [%s]", __FUNCTION__);
			return;
		}
	}
	Log::info(DATAFW_SOCKETDEBUGGER_LOG_TAG, "peer [%s] quit.", sPeerIp.data());
}

SocketDebugHelper::SocketDebugHelper(Model_Internal *model)
{
	g_model = model;
}

SocketDebugHelper::~SocketDebugHelper()
{
}

void SocketDebugHelper::startup()
{
	std::string eth0, mlan0;
	getLoacalIp(eth0, mlan0);
	std::string sIp = eth0.empty() ? (mlan0.empty() ? "" : mlan0) : eth0;
	try {
		Poco::Net::ServerSocket serverSk(Poco::Net::SocketAddress(sIp, SOCKET_SERVER_PORT));
		m_tcpServer = new Poco::Net::TCPServer(new ConnectionFactory(), serverSk);
		m_tcpServer->start();
		Log::info(DATAFW_SOCKETDEBUGGER_LOG_TAG, "server startup success: ip[%s], port[%d].", sIp.data(), SOCKET_SERVER_PORT);
	}
	catch(...){
		Log::info(DATAFW_SOCKETDEBUGGER_LOG_TAG, "server startup fail: ip[%s], port[%d].", sIp.data(), SOCKET_SERVER_PORT);
	}
}

void SocketDebugHelper::stop()
{
	m_tcpServer->stop();
}

std::string SocketDebugHelper::getDeviceIp(const std::string &sDev)
{
#ifdef WIN32
	return "";
#else
	char ip[80] = { 0 };
	struct ifreq ifr;
	int sk = socket(AF_INET, SOCK_DGRAM, 0);
	strcpy(ifr.ifr_name, sDev.data());
	if (ioctl(sk, SIOCGIFADDR, &ifr) == 0)
		strcpy(ip, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

	close(sk);
	return ip;
#endif
}

void SocketDebugHelper::getLoacalIp(std::string &eth0, std::string &mlan0)
{
#ifdef WIN32
	WSADATA Data;
	if (WSAStartup(MAKEWORD(1, 1), &Data) == 0)
	{
		char hostName[256] = { 0 };
		gethostname(hostName, sizeof(hostName));
		PHOSTENT hostinfo;
		hostinfo = gethostbyname(hostName);
		eth0 = inet_ntoa(*(struct in_addr*)*hostinfo->h_addr_list);
		WSACleanup();
	}
	else
	{
		Log::error(DATAFW_SOCKETDEBUGGER_LOG_TAG, "can't startup winSock when get ip.");
	}
#else
	//system("udhcpc -i eth0");
	eth0 = getDeviceIp("eth0");
	mlan0 = getDeviceIp("mlan0");
#endif
}
