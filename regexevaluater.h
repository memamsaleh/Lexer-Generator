#ifndef REGEXEVALUATER_H
#define REGEXEVALUATER_H

#include <stack>
#include "thompsonConstruction.h"
#include "automata.h"
#include <set>
#include <iostream>

namespace RegexEvaluater
{
	std::string postFixConversion(const std::string& exp);
	pAutomata evaluate(const std::string& exp, std::set<char>& inputs);
};

#endif