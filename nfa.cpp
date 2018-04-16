#include "nfa.h"

using namespace std;

NFA::NFA() {}

void findAndReplaceAll(std::string& line, std::string toSearch, std::string replaceStr)
{
	size_t pos = line.find(toSearch);

	while (pos != std::string::npos)
	{
		line.replace(pos, toSearch.size(), replaceStr);
		pos = line.find(toSearch, pos + replaceStr.size());
	}
}

std::string regexConcatenate(const std::string& line)
{
	string newLine;
	for (int i = 0; i < line.size(); i++)
	{
		newLine.push_back(line[i]);

		if (line[i] == '(' || line[i] == '\\' || line[i] == '|' || line[i] == '-')
			continue;

		if(i + 1 < line.size())
			if (line[i + 1] != '|' && line[i + 1] != '*' && line[i + 1] != '+' && line[i + 1] != ')' && line[i+1] != '-')
				newLine.push_back('.');

	}
	return newLine;
}

pAutomata NFA::combineNFA()
{
	pState start(new State());
	pState end(new State());
	for (int i = 0; i < collections.size(); i++)
	{
		start->addChild(collections[i]->start, "\\L");
		collections[i]->end->addChild(end, "\\L");
	}
	return pAutomata(new Automata(start, end));
}

pAutomata NFA::getNFA(const std::string& grammarPath)
{
	ifstream gFile(grammarPath);
	string line;

	if (gFile.is_open())
	{
		while (getline(gFile, line))
		{
			//cout << line << endl;
			size_t eIndex = line.find(":");
			size_t dIndex = line.find("=");

			if (line.empty())
				continue;
			else if (line[0] == '[')
				handlePunc(line);
			else if (line[0] == '{')
				handleKeyword(line);
			else if (eIndex != string::npos)
			{
				string lhs = line.substr(0, eIndex);
				string rhs = line.substr(eIndex + 1, line.size());
				map<string, string>::reverse_iterator itr = definitions.rbegin();
				for (itr; itr != definitions.rend(); ++itr)
				{
					size_t indx = rhs.find(itr->first);
					if (indx != string::npos)
					{
						findAndReplaceAll(rhs, itr->first, "(" + itr->second + ")");
					}
				}
				findAndReplaceAll(rhs, ".", "\\.");
				findAndReplaceAll(rhs, " ", "");
				rhs = regexConcatenate(rhs);
				//cout << lhs << "->" << rhs << endl;
				pAutomata temp = RegexEvaluater::evaluate(rhs);
				temp->end->isAcceptance = true;
				temp->end->acceptanceType = lhs;
				collections.push_back(temp);
				//temp->print();
				//pair<string, string> p(lhs, rhs);
				//expressions.insert(p);
			}
			else if (dIndex != string::npos)
			{
				string lhs = line.substr(0, dIndex);
				string rhs = line.substr(dIndex + 1, line.size());
				map<string, string>::reverse_iterator itr = definitions.rbegin();
				for (itr; itr != definitions.rend(); ++itr)
				{
					size_t indx = rhs.find(itr->first);
					if (indx != string::npos)
					{
						findAndReplaceAll(rhs, itr->first, "(" + itr->second + ")");
					}
				}
				pair<string, string> p(lhs, rhs);
				definitions.insert(p);
				//cout << lhs << "->" << rhs << endl;
			}
		}
		gFile.close();
		return combineNFA();
	}
	else
	{
		cout << "File error" << endl;
		return pAutomata(new Automata());
	}
}

void NFA::handleKeyword(const std::string& line)
{
	for (int i = 1; i < line.size(); i++)
	{
		if (line[i] == ' ')
			continue;

		if (line[i] == '}')
			break;

		string keyword;
		keyword.push_back(line[i]);
		inputs.insert(line[i]);
		pAutomata temp = ThompsonConstruction::symbol(string(1, line[i]));
		i++;
		while (line[i] != ' ' && line[i] != '}')
		{
			keyword.push_back(line[i]);
			pState s(new State());
			temp->end->addChild(s, string(1, line[i]));
			temp->end = s;
			//pAutomata temp2 = ThompsonConstruction::symbol(string(1, line[i]));
			//temp = ThompsonConstruction::concatenate(temp, temp2);
			i++;
		}
		//temp->print();
		temp->end->isAcceptance = true;
		temp->end->acceptanceType = keyword;
		keyword = "";
		collections.push_back(temp);
	}
}

void NFA::handlePunc(const std::string& line)
{
	for (int i = 1; i < line.size(); i++)
	{
		if (line[i] == ' ' || line[i] == '\\')
			continue;

		if (line[i] == ']')
			break;

		inputs.insert(line[i]);
		pAutomata temp = ThompsonConstruction::symbol(string(1, line[i]));
		temp->end->isAcceptance = true;
		temp->end->acceptanceType = line[i];
		collections.push_back(temp);
		//temp->print();
	}
}

/*
pAutomata NFA::handleRegex(const string& line)
{
	cout << line << endl;
	//string exp = RegexEvaluater::postFixConversion(line);
	//cout << exp << endl;
	pAutomata a = RegexEvaluater::evaluate(line);
	//a->print();
	//pAutomata a(new Automata());
	return a;
}
*/