#pragma once 
#include <Windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <chrono>
#include "mosquitto.h"
#include "mosquittopp.h"



#pragma comment(lib,"mosquittopp.lib")



class SubscriberInterface {
public:
	virtual ~SubscriberInterface() {};

	virtual void AlarmNotify(const std::string& message) = 0;

	virtual void MsgHandle() = 0;

	virtual void GetMsg(std::string& message) = 0;

};

class PublisherInterface {
public:
	virtual ~PublisherInterface() {};

	virtual void addSubscriber(std::shared_ptr<SubscriberInterface> subscriber) = 0;

	virtual void removeSubscriber(std::shared_ptr<SubscriberInterface> subscriber) = 0;

	virtual void alarmPublish(const std::string& message) = 0;
};



class Publisher : public mosqpp::mosquittopp ,public SubscriberInterface
{
public:
	Publisher(const char* _id, const char* _host = "127.0.0.1", int _port = 1883);

	~Publisher();

	bool login(const char* username, const char* password, int keepAlive);

	std::wstring Utf8ToWstring(const std::string& str);

	std::string Format2Utf8(const std::wstring& src);

	void AlarmNotify(const std::string& message);

	void MsgHandle();
private:
	void GetMsg(std::string& message);
private:
	int MaxSize;                          /*消息队列最大容量*/
	std::mutex mt;
	std::condition_variable not_empty_cv; /*信号量*/
	std::condition_variable not_full_cv;  /*信号量*/
	std::queue<std::string> Msg_queue;    /*消息队列*/
protected:
	std::string host;
	int port;
	struct mosquitto* mosq;
};