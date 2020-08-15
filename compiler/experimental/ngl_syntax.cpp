#include <ngl/lexer.hpp>
#include <ngl/shape_cluster.hpp>

int main()
{
    try
    {
        ngl::ngl_shape_cluster ngl_shape;


        ngl::lexer lx{ ngl_shape };

        std::string data = R"(
        ngl:shape scalar_description
        {
            ngc:sequence<ngs:identifier ngs:identifier>
        }
)";

        lx.process(data);
        std::cout << lx.to_string();

    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
    }

    return 0;
}