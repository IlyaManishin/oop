#include "file_parser/types.hpp"

#include "types.hpp"
#include "exceptions.hpp"

namespace tree_executor
{
    const std::string &parse_string_from_obj_ptr(const ExObjPtr obj)
    {
        const StringType *strObj = dynamic_cast<const StringType *>(obj);
        if (!strObj)
        {
            throw UnexpectedArgExc(obj->GetType(), STRING_TYPE_NAME);
        }
        return strObj->GetValue();
    }

    const ExObjUPtr exObj_from_value_arg(const file_parser::ArgUPtr &arg)
    {
        if (arg->type == file_parser::Arg::Type::NUMBER)
            return std::make_unique<FloatType>(std::get<float>(arg->value));

        if (arg->type == file_parser::Arg::Type::STRING)
            return std::make_unique<StringType>(std::get<std::string>(arg->value));

        return nullptr;
    }

} // namespace tree_executor
