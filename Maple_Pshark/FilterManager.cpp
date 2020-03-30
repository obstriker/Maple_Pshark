#include "pch.h"
#include "FilterManager.h"


FilterManager::FilterManager() {}


void FilterManager::add(Filter _filter)
{
	this->filters.push_back(_filter);
}

int FilterManager::filter_message(char* filtername, char* msg_buff)
{
	for (int i = 0; i < this->filters.size(); i++)
	{
		if (this->filters[i].filtername.compare(filtername))
		{
			if (filters[i].filter_action(msg_buff) < 0)
			{
				return -1;
			}
		}
	}
}