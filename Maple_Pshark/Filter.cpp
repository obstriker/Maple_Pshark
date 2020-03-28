#include "pch.h"
#include "Filter.h"
#include <vector>


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

Filter::Filter(Rule rule, std::string filtername)
{
	this->filtername = filtername;
	this->Add_rule(rule);
}