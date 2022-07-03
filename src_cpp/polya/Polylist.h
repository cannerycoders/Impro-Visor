#pragma once
// Reference: https://rosettacode.org/wiki/S-expressions#C.2B.2B

#include <iostream>
#include <string>
#include <iomanip> /* std::quoted */
#include <list>
#include <memory> /* std::shared_ptr */
#include <unordered_set>

class Polylist 
{
public: /* builder, serializer */
    class object
    {
    public:
        virtual ~object() {}
        virtual void write(std::ostream&) const {};
        virtual void writeIndented(std::ostream& out, int level) const 
        {
            indent(out, level);
            write(out);
        }
        virtual char const *getType() { return "object"; }
        void indent(std::ostream& out, int level) const
        {
            for (int i = 0; i < level; ++i)
                out << "   ";
        }
    };

    class string : public object
    {
    public:
        explicit string(const std::string& str) : m_string(str) {}
        void write(std::ostream& out) const override { out << std::quoted(m_string); }
        char const *getType() override { return "string"; }
        char const *getValue() const { return m_string.c_str(); }
    private:
        std::string m_string;
    };

    class symbol : public object // todo: tokenize symbols
    {
    public:
        explicit symbol(std::string const& str) 
        {
            m_token = getToken(str);
        }
        void write(std::ostream& out) const override
        {
            int i=0;
            char ch;
            while((ch=m_token[i++]))
            {
                if(tokenizer::GetCharType(ch) != t_Char::other)
                    out << '\\';
                out << ch;
            }
        }
        char const *getType() override { return "symbol"; }
        char const *getValue() const { return m_token; }

        static char const *getToken(std::string const &);

    private:
        char const *m_token;
        static std::unordered_set<std::string> s_tokens;
    };

    class number : public object 
    {
    public:
        explicit number(double num) : m_number(num) {}
        void write(std::ostream& out) const override { out << m_number; }
        char const *getType() override { return "number"; }
        double getValue() { return m_number; }
    private:
        double m_number;
    };

    class integer : public object 
    {
    public:
        explicit integer(long num) : m_number(num) {}
        void write(std::ostream& out) const override { out << m_number; }
        char const *getType() override { return "integer"; }
        long getValue() { return m_number; }
    private:
        long m_number;
    };

    using ObjectPtr = std::shared_ptr<object>;
    using SymbolPtr = std::shared_ptr<symbol>;
    using StringPtr = std::shared_ptr<string>;
    using LongPtr = std::shared_ptr<integer>;
    using NumberPtr = std::shared_ptr<number>;

    using OList = std::list<ObjectPtr>;
    using OListIt = OList::iterator;
    class list : public object 
    {
    public:
        char const *getType() override { return "list"; }
        void write(std::ostream& out) const
        {
            out << "(";
            if (!m_list.empty()) 
            {
                auto i = m_list.begin();
                (*i)->write(out);
                while (++i != m_list.end())
                {
                    out << ' ';
                    (*i)->write(out);
                }
            }
            out << ")";
        }
        void writeIndented(std::ostream& out, int level) const
        {
            indent(out, level);
            out << "(\n";
            if (!m_list.empty()) 
            {
                for(auto i = m_list.begin(); i != m_list.end(); ++i) 
                {
                    (*i)->writeIndented(out, level + 1);
                    out << '\n';
                }
            }
            indent(out, level);
            out << ")";
        }
        void append(const std::shared_ptr<object>& ptr) 
        {
            m_list.push_back(ptr);
        }
        size_t size()
        {
            return m_list.size();
        }
        OListIt getBegin()
        {
            return m_list.begin();
        }
        OListIt getEnd()
        {
            return m_list.end();
        }
        SymbolPtr firstSymbol()
        {
            std::shared_ptr<object> o = *m_list.begin();
            std::shared_ptr<symbol> s = std::dynamic_pointer_cast<symbol>(o);
            if(!s)
                std::cerr << "type of first element " << o->getType() << "\n";
            return s;
        }
        ObjectPtr getNth(unsigned n)
        {
            int i=0;
            auto objIt = m_list.begin();
            while(i != n)
            {
                objIt++;
                i++;
                if(objIt == m_list.end())
                    return ObjectPtr(); // not-found
            }
            return *objIt;
        }

        std::shared_ptr<list> findSublist(char const *tok)
        {
            std::shared_ptr<list> ret;
            for(auto e : m_list)
            {
                std::shared_ptr<list> l = std::dynamic_pointer_cast<list>(e);
                if(l)
                {
                    auto sym = l->firstSymbol();
                    if(sym && sym->getValue() == tok)
                    {
                        ret = l;
                        break;
                    }
                }
            }
            return ret;
        }

    private:
        OList m_list;
    };
    using ListPtr = std::shared_ptr<list>;

    ListPtr Parse(std::string const &str, bool dump=false);
    ListPtr Parse(std::istream &istr, bool dump=false);

private:
    ListPtr m_root;

private: /* tokenizer */
    enum class t_Token 
    { 
        none, 
        left_paren, right_paren, 
        symbol, string, 
        integer, number 
    };
    struct token
    {
        t_Token type = t_Token::none;
        union Number
        {
            double d;
            long l;
        } number;
        std::string s;
    };
    enum class t_Char { left_paren, right_paren, quote, escape, space, other };
    enum class t_ParseState { init, quote, symbol };

    class tokenizer 
    {
    public:
        tokenizer(std::istream& in) : m_in(in) {}
        ~tokenizer() {}
        bool next() 
        {
            if(m_putback)
            {
                m_putback = false;
                return true;
            }
            return GetToken(m_in, m_current);
        }
        const token& current() const 
        {
            return m_current;
        }
        void putback() 
        {
            m_putback = true;
        }
        bool GetToken(std::istream & in, token &tok);

    static t_Char GetCharType(char ch);

    private:
        bool parseNumber(const std::string& str, token& tok);

    private:
        std::istream& m_in;
        bool m_putback = false;
        token m_current;
    };

    std::shared_ptr<object> parse(tokenizer &);
    std::shared_ptr<list> parseList(tokenizer& tok, bool skipOuter=false);

};