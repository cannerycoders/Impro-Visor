// From: https://rosettacode.org/wiki/S-expressions#C.2B.2B
#include "Polylist.h"
#include <fstream>
#include <sstream>
#include <strstream>
#include <string>

Polylist::ListPtr
Polylist::Parse(std::string const& str, bool dump) 
{
    std::istringstream iss(str);
    return Parse(iss, dump);
}

Polylist::ListPtr
Polylist::Parse(std::istream& istr, bool dump) 
{
    tokenizer tokenizer(istr);
    m_root = parseList(tokenizer, true/*skipOuter*/);
    if(m_root != nullptr) 
    {
        std::cout << "polylist with " << m_root->size() << " elements\n";
        if(dump)
        {
            m_root->writeIndented(std::cout, 0);
            std::cout << '\n';
        }
    }
    return m_root;
}

std::shared_ptr<Polylist::object> 
Polylist::parse(tokenizer& tokenizer) 
{
    if (!tokenizer.next())
        return nullptr;
    const token& tok = tokenizer.current();
    switch (tok.type) 
    {
    case t_Token::string:
        return std::make_shared<string>(tok.s);
    case t_Token::symbol:
        return std::make_shared<symbol>(tok.s);
    case t_Token::number:
        return std::make_shared<number>(tok.number.d);
    case t_Token::integer:
        return std::make_shared<integer>(tok.number.l);
    case t_Token::left_paren:
        return parseList(tokenizer, false/*means require right-paren*/);
    default:
        break;
    }
    throw std::runtime_error("syntax error: unexpected token");
}

std::shared_ptr<Polylist::list> 
Polylist::parseList(tokenizer& tok, bool skipOuter) 
{
    std::shared_ptr<list> lst = std::make_shared<list>();
    while(tok.next()) 
    {
        if (tok.current().type == t_Token::right_paren)
            return lst;
        else
            tok.putback();
        lst->append(parse(tok));
    }
    if(!skipOuter)
        throw std::runtime_error("syntax error: unclosed list");
    else
        return lst;
}
 
 /* ------------------------------------------------------------- */

std::unordered_set<std::string> Polylist::symbol::s_tokens;

/*static*/ char const *
Polylist::symbol::getToken(std::string const &str)
{
    auto x = s_tokens.find(str);
    if(x == s_tokens.end())
    {
        s_tokens.insert(str);
        x = s_tokens.find(str);
    }
    return x->c_str();
}