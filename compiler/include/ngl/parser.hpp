#ifndef COMPILER_INCLUDE_NGL_PARSER_HPP_NGL
#define COMPILER_INCLUDE_NGL_PARSER_HPP_NGL

#include <ngl/lexer.hpp>
#include <ngl/log.hpp>

namespace ngl
{
    using token_kind = int;

    /**
     * Generic representation of a rule of parsing
     */
    struct parser_rule
    {
        virtual bool verify(/* TODO: */) = 0;
    };

    struct parser_sequence : public parser_rule
    {
        std::vector<token_kind> predicate;
    };

    struct parser_many : public parser_rule
    {

    };

    class parser
    {
    public:
        explicit parser(ngl::lexer& lexer);

        void process(const std::string& source);

    private:
        ngl::lexer& lexer_;
        std::vector<parser_rule> rules_;
        std::vector<token_kind> kinds_;
    };
} // ngl

#endif // COMPILER_INCLUDE_NGL_PARSER_HPP_NGL
