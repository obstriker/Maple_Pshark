#pragma once
#include <vector>
#include "Filter.h"

class FilterManager
{
	vector<Filter> filters;
	FilterManager();
	int filter_message(char* filtername, char* msg_buff);
	void add(Filter _filter);
};
