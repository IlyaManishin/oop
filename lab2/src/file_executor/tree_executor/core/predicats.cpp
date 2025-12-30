#include "helpers/exceptions.hpp"
#include "types/types.hpp"

#include "wav/wav.hpp"

#include <string>
#include <unordered_map>

namespace tree_executor
{
    bool exists_wav_file(const ExObjs &objs);

    const std::unordered_map<std::string, bool (*)(const ExObjs &objs)> predicats = {
        {"exists", exists_wav_file}};

    bool check_predicate(const std::string &predicatName, const ExObjs &args)
    {
        auto it = predicats.find(predicatName);
        if (it == predicats.end())
        {
            throw UnexpectedIdentExc(predicatName);
        }

        return it->second(args);
    }

    bool exists_wav_file(const ExObjs &objs)
    {
        if (objs.empty())
            return false;

        const ExObjUPtr &arg = objs[0];
        const StringType *strObj = dynamic_cast<const StringType *>(arg.get());
        if (!strObj)
        {
            throw UnexpectedArgExc(arg, STRING_TYPE_NAME);
        }

        const std::string &path = strObj->GetValue();
        wav_lib::WavReader reader;
        return reader.IsExistsWav(path);
    }

} // namespace tree_executor
