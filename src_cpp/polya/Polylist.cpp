// From: https://rosettacode.org/wiki/S-expressions#C.2B.2B
#include "Polylist.h"
#include <fstream>
#include <sstream>
#include <strstream>
#include <string>

std::shared_ptr<Polylist::list> 
Polylist::Parse(std::string const& str, bool dump) 
{
    std::istringstream iss(str);
    return Parse(iss, dump);
}

std::shared_ptr<Polylist::list> 
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
        return parseList(tokenizer);
    default:
        break;
    }
    throw std::runtime_error("syntax error: unexpected token");
}

std::shared_ptr<Polylist::list> 
Polylist::parseList(tokenizer& tok, bool skipOuter) 
{
    std::shared_ptr<list> lst = std::make_shared<list>();
    while (tok.next()) 
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
 
#if 1

int main(int argc, char** argv) 
{
    // std::ifstream is("../../vocab/My.voc", std::ios::binary); // no-comments, but large
    std::ifstream is("../../vocab/My.substitutions", std::ios::binary); // comments
    Polylist plist;
    bool dump(true);
    try 
    {
        plist.Parse(is, dump);
    } 
    catch (const std::exception& ex) 
    {
        std::cerr << ex.what() << '\n';
    }
    return 0;
}

#endif