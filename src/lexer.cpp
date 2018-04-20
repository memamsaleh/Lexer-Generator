#include "lexer.h"
#include <fstream>
#include <iostream>

using namespace std;

Lexer::Lexer(const string& codePath)
{
	index = 0;

	ifstream cFile(codePath);
	string line;

	if (cFile.is_open())
	{
		while (getline(cFile, line))
		{
			DFA& d = DFA::getDFA();
			pDFAState state = d.start;
			pDFAState lastAcceptance;
			string lexeme = "";
			string temp = "";
			int aIndex = 0;

			for (int i = 0; i < line.size(); i++)
			{
				char c = line[i];

				if (c == ' ' || c == '\n')
				{
					Token t;
					if (lastAcceptance == nullptr)
					{
						std::cout << "Error in Program File" << std::endl;
						return;
					}
					t.type = lastAcceptance->acceptanceType;
					t.lexeme = lexeme;
					tokens.push_back(t);
					i = aIndex + 1;
					state = d.start;
					lexeme = "";
					temp = "";
				}
				else
				{
					state = d.transitionTable[pair<pDFAState, char>(state, c)];
					temp.push_back(c);
					if (state->isAcceptance)
					{
						aIndex = i;
						lastAcceptance = state;
						lexeme = temp;
					}
				}
			}
		}
	}
}