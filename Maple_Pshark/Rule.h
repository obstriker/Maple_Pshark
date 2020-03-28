#pragma once
#include<string>

enum Action { ACTION_PASS , ACTION_DROP , ACTION_MODIFY , ACTION_ADD };

struct modify_data {
	int* mod_data;
	int place;
	int size;
};

struct identifier {
	int* identifier;
	int place;
	int size;
};

class Rule
{
	public:
		std::string rulename;
		modify_data mod_data;
		Action act;
		identifier ident;
		double droprate;

		Rule(std::string rulename, identifier ident, Action act, modify_data mod_data, double droprate);
		int identify(int* msg_buff);
		void modify(int* msg_buff);
};