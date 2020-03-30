#include "pch.h"
#include "Rule.h"



Rule::Rule(std::string rulename, identifier ident, Action act, modify_data mod_data, double droprate)
{
	this->mod_data = mod_data;
	this->droprate = droprate;
	this->ident = ident;
	this->act = act;
}

Rule::Rule(const Rule& rule2)
{
	act = rule2.act;
	droprate = rule2.droprate;
	ident = rule2.ident;
	//malloc ?
	mod_data = rule2.mod_data;
	rulename = rule2.rulename;
}

void Rule::modify(int* msg_buff)
{
	if (this->act == ACTION_MODIFY)
	{
		if (this->identify(msg_buff) > -1)
		{
			for (int i = this->mod_data.place; i < this->mod_data.size; i++)
			{
				msg_buff[i] = mod_data.mod_data[i];
			}
		}
	}
}

int Rule::identify(int* msg_buff)
{
	for (int i = this->ident.place; i < this->ident.size; i++)
	{
		if (ident.identifier[i] != ((char*)msg_buff)[i])
		{
			return -1;
		}
	}
	return 0;
}

int Rule::rule_execute(char* msg_buff)
{
	if(this->identify((int*)msg_buff) > -1)
	{
		switch (this->act) {
		case ACTION_MODIFY:
			this->modify((int*)msg_buff);
			break;
		case ACTION_ADD:
			//TODO
			break;
		case ACTION_PASS:
			//Nothing TODO
			break;
		case ACTION_DROP:
			return -1;
			break;
		default:
			break;
		}
	}
	return 0;
}