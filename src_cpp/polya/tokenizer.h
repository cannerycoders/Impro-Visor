    #pragma once

    #include <string>

    class PlistTokenizer 
    {
    public:
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

    public:
        PlistTokenizer(std::istream& in) : m_in(in) {}
        ~PlistTokenizer() {}
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