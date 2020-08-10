#ifndef COMPILER_INCLUDE_NGL_CONCRETIZER_HPP_NGL
#define COMPILER_INCLUDE_NGL_CONCRETIZER_HPP_NGL

#include "lang.hpp"

namespace ngl
{

    class concretizer
    {
    public:
        concretizer() = default;
        virtual ~concretizer() = default;
        virtual void process(ngl::lang::identifier&) = 0;
    };


    class llvm_concretizer : public concretizer
    {
        void process(ngl::lang::identifier& id) override
        {
            //auto id_name = id.name();
            //std::cout << "\nProcess: " << id_name;
        }
    };
} // ngl

#endif // COMPILER_INCLUDE_NGL_CONCRETIZER_HPP_NGL
