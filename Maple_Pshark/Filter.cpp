#include "pch.h"
#include "Filter.h"
#include "Rule.h"
#include <vector>

int Filter::filter_action(char* msg_buff)
{
	for (int i = 0; i < this->ruleset.size(); i++)
	{
		if (ruleset[i].rule_execute(msg_buff) < 0)
		{
			if (calculate_drop(ruleset[i].droprate) == ACTION_DROP)
			{
				return -1;
			}
		}
	}

	return 0;
}

Action Filter::calculate_drop(double droprate)
{
	int random_var = std::rand() % 10;

	if (random_var < droprate * 10)
	{
		return ACTION_DROP;
	}
	else
	{
		return ACTION_PASS;
	}
}
;

void Filter::Add_rule(Rule rule_to_add)
{
	ruleset.push_back(rule_to_add);
}

void Filter::Remove_rule(Rule rule_to_remove)
{
	for (int i = 0; i < this->ruleset.size(); i++)
	{
		if (this->ruleset[i].rulename.compare(rule_to_remove.rulename))
		{
			this->ruleset.erase(this->ruleset.begin() + i);
		}
	}
}

void Filter::Update_rule()
{

}

Filter::Filter(std::string filtername)
{
	this->filtername = filtername;
}