//
// Created by Lezenn on 13/08/2020.
//

#include <ngl/parser.hpp>

namespace ngl
{
    parser::parser(lexer& lexer)
            : lexer_{ lexer }
    {
        // TODO: Get all shapes kinds from the lexer and cluster shape to compose parser rules

//        const auto& clusters = lexer_.shape_cluster();
//        for (const auto& cluster : clusters)
//        {
//            const auto& shape_datas = cluster.get().datas();
//            for (const auto& sd : shape_datas)
//            {
//                std::cout << sd.name << "\t\t => " << sd.id << std::endl;
//            }
//        }
    }

    void parser::process(const std::string &source)
    {

        // TODO: process different parser rules and get the last matching one then construct the graph
        /*
        lexer_.set_source(source);

        while (lexer_.has_shapes())
        {
            auto shape = lexer_.next_shape();
            lexer_.display(shape);
        }*/
    }
}