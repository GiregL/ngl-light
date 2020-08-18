#ifndef COMPILER_INCLUDE_NGL_CLUSTER_HPP_NGL
#define COMPILER_INCLUDE_NGL_CLUSTER_HPP_NGL

#include <ngl/parser.hpp>
#include <ngl/lexer.hpp>
#include <ngl/graph.hpp>
#include "lang.hpp"

namespace ngl
{
    class cluster
    {
    public:
        cluster(std::string name, std::string source);

        void process();

        ngl::graph& graph();
        ngl::node_ptr<std::string> root();
        ngl::node_ptr<std::string> node();

    private:
        std::string name_;
        std::string source_;
        ngl::lexer lexer_;
        ngl::parser parser_;
        ngl::graph graph_;
        ngl::node_ptr<std::string> root_;
        ngl::node_ptr<std::string> node_;
    };
} // ngl

#endif // COMPILER_INCLUDE_NGL_CLUSTER_HPP_NGL
