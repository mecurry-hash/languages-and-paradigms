#pragma once

#include <fstream>
#include <string>

enum TokenType
{
    TOKEN_A,
    TOKEN_B,
    TOKEN_C,
    ENDFILE,
    UNKNOWN
};

struct Token
{
    TokenType type;
    std::string lexeme;
    int line;
    int column;
};

std::string tokenTypeName(TokenType type);
TokenType lookupKeyword(char ch);

class Tokenizer
{
private:
    std::ifstream input;
    int line;
    int column;

public:
    explicit Tokenizer(std::string filename);
    ~Tokenizer();

    bool isOpen() const;
    Token tokenize();
};