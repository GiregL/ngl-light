#include <ngl/lexer.hpp>
#include <ngl/shape_cluster.hpp>

int main()
{
    try
    {

        ngl::shape_cluster parser_shapes;
        ngl::shape_cluster shapes;
        auto letter = shapes.add(ngl::shape_element('n'));
        auto digit = shapes.add(ngl::shape_element('0'));
        auto underscore = shapes.add(ngl::shape_element('_'));

        auto digits = shapes.add(ngl::shape_many(digit));

        auto seq = shapes.add(ngl::shape_sequence(letter, underscore, underscore));


        ngl::lexer lx{ shapes };

        //std::string data = "n_0n_000"; n_0, n_0
        std::string data = "n__nn"; // n, _, _

        shapes.display();
        lx.process(data);


        std::cout << "\n\nCPP lexer\n";
        std::cout << lx.to_string();

        //
        std::cout << "\nASM lexer\n";
        lx.asm_process();
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
    }

    return 0;
}


/*
identifier: (letter | cap_letter) id_symbol* ^[a-zA-Z_][a-zA-Z0-9_\-?']*
operator: ^(\+|\-|\*|/|<=|>=|!=|<|>|@=|@|=|\^)
capture: ^&[a-zA-Z_][a-zA-Z0-9_\-?']*
getfield: ^\.[a-zA-Z_][a-zA-Z0-9_\-?']*
skip: ^[\s]+
comment: # any
shorthand: ^[']
 */



/*
    I: index
    IM: index_match
    NM: next IM
    LI: last index
    LS: last shape
    VID: vector id

    # seq<_ ng _>
    _ I: 0 IM: 1 PM: 0 NM: 0 M: 1
        n I: 0 IM: 0 PM: 1 NM: 1 M: 0 -> if !IM && PM -> I = 1 IM = 1 M = 1
    n I: 1 IM: 1 PM: 1 NM: 0 M: 1
    g I: 1 IM: 1 PM: 1 NM: 0 M: 1
        _ I: 1 IM: 0 PM: 1 NM: 1 M: 0 -> if !IM && PM -> I = 2 IM = 1 M = 1
    _ I: 2 IM: 1 PM: 1 NM: 0 M: 1

    #end1 I == LI && is_scalar<LS>               -> I = 0 VID = ~VID
    #end2 I == LI && !is_scalar<LS> && NM == 0   -> I = 0 VID = ~VID


    Size S
    False F
    True T
    Index I

    I = I + (((TF & 1u) ^ 1u) & ((TF & 2u) >> 1)) % S;




G (100)

ngl
    add_shape (101)
00
    add_shape (101)

NGL
    add_shape (110)
00
    add_shape (110)

100 : desc = ngl::description
001 : sd = ngl::scalar_description

101 : sd = ngl::scalar_description
        .add(ngl)
        .add(00)
desc.add(sc)

110 : vd = ngl::vector_description
        .add(ngl)
        .add(00)
desc.add(sc)
 ...


   desc
   /  \
  SC  VD
/  \  /  \
ID  ID ID ID

*/
