#pragma once
// Reference: https://rosettacode.org/wiki/S-expressions#C.2B.2B

#include "tokenizer.h"

#include <iostream>
#include <string>
#include <iomanip> /* std::quoted */
#include <list>
#include <memory> /* std::shared_ptr */
#include <unordered_set>

/**
 * @brief base class for contents of Polylist
 * 
 */
class PListObj
{
public:
    enum Type
    {
        k_object,
        k_symbol,
        k_string,
        k_number,
        k_integer,
        k_list,
        k_invalid
    };

    virtual ~PListObj() {}
    virtual void write(std::ostream&) const {};
    virtual void writeIndented(std::ostream& out, int level) const 
    {
        indent(out, level);
        write(out);
    }
    virtual char const *getTypeName() { return "object"; }
    virtual Type getType() { return k_object;  } 
    virtual std::string asString() { return std::string(); }
    void indent(std::ostream& out, int level) const
    {
        for (int i = 0; i < level; ++i)
            out << "   ";
    }
    void *asType(Type t)
    {
        if(this->getType() == t) 
            return this;
        else 
            return nullptr;
    }
};

/**
 * @brief string entry in polylist
 * 
 */
class PListString : public PListObj
{
public:
    explicit PListString(const std::string& str) : m_string(str) {}
    void write(std::ostream& out) const override { out << std::quoted(m_string); }
    char const *getTypeName() override { return "string"; }
    Type getType() override { return k_string; }
    std::string asString() override { return m_string; }
    char const *getValue() const { return m_string.c_str(); }
private:
    std::string m_string;
};

/**
 * @brief symbol entry in polylist.  Symbols are analogous to contants
 *    and are compared by "tokenized" pointers for efficiency.
 * 
 */
class PListSymbol : public PListObj
{
public:
    explicit PListSymbol(std::string const& str) 
    {
        m_token = getSymbol(str);
    }
    void write(std::ostream& out) const override
    {
        int i=0;
        char ch;
        while((ch=m_token[i++]))
        {
            if(PListTokenizer::GetCharType(ch) != PListTokenizer::t_Char::other)
                out << '\\';
            out << ch;
        }
    }
    char const *getTypeName() override { return "symbol"; }
    Type getType() override { return k_symbol; }
    std::string asString() override { return std::string(m_token); }
    char const *getValue() const { return m_token; }

    static char const *getSymbol(std::string const &); // aka tokenize

private:
    char const *m_token;
    static std::unordered_set<std::string> s_tokens;
};

/**
 * @brief floating point number entries in Polylist
 * 
 */
class PListNumber : public PListObj
{
public:
    explicit PListNumber(double num) : m_number(num) {}
    void write(std::ostream& out) const override { out << m_number; }
    char const *getTypeName() override { return "number"; }
    Type getType() override { return k_number; }
    std::string asString() override 
    { 
        return std::to_string(m_number); 
    }
    double getValue() { return m_number; }

private:
    double m_number;
};

/**
 * @brief integral number entries in Polylist
 * 
 */
class PListLong : public PListObj
{
public:
    explicit PListLong(long num) : m_number(num) {}
    void write(std::ostream& out) const override { out << m_number; }
    char const *getTypeName() override { return "integer"; }
    Type getType() override { return k_integer; }
    std::string asString() override 
    { 
        return std::to_string(m_number); 
    }
    long getValue() { return m_number; }
private:
    long m_number;
};


/* ------------------------------------------------------------------- */
/**
 * @brief  Polylist entry in polylist.
 * 
 */
class Polylist : public PListObj
{
public:
    using ObjPtr = std::shared_ptr<PListObj>;
    using ObjList = std::list<ObjPtr>; // xxx: deque or vector might be preferred?
    using ObjListIt = ObjList::iterator;
    using Ptr = std::shared_ptr<Polylist>;

private:
    ObjList m_list;

public: /* builder, serializer */
    static Polylist::Ptr Make() { return std::make_shared<Polylist>(); }

    Polylist();
    virtual ~Polylist();
    /** 
     * constructor via a list of symbols 
     */
    Polylist(std::vector<std::string> &values)
    {
        for(auto x : values)
            this->addSymbol(x);
    }

    /**
     * @brief Parse a string containing plist serialization.
     * 
     * @param str 
     * @param dump signals to print results
     * @return int 0 on non-empty result, 1 on empty.
     */
    int Parse(std::string const &str, bool dump=false);
    /**
     * @brief  Parse a istream containing plist serialization.
     * 
     * @param istr 
     * @param dump signals to print results
     * @return int 
     */
    int Parse(std::istream &istr, bool dump=false);

    /* PListObj methods (custom method in next section) ----------------- */
    char const *getTypeName() override { return "plist"; }
    Type getType() override { return k_list; }
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

    /**
     * @brief  recursively serialize the Plist
     * 
     * @param out 
     * @param level 
     */
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
        if(level == 0)
            out << "\n";
    }

    /* custom methods ------------------------------------------------- */
    void append(const ObjPtr & ptr) 
    {
        m_list.push_back(ptr);
    }
    void addSymbol(const std::string &val)
    {
        m_list.push_back(std::make_shared<PListSymbol>(val));
    }
    size_t size()
    {
        return m_list.size();
    }
    bool isValid()  { return !isEmpty(); }
    bool isEmpty() 
    { 
        if(m_list.size() > 0) 
            return false;
        else
            return true;
    }
    ObjListIt getBegin()
    {
        return m_list.begin();
    }
    ObjListIt getEnd()
    {
        return m_list.end();
    }
    PListSymbol const *firstSymbol()
    {
        ObjPtr o = *m_list.begin();
        PListSymbol const *s = static_cast<PListSymbol *>(o->asType(k_symbol));
        if(!s)
            std::cerr << "type of first element " << o->getType() << "\n";
        return s;
    }
    PListSymbol const *findSymbol(char const *tok)
    {
        auto objIt = m_list.begin();
        while(objIt != m_list.end())
        {
            PListSymbol const *s = static_cast<PListSymbol *>((*objIt)->asType(k_symbol));
            if(s) return s;
            objIt++;
        }
        return nullptr;
    }
    ObjPtr first() { return getNth(0); }
    ObjPtr second() { return getNth(1); }
    ObjPtr third() { return getNth(2); }
    ObjPtr fourth() { return getNth(3); }

    ObjPtr getNth(unsigned n)
    {
        int i=0;
        auto objIt = m_list.begin();
        while(i != n)
        {
            objIt++;
            i++;
            if(objIt == m_list.end())
                return ObjPtr(); // not-found
        }
        return *objIt;
    }

    /** 
     * return the Polylist child of this whose first field matches tok
     */
    Polylist *findSublist(char const *tok, int debug=0)
    {
        for(auto e : m_list)
        {
            Polylist *l = static_cast<Polylist*>(e->asType(k_list));
            if(l)
            {
                auto sym = l->firstSymbol();
                if(sym && sym->getValue() == tok)
                {
                    return l;
                }
            }
            else
            {
                if(debug)
                {
                    std::cerr << "sublist is-a " << e->getType() 
                        << ": " << e->asString() << "\n";
                }
            }
        }
        return nullptr;
    }

private: /* tokenizer */
    ObjPtr parse(PListTokenizer &);
    ObjPtr parseList(PListTokenizer& tok, bool skipOuter=false);
    size_t m_iId;
    static size_t s_iCounter;
}; // end Polylist