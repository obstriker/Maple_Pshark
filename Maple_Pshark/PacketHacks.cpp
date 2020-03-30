#include "pch.h"
#include <cstdlib>
#include <iostream>
#include "PacketHacks.h"

static int packet_identify(char* packet_buff) {
	return 0;
}

static int packet_godmode(char* packet_buff, double droprate) {
	int random_var = std::rand() % 10;

	if (random_var <= droprate * 10)
	{
		//drop
		
	}
	else
	{
		//pass packet normally
	}
	return 0;
}

static int autologin(char* username, char* password)
{
	char packet_buff[50];
	//TODO

	//PacketInject();
}
