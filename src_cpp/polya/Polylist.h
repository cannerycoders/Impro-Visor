#pragma once
// Reference: https://rosettacode.org/wiki/S-expressions#C.2B.2B

#include <iostream>
#include <string>
#include <iomanip> /* std::quoted */
#include <list>
#include <memory> /* std::shared_ptr */

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
        void write(std::ostream& out) const { out << std::quoted(m_string); }
    private:
        std::string m_string;
    };

    class symbol : public object
    {
    public:
        explicit symbol(const std::string& str) : m_string(str) {}
        void write(std::ostream& out) const 
        {
            for(char ch : m_string) 
            {
                if(tokenizer::GetCharType(ch) != t_Char::other)
                    out << '\\';
                out << ch;
            }
        }
    private:
        std::string m_string;
    };

    class number : public object 
    {
    public:
        explicit number(double num) : m_number(num) {}
        void write(std::ostream& out) const { out << m_number; }
    private:
        double m_number;
    };

    class integer : public object 
    {
    public:
        explicit integer(long num) : m_number(num) {}
        void write(std::ostream& out) const { out << m_number; }
    private:
        long m_number;
    };

    class list : public object 
    {
    public:
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

    private:
        std::list<std::shared_ptr<object>> m_list;
    };

    std::shared_ptr<list> Parse(std::string const &str, bool dump=false);
    std::shared_ptr<list> Parse(std::istream &istr, bool dump=false);

private:
    std::shared_ptr<list> m_root;

public: /* builder, serializer */

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