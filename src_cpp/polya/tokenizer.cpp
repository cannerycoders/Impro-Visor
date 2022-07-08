#include "tokenizer.h"

#include <cctype>

bool 
PListTokenizer::GetToken(std::istream & in, token &tok)
{
    char ch;
    t_ParseState state = t_ParseState::init;
    bool escape = false;
    std::string str;
    t_Token type = t_Token::none;
    while(in.get(ch)) 
    {
        t_Char ctype = GetCharType(ch);
        if(escape) 
        {
            ctype = t_Char::other;
            escape = false;
        } 
        else 
        if(ctype == t_Char::escape) 
        {
            escape = true;
            continue;
        }
        if(state == t_ParseState::quote)
        {
            if(ctype == t_Char::quote) 
            {
                type = t_Token::string;
                break;
            }
            else
                str += ch;
        } 
        else 
        if(state == t_ParseState::symbol)  
        {
            /* symbols are collected 'til space, quote, paren */
            if(ctype == t_Char::space)
                break;
            if(ctype != t_Char::other) 
            {
                in.putback(ch);
                break;
            }
            str += ch;
        }
        else
        if(ctype == t_Char::quote)
        {
            state = t_ParseState::quote;
        }
        else
        if(ctype == t_Char::other) 
        {
            if(ch == '/') /* we're not in symbol, nor quote */
            {
                in.get(ch); 
                if(ch == '*')
                {
                    /* block comment, look for 2char termination */
                    char prev = 0;
                    while(in.get(ch))
                    {
                        if(ch == '/' && prev == '*')
                            break; // <-----------
                        else
                        {
                            if(ch == '\r')
                            {
                                // one line
                                in.get(ch);
                                if(ch == '\n')
                                    in.get(ch);
                            }
                            else
                            if(ch == '\n')
                            {
                                // one line
                                in.get(ch);
                            }
                        }
                        if(ch) // 0 is EOF
                            prev = ch;
                        else
                            return false;
                    }
                    return GetToken(in, tok);
                }
                else
                if(ch == '/')
                {
                    /* line comment, look for eol */
                    while(in.get(ch))
                    {
                        if(ch == '\n' || ch == '\r')
                            break;
                    }
                    if(ch == 0) // 0 is EOF
                        return false;
                    else
                        return GetToken(in, tok);
                }
                else
                    in.putback(ch);
            }
            state = t_ParseState::symbol;
            type = t_Token::symbol;
            str = ch;
        }
        else
        if(ctype == t_Char::left_paren)
        {
            type = t_Token::left_paren;
            break;
        }
        else 
        if (ctype == t_Char::right_paren) 
        {
            type = t_Token::right_paren;
            break;
        }
    }
    if(type == t_Token::none)
    {
        if(state == t_ParseState::quote)
            throw std::runtime_error("syntax error: missing quote");
        return false;
    }
    tok.type = type;
    if(type == t_Token::string)
        tok.s = str;
    else 
    if(type == t_Token::symbol) 
    {
        if(!parseNumber(str, tok))
            tok.s = str;
    }
    return true;
}

PListTokenizer::t_Char 
PListTokenizer::GetCharType(char ch)
{
    switch (ch) 
    {
    case '(':
        return t_Char::left_paren;
    case ')':
        return t_Char::right_paren;
    case '"':
        return t_Char::quote;
    case '\\':
        return t_Char::escape;
    default:
        if(isspace(static_cast<unsigned char>(ch)))
            return t_Char::space;
        else
            return t_Char::other;
    }
}

bool 
PListTokenizer::parseNumber(const std::string& str, token& tok) 
{
    if(!isdigit(str[0]) && str[0] != '.' && str[0] != '-') 
        return false; // quick early-out
    if(str.find(".") == std::string::npos)
    {
        size_t pos = 0;
        try
        {
            long num = std::stol(str, &pos);
            if(pos == str.size()) 
            {
                tok.type = t_Token::integer;
                tok.number.l = num;
                return true;
            }
        }
        catch(std::exception const& )
        {
            // std::cout << str << " " << ex.what() << '\n';
            return false;
        }
    }
    else
    {
        try
        {
            size_t pos = 0;
            double num = std::stod(str, &pos);
            if(pos == str.size()) 
            {
                tok.type = t_Token::number;
                tok.number.d = num;
                return true;
            }
        }
        catch(std::exception const& )
        {
            // '.../F'
            // std::cout << str << " " << ex.what() << '\n';
            return false;
        }
    }
    #if 0
    std::cerr << "NaN:" << str << "\n";
    #endif
    return false;
}
