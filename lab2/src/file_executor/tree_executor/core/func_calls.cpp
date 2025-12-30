#include "helpers/exceptions.hpp"
#include "helpers/utils.hpp"
#include "types/types.hpp"

#include "wav/wav.hpp"

#include <string>
#include <unordered_map>

using namespace wav_lib;

namespace tree_executor
{
    using FuncCallType = ExObjUPtr (*)(const ExObjs &);

    ExObjUPtr create_wav(const ExObjs &args);
    ExObjUPtr open_wav(const ExObjs &args);
    ExObjUPtr print_args(const ExObjs &args);

    const std::unordered_map<std::string, FuncCallType> functions = {
        {"create_wav", create_wav},
        {"open_wav", open_wav},
        {"print", print_args}};

    static const std::string &args_to_string(const ExObjs &args)
    {
        if (args.size() != 1)
            throw InvalidArgsCountExc(args.size(), 1);

        const ExObjUPtr &arg = args[0];
        return parse_string_from_obj(arg);
    }

    ExObjUPtr func_call(const std::string &funcName, const ExObjs &args)
    {
        auto it = functions.find(funcName);
        if (it == functions.end())
            throw UnexpectedIdentExc(funcName);

        return it->second(args);
    }

    ExObjUPtr create_wav(const ExObjs &args)
    {
        const std::string &path = args_to_string(args);
        WavReader reader;
        try
        {
            reader.CreateWav(path);
        }
        catch (const WavException &exc)
        {
            throw RunTimeExc(exc.what());
        }
    }

    ExObjUPtr open_wav(const ExObjs &args)
    {
        const std::string &path = args_to_string(args);
        WavReader reader;
        try
        {
            reader.CreateWav(path);
        }
        catch (const WavException &exc)
        {
            throw RunTimeExc(exc.what());
        }
    }

    ExObjUPtr print_args(const ExObjs &args)
    {
        for (size_t i = 0; i < args.size(); ++i)
        {
            if (i != 0)
                std::cout << ' ';

            args[i]->Print(std::cout);
        }

        std::cout << std::endl;
        return nullptr;
    }

} // namespace tree_executor
