// From: https://rosettacode.org/wiki/S-expressions#C.2B.2B
#include "Polylist.h"
#include <fstream>
#include <sstream>
#include <strstream>
#include <string>

size_t Polylist::s_iCounter = 0;

Polylist::Polylist()
{
    m_iId = s_iCounter++; // for memory debuggin
}

Polylist::~Polylist()
{
    // std::cerr << "Adios " << m_iId << "\n";
}

int
Polylist::Parse(std::string const& str, bool dump) 
{
    std::istringstream iss(str);
    return Parse(iss, dump);
}

int
Polylist::Parse(std::istream& istr, bool dump) 
{
    PlistTokenizer tokenizer(istr);
    parseList(tokenizer, true/*skipOuter*/);
    std::cout << "polylist with " << this->size() << " elements\n";
    if(dump)
    {
        this->writeIndented(std::cout, 0);
        std::cout << '\n';
    }
    return m_list.size() > 0;
}

Polylist::ObjPtr
Polylist::parse(PlistTokenizer& tokenizer) 
{
    if(tokenizer.next())
    {
        const PlistTokenizer::token& tok = tokenizer.current();
        switch(tok.type) 
        {
        case PlistTokenizer::t_Token::string:
            return std::make_shared<PlistString>(tok.s);
        case PlistTokenizer::t_Token::symbol:
            return std::make_shared<PlistSymbol>(tok.s);
        case PlistTokenizer::t_Token::number:
            return std::make_shared<PlistFloat>(tok.number.d);
        case PlistTokenizer::t_Token::integer:
            return std::make_shared<PlistInt>(tok.number.l);
        case PlistTokenizer::t_Token::left_paren:
            return parseList(tokenizer, false/*means require right-paren*/);
        default:
            break;
        }
    }
    return ObjPtr();
}

Polylist::ObjPtr
Polylist::parseList(PlistTokenizer&tok, bool skipOuter) 
{ 
    Polylist *pl;
    ObjPtr ret; // only when we allocate a new one
    bool closed = false;
    if(skipOuter)
        pl = this;
    else
    {
        pl = new Polylist();
        ret = ObjPtr(pl);
    }
    while(tok.next()) 
    {
        if(tok.current().type == PlistTokenizer::t_Token::right_paren)
        {
            closed = true;
            break;
        }
        else
            tok.putback();
        pl->append(parse(tok));
    }
    if(!skipOuter && !closed)
        throw std::runtime_error("syntax error: unclosed plist");
    return ret;
}
 
 /* ------------------------------------------------------------- */

std::unordered_set<std::string> PlistSymbol::s_tokens;

/*static*/ char const *
PlistSymbol::getSymbol(std::string const &str)
{
    auto x = s_tokens.find(str);
    if(x == s_tokens.end())
    {
        s_tokens.insert(str);
        x = s_tokens.find(str);
    }
    return x->c_str();
}