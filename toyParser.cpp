#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "toyTokenizer.h"

using namespace std;

class Parser
{
private:
    Tokenizer tokenizer;
    Token currentToken;

    void getNextToken()
    {
        currentToken = tokenizer.tokenize();
    }

    bool parseA(int inputLine)
    {
        // <A> -> a<A> | a

        if (currentToken.type != TOKEN_A || currentToken.line != inputLine)
        {
            return false;
        }

        // Consume one or more a's
        while (currentToken.type == TOKEN_A && currentToken.line == inputLine)
        {
            getNextToken();
        }

        return true;
    }

    bool parseB(int inputLine)
    {
        // <B> -> b<B> | b

        if (currentToken.type != TOKEN_B || currentToken.line != inputLine)
        {
            return false;
        }

        // Consume one or more b's
        while (currentToken.type == TOKEN_B && currentToken.line == inputLine)
        {
            getNextToken();
        }

        return true;
    }

    bool parseC(int inputLine)
    {
        // <C> -> c

        if (currentToken.type != TOKEN_C || currentToken.line != inputLine)
        {
            return false;
        }

        getNextToken();

        return true;
    }

    bool parseS(int inputLine)
    {
        // S -> <A><C><B> | <A>

        // First parse <A>
        if (!parseA(inputLine))
        {
            return false;
        }

        // If there is a c, then we must parse C followed by B
        if (currentToken.type == TOKEN_C && currentToken.line == inputLine)
        {
            if (!parseC(inputLine))
            {
                return false;
            }

            if (!parseB(inputLine))
            {
                return false;
            }
        }

        return true;
    }

public:
    Parser(string filename)
        : tokenizer(filename)
    {
        currentToken.type = UNKNOWN;
        currentToken.lexeme = "";
        currentToken.line = 0;
        currentToken.column = 0;
    }

    bool inputIsOpen() const
    {
        return tokenizer.isOpen();
    }

    vector<pair<int, bool>> parse()
    {
        vector<pair<int, bool>> results;
        getNextToken();

        while (currentToken.type != ENDFILE)
        {
            int inputLine = currentToken.line;
            bool valid = parseS(inputLine);

            if (currentToken.type != ENDFILE && currentToken.line == inputLine)
            {
                valid = false;
            }

            results.emplace_back(inputLine, valid);

            while (currentToken.type != ENDFILE && currentToken.line == inputLine)
            {
                getNextToken();
            }
        }

        return results;
    }
};

int main(int argc, char* argv[])
{
    string filename;

    if (argc > 1)
    {
        filename = argv[1];
    }
    else
    {
        cout << "Enter input file name: ";
        cin >> filename;
    }

    Parser parser(filename);

    if (!parser.inputIsOpen())
    {
        cerr << "Could not open input file: " << filename << endl;
        return 1;
    }

    vector<pair<int, bool>> results = parser.parse();
    if (results.empty())
    {
        cout << "No test cases found." << endl;
        return 0;
    }

    for (const auto& result : results)
    {
        cout << "Line " << result.first << ": "
             << (result.second ? "VALID" : "INVALID") << endl;
    }

    return 0;
}