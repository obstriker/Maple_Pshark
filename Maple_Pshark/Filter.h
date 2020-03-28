#pragma once
#include "Rule.h"
#include <vector>

class Filter
{
	public:
		std::string filtername;
		std::vector<Rule> ruleset;

		void Add_rule(Rule rule_to_add);
		void Remove_rule(Rule rule_to_remove);
		void Update_rule();
		Filter(Rule rule, std::string filtername);
};
