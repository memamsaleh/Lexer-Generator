#ifndef REGEXEVALUATER_H
#define REGEXEVALUATER_H

#include <stack>
#include "thompsonConstruction.h"
#include "automata.h"
#include <unordered_map>
#include <iostream>

namespace RegexEvaluater
{
	std::string postFixConversion(const std::string& exp);
	pAutomata evaluate(const std::string& exp);
};

#endif