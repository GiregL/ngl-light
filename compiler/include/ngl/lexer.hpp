#ifndef COMPILER_INCLUDE_NGL_LEXER_HPP_NGL
#define COMPILER_INCLUDE_NGL_LEXER_HPP_NGL

#include <ngl/shape.hpp>

#include "shape_cluster.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace ngl
{
    struct location
    {
        size_t origin;
        size_t size;
    };

    struct shape
    {
        uint64_t id{};
        std::string name;
        ngl::location location{};
    };

    class lexer
    {
    public:
        using element_type = char;

        explicit lexer();
        explicit lexer(ngl::shape_cluster);

        void process();
        void process(std::string_view);
        void process(const char*) = delete;

        void parse();

        void process_v2(const std::string&);
        void asm_process();

        void add(ngl::shape_cluster&);
        void add_shape(uint64_t shape_id, const std::string& name, ngl::location);

        std::string_view display(const ngl::shape&);
        void display();

        void reset();

        [[nodiscard]] const std::vector<ngl::shape_cluster>& shape_cluster() const;

        [[nodiscard]] std::string_view data() const;

        [[nodiscard]] const std::vector<shape>& shapes() const;
        [[nodiscard]] std::string_view shape_view(int index) const;

        [[nodiscard]] std::string to_string(const shape&) const;
        [[nodiscard]] std::string to_string() const;

    private:
        std::string_view data_;
        std::vector<shape> shapes_;
        std::vector<ngl::shape_cluster> shape_clusters_;

        unsigned int parser_cursor_ = 0;
    };
} // ngl

#endif // COMPILER_INCLUDE_NGL_LEXER_HPP_NGL
