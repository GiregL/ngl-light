#include "../lexer.hpp"

TEST(lexer_ngl_shape, ngl_identifier)
{
    // identifier
    {
        ngl::lexer lx{ ngl::ngl_shape_cluster };
        std::string data { "test_id00 _test2 _id3" };
        lx.process(data);
        LX_EXPECT("test_id00", "_test2", "_id3");
    }

    // wrong identifier
    {
        ngl::lexer lx{ ngl::ngl_shape_cluster };
        std::string data { "0test" };
        lx.process(data);
        EXPECT_ANY_THROW("_id3");
    }
}
