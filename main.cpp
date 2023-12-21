#include "pub.hpp"


int main(int argc, char** argv)
{
	class Publisher* client = nullptr;

	client = new Publisher("Test123");

	if(!client->login("userName","password",60)){
		std::cout<<"login failed"<<std::endl;
	}

	std::string str = client->Format2Utf8(L"哼啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊~");

	client->publish(NULL,"Test", str.length(), str.c_str(),0);

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	return 0;
}


