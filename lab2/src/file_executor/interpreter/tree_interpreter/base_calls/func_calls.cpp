#include "../core/exceptions.hpp"
#include "../core/types.hpp"
#include "../core/utils.hpp"

#include "wav/wav.hpp"

#include <iostream>
#include <string>
#include <unordered_map>

using namespace wav_lib;

namespace tree_executor
{
    using FuncCallType = ExObjUPtr (*)(std::vector<ExObjPtr>, std::ostream &out);

    static ExObjUPtr create_wav(std::vector<ExObjPtr> args, std::ostream &out);
    static ExObjUPtr open_wav(std::vector<ExObjPtr> args, std::ostream &out);
    static ExObjUPtr print_args(std::vector<ExObjPtr> args, std::ostream &out);

    const std::unordered_map<std::string, FuncCallType> functions = {
        {"create_wav", create_wav},
        {"open_wav", open_wav},
        {"print", print_args}};

    static const std::string &args_to_string(const std::vector<ExObjPtr> &args)
    {
        if (args.size() != 1)
            throw InvalidArgsCountExc(args.size(), 1);

        return parse_string_from_obj_ptr(args[0]);
    }

    ExObjUPtr func_call_impl(const std::string &funcName, std::vector<ExObjPtr> args, std::ostream &out)
    {
        auto it = functions.find(funcName);
        if (it == functions.end())
            throw UnexpectedIdentExc(funcName);

        return it->second(args, out);
    }

    static ExObjUPtr create_wav(std::vector<ExObjPtr> args, std::ostream &out)
    {
        const std::string &path = args_to_string(args);
        WavReader reader;
        try
        {
            WavFileSPtr file = reader.CreateWav(path);
            return std::make_unique<WavFileType>(file);
        }
        catch (const WavException &exc)
        {
            throw RunTimeExc(exc.what());
        }
    }

    static ExObjUPtr open_wav(std::vector<ExObjPtr> args, std::ostream &out)
    {
        const std::string &path = args_to_string(args);
        WavReader reader;
        try
        {
            WavFileSPtr file = reader.OpenWav(path);
            return std::make_unique<WavFileType>(file);
        }
        catch (const WavException &exc)
        {
            throw RunTimeExc(exc.what());
        }
    }

    static ExObjUPtr print_args(std::vector<ExObjPtr> args, std::ostream &out)
    {
        for (size_t i = 0; i < args.size(); ++i)
        {
            if (i != 0)
                out << ' ';
            args[i]->Print(out);
        }
        out << std::endl;
        return nullptr;
    }

} // namespace tree_executor
