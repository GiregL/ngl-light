#include <nds/graph.hpp>
#include <ngl/cluster.hpp>
#include <nds/encoder/graph.hpp>

namespace nds::encoders
{
    template<> template<>
    inline std::string dot<>::node_name<std::string>(const std::string& v)
    {
        return v;
    }
}

using namespace std::string_literals;

int main()
{
    /*
     * This code is a graph representation of the sample program
     */

    ngl::cluster cluster{ "cluster", "" };
    auto& g = cluster.graph();

    // ngl.ngl and builtin

    auto ngl = g.add("ngl"s);
    auto nge = g.add("ngl:environment"s, ngl);
    auto ngstor = g.add("ngl:storage"s, ngl);
    auto ngd = g.add("ngl:data"s, ngl);
    auto ngentity = g.add("ngl:entity"s, ngl);
    auto ngc = g.add("ngl:concept"s, ngl);
    auto ngi = g.add("ngl:instruction"s, ngl);
    auto ngs = g.add("ngl:shape"s, ngl);

    auto ngc_number = g.add("ngl:concept:number"s, ngc);

    // shape.ngl

    auto ngs_plus = g.add("ngl:shape:plus"s, ngs);
    auto ngs_plus_concr = g.add("(concrete data) [+]"s, ngs_plus);
    auto ngs_add = g.add("ngl:shape:add"s, ngs);
    auto ngs_add_seq = g.add("ngl:shape:sequence"s, ngs_add);
    auto ngs_add_seq_id1 = g.add("ngl:shape:identifier"s, ngs_add_seq);
    auto ngs_add_seq_plus = g.add("ngl:shape:plus"s, ngs_add_seq);
    auto ngs_add_seq_id2 = g.add("ngl:shape:identifier"s, ngs_add_seq);

    // program.ngl

    auto ngc_instruction_add = g.add("ngl:instruction:add"s, ngi);
    auto ngc_function = g.add("ngl:concept:function"s, ngc);
    auto ngc_function_add = g.add("ngl:concept:function:add"s, ngc_function);
    auto ngc_function_add_a = g.add("a"s, ngc_function_add);
    auto ngc_function_add_b = g.add("b"s, ngc_function_add);
    auto ngc_function_result = g.add(".result"s, ngc_function);
    g.connect(ngc_function_result, ngc_instruction_add);
    g.connect(ngc_function_result, ngc_function_add_a);
    g.connect(ngc_function_result, ngc_function_add_b);
    g.connect(ngc_function_add, ngd);
    g.connect(ngc_function_add, ngd);


    auto ngc_add = g.add("ngl:concept:add"s, ngc);
    auto ngc_add_result = g.add(".result"s, ngc_add);
    g.connect(ngc_add, ngd);
    g.connect(ngc_add, ngd);
    g.connect(ngc_add_result, ngc_function_add);


    auto program = g.add("ngl:program"s, ngl);
    auto number_a = g.add("a"s, program);
    g.connect(number_a, ngc_number);
    auto number_concrete_a_1 = g.add("1"s, number_a);

    auto number_b = g.add("b"s, program);
    g.connect(number_b, ngc_number);
    auto number_concrete_b_2 = g.add("1"s, number_b);

    auto add_call = g.add("ngl:program:call_add_1"s, program);
    g.connect(add_call, number_a);
    g.connect(add_call, number_b);
    g.connect(add_call, ngc_add);

    // ...

//    auto ngc_add = g.add("ngc:add"s, ngc);
//    auto ngi_add = g.add("ngi:add"s, ngi);
//    auto ngs_add = g.add("ngs:add(id + id)"s, ngs);
//
//    auto id1 = g.add("id_1"s, ngs_add);
//    auto id2 = g.add("id_2"s, ngs_add);


//    g.connect(ngc_add, ngi_add);
//
//    g.connect(ngs_add, ngc_add);
//    g.connect(ngc_add, ngs_add);



    cluster.process();

    nds::encoders::dot<>::encode<nds::console>(cluster.graph());

    return 0;
}
