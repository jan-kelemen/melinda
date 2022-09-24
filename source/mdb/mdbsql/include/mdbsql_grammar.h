#ifndef MELINDA_MDBSQL_GRAMMAR_INCLUDED
#define MELINDA_MDBSQL_GRAMMAR_INCLUDED

#include <optional>

#include <boost/spirit/include/qi.hpp>

#include <mdbsql_statement.h>

namespace melinda::mdbsql
{
    namespace detail
    {
        namespace qi = ::boost::spirit::qi;
    } // namespace detail

    template<typename Iterator>
    struct [[nodiscard]] statement_parser final
        : detail::qi::grammar<Iterator, statement(), detail::qi::blank_type>
    {
    public:
        statement_parser() : statement_parser::base_type(start)
        {
            database_name %= detail::qi::lexeme[+(detail::qi::alnum)];

            create_statement %=
                detail::qi::lit("CREATE DATABASE ") >> database_name;

            start %= create_statement >> -detail::qi::lit(';');
        }

    private:
        template<typename T, typename Skipper>
        using rule_t = detail::qi::rule<Iterator, T(), Skipper>;

    private:
        rule_t<std::string, detail::qi::blank_type> database_name;
        rule_t<statement::create, detail::qi::blank_type> create_statement;
        rule_t<statement, detail::qi::blank_type> start;
    };

    template<typename Iterator>
    bool
    parse(Iterator first, Iterator last, std::optional<statement>& statement)
    {
        statement_parser<Iterator> parser;

        struct statement rv;

        if (detail::qi::phrase_parse(first,
                last,
                parser,
                detail::qi::blank,
                rv))
        {
            statement = std::move(rv);
            return true;
        }
        return false;
    }
} // namespace melinda::mdbsql

#endif
