#include "types.hpp"
#include "exceptions.hpp"

namespace tree_executor
{
    void tree_executor::ExObj::RunMethod(const std::string &name, const std::vector<ExObjPtr> &args)
    {
        auto it = methods.find(name);
        if (it != methods.end())
        {
            it->second(args);
        }
        else
        {
            throw RunTimeExc("Method '" + name + "' in object of type " +
                             std::string(this->GetType()) + " not found");
        }
    }

    WavFileType::WavFileType(wav_lib::IWavFileSPtr v) : value(std::move(v))
    {
        type = WAVFILE_TYPE_NAME;
    }

    WavIntervalType::WavIntervalType(wav_lib::IWavIntervalSPtr v)
        : value(std::move(v))
    {
        type = WAVINTERVAL_TYPE_NAME;
    }

} // namespace tree_executor
