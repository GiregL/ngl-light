#include <ngl/shape_cluster.hpp>

#include <bitset>
#include <iostream>

namespace ngl
{
    const std::string& shape_cluster::name() const { return name_; }
    std::vector<ngl::shape_data>& shape_cluster::datas() { return shape_datas_; }
    uint64_t shape_cluster::scalar_shapes_count() const { return scalar_shapes_; }
    uint64_t shape_cluster::vector_shapes_count() const { return vector_shapes_; }

    void shape_cluster::display() const
    {
        for (int i = 0; i < shape_datas_.size(); ++i)
        {
            std::cout << "index " << shape_datas_[i].index
                      << " | id " << std::bitset<16>{ shape_datas_[i].id }
                      << " | vector_id " << std::bitset<16>{ shape_datas_[i].vector_id }
                      << " | data " << std::bitset<16>{ shape_datas_[i].data }
                      << " | " << shape_datas_[i].name << "\n";
        }
        std::cout << std::endl;
    }

    ngl::shape_data shape_cluster::add(ngl::shape_type shape_type, std::vector<uint64_t> data, const std::string& name, bool is_parser)
    {
        vector_datas_.push_back(std::make_unique<std::vector<uint64_t>>(std::move(data)));

        ngl::shape_data shape;

        shape.index = shape_data_index_++;
        shape.id = 1u << shape.index;
        shape.type = static_cast<uint64_t>(shape_type);
        shape.data = reinterpret_cast<uint64_t>(std::addressof(*(vector_datas_.back())));
        shape.name = name;

        //for (const auto& sh_id : vector_datas_.back()) shape.vector_id |= sh_id;
        //shape.vector_id |= shape.id;
        uint64_t vector_state_mask{ ~uint64_t(0) << scalar_shapes_ };
        shape.vector_id = shape.id & vector_state_mask;

        shape_datas_.push_back(shape);

        return shape;
    }

    ngl::shape_data shape_cluster::add(ngl::shape_type shape_type, char data, const std::string& name, bool is_parser)
    {
        return add(shape_type, static_cast<uint64_t>(data), name, is_parser);
    }

    ngl::shape_data shape_cluster::add(ngl::shape_type shape_type, uint64_t data, const std::string& name, bool is_parser)
    {
        ngl::shape_data shape;

        switch (shape_type)
        {
        case ngl::shape_type::scalar_element:
        case ngl::shape_type::scalar_element_vector:
        case ngl::shape_type::scalar_range:
            scalar_shapes_++;
            break;

        case ngl::shape_type::vector_many:
        case ngl::shape_type::vector_sequence:
            vector_shapes_++;
            break;
        [[fallthrough]];
        default:;
        }

        shape.index = shape_data_index_++;
        shape.id = 1u << shape.index;
        uint64_t vector_state_mask{ ~uint64_t(0) << scalar_shapes_ };
        shape.vector_id = shape.id & vector_state_mask;
        shape.type = static_cast<uint64_t>(shape_type);
        shape.data = data;
        shape.name = name;

        shape.is_parser = is_parser;

        if (shape.is_parser) parser_shape_state_ |= shape.id;

        shape_datas_.push_back(shape);
        return shape;
    }

    ngl::shape_data shape_cluster::add(ngl::shape_element element, const std::string& name, bool is_parser)
    {
        return add(shape_type::scalar_element, element.data, name, is_parser);
    }
    ngl::shape_data shape_cluster::add(ngl::shape_element_vector element, const std::string& name, bool is_parser)
    {
        return add(shape_type::scalar_element_vector, element.data, name, is_parser);
    }
    ngl::shape_data shape_cluster::add(ngl::shape_or or_, const std::string& name, bool is_parser)
    {
        return add(shape_type::logical_or, or_.data, name, is_parser);
    }
    ngl::shape_data shape_cluster::add(ngl::shape_range range, const std::string& name, bool is_parser)
    {
        return add(shape_type::scalar_range, range.data, name, is_parser);
    }
    ngl::shape_data shape_cluster::add(ngl::shape_many many, const std::string& name, bool is_parser)
    {
        return add(shape_type::vector_many, many.data, name, is_parser);
    }
    ngl::shape_data shape_cluster::add(ngl::shape_not not_, const std::string& name, bool is_parser)
    {
        return add(shape_type::logical_not, not_.data, name, is_parser);
    }
    ngl::shape_data shape_cluster::add(ngl::shape_sequence sequence, const std::string& name, bool is_parser)
    {
        return add(shape_type::vector_sequence, std::move(sequence.data), name, is_parser);
    }
    ngl::shape_data shape_cluster::add(ngl::shape_space space, const std::string& name, bool is_parser)
    {
        return add(shape_type::space, space.data, name, is_parser);
    }

    const std::string& shape_cluster::name() const { return name_; }
    std::vector<ngl::shape_data>& shape_cluster::datas() { return shape_datas_; }
    uint64_t shape_cluster::scalar_shapes_count() const { return scalar_shapes_; }
    uint64_t shape_cluster::vector_shapes_count() const { return vector_shapes_; }

    bool shape_cluster::is_scalar(uint64_t shape_index) const
    {
        return shape_index < scalar_shapes_count();
    }

    // ngl_shape_cluster

    ngl_shape_cluster::ngl_shape_cluster() : shape_cluster("ngl_shape_cluster")
    {
        // Adding basic grammar of ngl
        auto left_chevron       = add(ngl::shape_element('<'));
        auto right_chevron      = add(ngl::shape_element('>'));
        auto left_brace         = add(ngl::shape_element('['));
        auto right_brace        = add(ngl::shape_element(']'));
        auto underscore         = add(ngl::shape_element('_'));
        auto edge               = add(ngl::shape_element(':'));
        auto left_curly_brace   = add(ngl::shape_element('{'));
        auto right_curly_brace  = add(ngl::shape_element('}'));

        auto newline            = add(ngl::shape_element('\n'));
        auto space              = add(ngl::shape_space(' '));
        auto tab                = add(ngl::shape_space('\t'));
        auto whitespace         = add(ngl::shape_or(space, tab, newline));
        auto whitespaces        = add(ngl::shape_many(whitespace));

        auto minus_letter       = add(ngl::shape_range('a', 'z'));
        auto maj_letter         = add(ngl::shape_range('A', 'Z'));
        auto letter             = add(ngl::shape_or(minus_letter, maj_letter));

        auto digit              = add(ngl::shape_range('0', '9'));
        auto digits             = add(ngl::shape_many(digit));

        auto identifier_symbol  = add(ngl::shape_or(letter, digit, underscore));
        auto raw_identifier     = add(ngl::shape_many(identifier_symbol));
    }
} // ngl