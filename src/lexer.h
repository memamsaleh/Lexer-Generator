#ifndef LEXER_H
#define LEXER_H

#include "dfa.h"

struct Token
{
	std::string type;
	std::string lexeme;
};

class Lexer
{
public:
	Lexer(const std::string& codePath);

	inline Token getToken() { return index < tokens.size()? tokens[index++]: Token(); }
private:
	std::vector<Token> tokens;
	unsigned int index;
};

#endif
