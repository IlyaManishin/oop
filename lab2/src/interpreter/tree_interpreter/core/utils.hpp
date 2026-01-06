#pragma once

#include "file_parser/types.hpp"

#include "types.hpp"

namespace tree_executor
{
    const std::string &parse_string_from_obj_ptr(const ExObjPtr obj);
    ExObjUPtr exobj_from_value_arg(const file_parser::ArgUPtr &arg);

    template <typename T>
    T exobj_convert(ExObj *obj, const char *typeName)
    {
        T tObj = dynamic_cast<T>(obj);
        if (tObj == nullptr)
            throw UnexpectedArgExc(obj->GetType(), typeName);
        return tObj;
    }

} // namespace tree_executor
