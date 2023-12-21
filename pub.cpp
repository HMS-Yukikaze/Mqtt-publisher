#include "pub.hpp"

Publisher::Publisher(const char* _id, const char* _host, int _port)
	: mosquittopp{ _id },
	MaxSize{ 100 }
{
	host = std::string(_host);
	port = _port;
	mosq = nullptr;
	mosqpp::lib_init();
}

Publisher::~Publisher()
{
	std::cout << "Cleanup...\n";
	mosqpp::lib_cleanup();
}

bool Publisher::login(const char* username, const char* password, int keepAlive)
{
	int ret = 0;
	reconnect_delay_set(3, 5, true);

	ret = username_pw_set(username, password);
	if (ret) {
		std::cout << mosqpp::strerror(ret) << std::endl;
		return false;
	}
	ret = connect_async(host.c_str(), port, keepAlive);
	if (ret) {
		std::cout << mosqpp::strerror(ret) << std::endl;
		return false;
	}
	return true;
}



std::wstring Publisher::Utf8ToWstring(const std::string& str)
{
	LPCSTR pszSrc = str.c_str();
	int Len = MultiByteToWideChar(CP_UTF8, 0, pszSrc, -1, NULL, 0);
	if (Len == 0)
		return std::wstring(L"");

	wchar_t* pwszDst = new wchar_t[Len];
	if (!pwszDst)
		return std::wstring(L"");

	MultiByteToWideChar(CP_UTF8, 0, pszSrc, -1, pwszDst, Len);
	std::wstring wstr(pwszDst);
	delete[] pwszDst;
	pwszDst = NULL;

	return wstr;
}

std::string Publisher::Format2Utf8(const std::wstring& src)
{
	int utf8Length = WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (utf8Length == 0)
		return std::string("");

	char* utf8Buffer = new char[utf8Length];
	if (!utf8Buffer)
		return std::string("");

	WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, utf8Buffer, utf8Length, nullptr, nullptr);

	std::string str(utf8Buffer);
	delete[] utf8Buffer;

	return str;
}

void Publisher::AlarmNotify(const std::string& message)
{
	std::unique_lock<std::mutex> lk(mt);
	not_full_cv.wait(lk, [this] { return Msg_queue.size() < MaxSize; });
	Msg_queue.push(message);
	not_empty_cv.notify_one();
}

void Publisher::MsgHandle()
{
	while (true)
	{
		std::string str = {};
		GetMsg(str);
		std::cout << "receive msg:" << str << std::endl;
	}
}

void Publisher::GetMsg(std::string& message)
{
	std::unique_lock<std::mutex> lk(mt);
	not_empty_cv.wait(lk, [this] { return !Msg_queue.empty(); });
	message = Msg_queue.front();
	Msg_queue.pop();
	not_full_cv.notify_one();
}
