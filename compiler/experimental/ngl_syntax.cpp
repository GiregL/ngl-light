#include <ngl/lexer.hpp>
#include <ngl/shape_cluster.hpp>

int main()
{
    try
    {
        //ngl::lexer lx{ ngl::ngl_shape_cluster };
        ngl::shape_cluster shapes;

        auto letter = shapes.add_fragment(ngl::shape_range('a', 'z'));
        auto digit = shapes.add_fragment(ngl::shape_range('0', '9'));
        auto colon = shapes.add_fragment(ngl::shape_element(':'));
        auto underscore = shapes.add(ngl::shape_element('_'));
        auto identifier_symbol = shapes.add_fragment(ngl::shape_or(letter));
        auto many_identifier_symbol = shapes.add_fragment(ngl::shape_many(identifier_symbol));

        auto raw_identifier = shapes.add(ngl::shape_sequence(letter, many_identifier_symbol));
        auto path_identifier = shapes.add(ngl::shape_sequence(raw_identifier, colon, raw_identifier));

        shapes.display();

        ngl::lexer lx{ shapes };

        /*
        std::string data = R"(
        ngl:shape scalar_description
        {
            ngc:sequence<ngs:identifier ngs:identifier>
        }
        )";*/

        std::string data = "ngl:concept:container";

        lx.process(data);
        std::cout << "\n" << lx.to_string();

    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
    }

    return 0;
}

/*
L, NB, L
n 0001 I0 S M1 I++
0 0010 I1 V M1
0 0010 I1 V M1
n 0001 I1 V M0
                I++ I2 S M1*/