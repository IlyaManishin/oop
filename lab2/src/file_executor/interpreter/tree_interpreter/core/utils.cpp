#include "file_parser/types.hpp"

#include "exceptions.hpp"
#include "types.hpp"

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

    ExObjUPtr exobj_from_value_arg(const file_parser::ArgUPtr &arg)
    {
        if (arg->type == file_parser::Arg::Type::NUMBER)
            return std::make_unique<FloatType>(std::get<float>(arg->value));

        if (arg->type == file_parser::Arg::Type::STRING)
            return std::make_unique<StringType>(std::get<std::string>(arg->value));

        if (arg->type == file_parser::Arg::Type::BOOL)
            return std::make_unique<BoolType>(std::get<bool>(arg->value));

        return nullptr;
    }

} // namespace tree_executor
