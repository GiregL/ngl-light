#include <ngl/lexer.hpp>
#include <ngl/shape_cluster.hpp>

int main()
{
    try
    {
        //ngl::lexer lx{ ngl::ngl_shape_cluster };
        ngl::shape_cluster sh;

        //sh.add(ngl::shape_space(' '));
        auto L = sh.add(ngl::shape_range('a', 'z'), "L");
        auto D = sh.add(ngl::shape_range('0', '9'), "D");

        //auto number = sh.add(ngl::shape_many(D), "number");
        //auto word = sh.add(ngl::shape_many(L), "word");


        //auto s = sh.add(ngl::shape_sequence(L, D));
        sh.display();

        ngl::lexer lx{ sh };

        /*
        std::string data = R"(
        ngl:shape scalar_description
        {
            ngc:sequence<ngs:identifier ngs:identifier>
        }
        )";*/

        std::string data = "ngl012";

        lx.process(data);
        std::cout << "\n" << lx.to_string();

    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
    }

    return 0;
}