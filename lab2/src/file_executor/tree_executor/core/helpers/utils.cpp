#include "../types/types.hpp"
#include "exceptions.hpp"

namespace tree_executor
{
    const std::string &parse_string_from_obj(const ExObjUPtr &obj)
    {
        const StringType *strObj = dynamic_cast<const StringType *>(obj.get());
        if (!strObj)
        {
            throw UnexpectedArg(obj, STRING_TYPE_NAME);
        }
        return strObj->GetValue();
    }
} // namespace tree_executor
