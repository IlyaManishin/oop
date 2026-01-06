#include "../core/exceptions.hpp"
#include "../core/types.hpp"

#include "wav/wav.hpp"

#include <string>
#include <unordered_map>

namespace tree_executor
{
    using PredicateType = bool (*)(std::vector<ExObjPtr>);

    static bool exists_wav_file(std::vector<ExObjPtr> args);

    const std::unordered_map<std::string, PredicateType> predicates = {
        {"exists", exists_wav_file}};

    bool predicate_call_impl(const std::string &predicatName, std::vector<ExObjPtr> args)
    {
        auto it = predicates.find(predicatName);
        if (it == predicates.end())
        {
            throw UnexpectedIdentExc(predicatName);
        }

        return it->second(args);
    }

    static bool exists_wav_file(std::vector<ExObjPtr> args)
    {
        if (args.empty())
            return false;

        const StringType *strObj = dynamic_cast<const StringType *>(args[0]);
        if (!strObj)
        {
            throw UnexpectedArgExc(args[0]->GetType(), STRING_TYPE_NAME);
        }

        const std::string &path = strObj->GetValue();
        wav_lib::WavReader reader;
        return reader.IsExistsWav(path);
    }

} // namespace tree_executor
