#include "types.hpp"
#include "exceptions.hpp"
#include "utils.hpp"

#include "wav/wav.hpp"

using namespace wav_lib;
namespace tree_executor
{
    ExObjUPtr tree_executor::ExObj::RunMethod(const std::string &name, const std::vector<ExObjPtr> &args)
    {
        auto it = methods.find(name);
        if (it != methods.end())
        {
            return it->second(this, args);
        }
        else
        {
            throw RunTimeExc("Method '" + name + "' in object of type " +
                             std::string(this->GetType()) + " not found");
        }
    }

    ExObjUPtr WavFileType::getInterval(ExObj *cur, const std::vector<ExObjPtr> &args)
    {
        const size_t argsCount = 2;
        if (args.size() != argsCount)
            throw InvalidArgsCountExc(args.size(), argsCount);
        FloatType *startSec = exobj_convert<FloatType *>(args[0], FLOAT_TYPE_NAME);
        FloatType *endSec = exobj_convert<FloatType *>(args[1], FLOAT_TYPE_NAME);

        WavFileType *wavObj = static_cast<WavFileType *>(cur);
        IWavFileSPtr wavFile = wavObj->GetValue();

        IWavIntervalSPtr interval = nullptr;
        try
        {
            interval = wavFile->GetInterval(startSec->GetValue(),
                                            endSec->GetValue());
        }
        catch (const std::exception &exc)
        {
            throw RunTimeExc(exc.what());
        }
        return std::make_unique<WavIntervalType>(interval);
    }

    WavFileType::WavFileType(wav_lib::IWavFileSPtr v) : value(std::move(v))
    {
        type = WAVFILE_TYPE_NAME;
        this->regMethod("get_interval", getInterval);
    }

    WavIntervalType::WavIntervalType(wav_lib::IWavIntervalSPtr v)
        : value(std::move(v))
    {
        type = WAVINTERVAL_TYPE_NAME;
    }

} // namespace tree_executor
