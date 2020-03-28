#include "pch.h"
#include "Rule.h"


Rule::Rule(std::string rulename,identifier ident, Action act, modify_data mod_data, double droprate)
{
	switch (act) {
	case ACTION_MODIFY:
		this->mod_data = mod_data;
		break;
	case ACTION_ADD:
		this->mod_data = mod_data;
		break;
	case ACTION_PASS:
		break;
	case ACTION_DROP:
		this->droprate = droprate;
		break;
	default:
		break;
	}

	this->ident = ident;
	this->act = act;
}

void Rule::modify(int* msg_buff)
{
	if (this->act == ACTION_MODIFY)
	{
		if (this->identify(msg_buff) > 0)
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
		if (ident.identifier[i] != msg_buff[i])
		{
			return -1;
		}
	}

	return 0;
}