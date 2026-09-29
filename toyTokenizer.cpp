#include "toyTokenizer.h"

TokenType lookupKeyword(char ch)
{
    switch (ch)
    {
    case 'a':
        return TOKEN_A;
    case 'b':
        return TOKEN_B;
    case 'c':
        return TOKEN_C;
    default:
        return UNKNOWN;
    }
}

std::string tokenTypeName(TokenType type)
{
    switch (type)
    {
    case TOKEN_A:
        return "a";
    case TOKEN_B:
        return "b";
    case TOKEN_C:
        return "c";
    case ENDFILE:
        return "ENDFILE";
    default:
        return "UNKNOWN";
    }
}

Tokenizer::Tokenizer(std::string filename)
    : input(filename), line(1), column(0)
{
}

Tokenizer::~Tokenizer() = default;

bool Tokenizer::isOpen() const
{
    return input.is_open();
}

Token Tokenizer::tokenize()
{
    char ch;

    while (input.get(ch))
    {
        ++column;

        if (ch == ' ' || ch == '\t')
        {
            continue;
        }

        if (ch == '\n')
        {
            ++line;
            column = 0;
            continue;
        }

        return {lookupKeyword(ch), std::string(1, ch), line, column};
    }

    return {ENDFILE, "", line, column + 1};
}
