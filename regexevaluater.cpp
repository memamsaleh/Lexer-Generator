#include "regexEvaluater.h"

using namespace std;

namespace RegexEvaluater
{
	string postFixConversion(const string& exp)
	{
		std::stack<std::string> operand;
		std::stack<char> operation;

		string op = "";
		int i = 0;
		while (i < exp.size())
		{
			char c = exp[i];
			if (c == ' ')
			{
				i++;
				continue;
			}

			if (c == '\\')
			{
				op = "\\" + string(1, exp[i + 1]);
				i += 2;
				operand.push(op);
				op = "";
				continue;
			}

			if (c == '-')
			{
				if (i + 1 < exp.size() && exp[i + 1] != '*' && exp[i + 1] != '+' && exp[i + 1] != '.' && exp[i + 1] != '|' && exp[i + 1] != ')')
				{
					op = operand.top() + '-' + string(1, exp[i + 1]);
					operand.pop();
					operand.push(op);
					op = "";
					i += 2;
					continue;
				}
			}

			if ((c != '*') && (c != '+') && (c != '.') && (c != '|') && (c != '(') && (c != ')'))
			{
				operand.push(string(1, c));
				i++;
			}
			else if (c == '*' || c == '+' || c == '(')
			{
				operation.push(c);
				i++;
			}
			else if (c == '.' || c == '|')
			{
				if (operation.empty())
				{
					operation.push(c);
					i++;
				}
				else
				{
					if ((operation.top() == '*') || operation.top() == '+')
					{
						op = op + operand.top() + operation.top();
						operand.pop();
						operation.pop();
						operand.push(op);
						operation.push(c);
						op = "";
						i++;
					}
					else if (operation.top() == '.')
					{
						op = op + operand.top();
						operand.pop();
						op = op + operand.top() + operation.top();
						operand.pop();
						operation.pop();
						operand.push(op);
						operation.push(c);
						op = "";
						i++;
					}
					else
					{
						operation.push(c);
						i++;
					}
				}
			}
			else if (c == ')')
			{
				if (operation.empty())
				{
					cout << "Wrong Expression" << endl;
				}
				while ((operation.top() != '(') && (!operation.empty()) && (!operand.empty()))
				{
					if ((operation.top() == '*' || operation.top() == '+'))
					{
						op = op + operand.top() + operation.top();
						operation.pop();
						operand.pop();
						operand.push(op);
						op = "";
					}
					else if ((operation.top() == '|') || (operation.top() == '.'))
					{
						op = op + operand.top();
						operand.pop();
						op = op + operand.top() + operation.top();
						operand.pop();
						operation.pop();
						operand.push(op);
						op = "";
					}
				}

				operation.pop();
				i++;
			}
		}

		if (operand.empty())
		{
			return nullptr;
		}

		if (operation.empty())
		{
			op = operand.top();
			operand.pop();
		}
		else
		{
			while ((!operand.empty()) && (!operation.empty()))
			{
				if ((operation.top() == '*') || (operation.top() == '+'))
				{
					op = op + operand.top() + operation.top();
					operand.pop();
					operation.pop();
					operand.push(op);
					op = "";
				}
				else if ((operation.top() == '|') || (operation.top() == '.'))
				{
					op = op + operand.top();
					operand.pop();
					op = op + operand.top() + operation.top();
					operand.pop();
					operation.pop();
					operand.push(op);
					op = "";
				}
			}
			op = operand.top();
			while (!operation.empty())
			{
				op = op + operation.top();
				operation.pop();
			}
		}

		return op;
	}

	pAutomata evaluate(const string& exp)
	{
		string postFix = postFixConversion(exp);
		pAutomata automata;
		string op = "";
		int i = 0;
		stack<pAutomata> opStack;

		for (int i = 0; i < postFix.size(); i++)
		{
			char c = postFix[i];
			if (c == '\\')
			{
				i++;
				pAutomata temp = ThompsonConstruction::symbol(string(1, postFix[i]));
				opStack.push(temp);
				continue;
			}

			if (c == '-')
			{
				if (i - 1 >= 0)
				{
					char begin = postFix[i - 1];
					char end = postFix[i + 1];
					if (isalnum(begin) && isalnum(end))
					{
						pAutomata temp = opStack.top();
						opStack.pop();
						int b = begin;
						int e = end;
						int range = e - b;
						for (int j = 1; j <= range; j++)
						{
							char debug = b + j;
							temp->start->addChild(temp->end, string(1, debug));
						}
						opStack.push(temp);
						i++;
						continue;
					}
				}
			}
			
			if ((c != '*') && (c != '+') && (c != '.') && (c != '|'))
			{				
				pAutomata temp = ThompsonConstruction::symbol(string(1, c));
				opStack.push(temp);
			}
			else
			{
				if (c == '*')
				{
					pAutomata g1(opStack.top());
					opStack.pop();
					opStack.push(ThompsonConstruction::kColusre(g1));
				}
				else if (c == '+')
				{
					pAutomata g1(opStack.top());
					opStack.pop();
					opStack.push(ThompsonConstruction::pClosure(g1));
				}
				else if (c == '.')
				{
					pAutomata g1(opStack.top());
					opStack.pop();
					pAutomata g2(opStack.top());
					opStack.pop();
					opStack.push(ThompsonConstruction::concatenate(g1, g2));
				}
				else if (c == '|')
				{
					pAutomata g1(opStack.top());
					opStack.pop();
					pAutomata g2(opStack.top());
					opStack.pop();
					opStack.push(ThompsonConstruction::unionG(g1, g2));
				}
			}
		}

		return opStack.top();
	}
}
