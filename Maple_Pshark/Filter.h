#pragma once
#include "Rule.h"
#include <vector>

using namespace std;
class Filter
{
public:
	std::string filtername;
	std::vector<Rule> ruleset;

	int filter_action(char* msg_buff);
	void Add_rule(Rule rule_to_add);
	void Remove_rule(Rule rule_to_remove);
	void Update_rule();
	Action calculate_drop(double droprate);
	Filter(std::string filtername);
};
