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
        if (it == methods.end())
        {
            throw RunTimeExc("Method '" + name + "' in object of type " +
                             std::string(this->GetType()) + " not found");
        }
        return it->second(this, name, args);
    }

    ExObjUPtr WavFileType::getInterval(ExObj *cur, const std::string &methodName,
                                       const std::vector<ExObjPtr> &args)
    {
        const size_t argsCount = 2;
        if (args.size() != argsCount)
            throw InvalidArgsCountExc(args.size(), argsCount, methodName);
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
            throw RunTimeExc(std::string("Can't get interval: ") + exc.what());
        }
        return std::make_unique<WavIntervalType>(interval);
    }

    ExObjUPtr WavFileType::writeInterval(ExObj *cur, const std::string &methodName,
                                         const std::vector<ExObjPtr> &args)
    {
        const size_t argsCount = 3;
        if (args.size() != argsCount)
            throw InvalidArgsCountExc(args.size(), argsCount, methodName);

        WavIntervalType *intervalObj = exobj_convert<WavIntervalType *>(args[0], INTERVAL_TYPE_NAME);
        FloatType *destSec = exobj_convert<FloatType *>(args[1], FLOAT_TYPE_NAME);
        BoolType *isInsert = exobj_convert<BoolType *>(args[2], BOOL_TYPE_NAME);

        WavFileType *wavObj = static_cast<WavFileType *>(cur);
        IWavFileSPtr wavFile = wavObj->GetValue();
        IWavIntervalSPtr interval = intervalObj->GetValue();

        try
        {
            wavFile->WriteInterval(interval, destSec->GetValue(), isInsert->GetValue());
            wavFile->Save();
        }
        catch (const std::exception &exc)
        {
            throw RunTimeExc(std::string("Can't write interval: ") + exc.what());
        }

        return nullptr;
    }

    ExObjUPtr WavIntervalType::setEffect(ExObj *cur, const std::string &methodName,
                                         const std::vector<ExObjPtr> &args)
    {
        const size_t argsCount = 1;
        if (args.size() != argsCount)
            throw InvalidArgsCountExc(args.size(), argsCount, methodName);

        StringType *effectStr = exobj_convert<StringType *>(args[0], STRING_TYPE_NAME);

        WavIntervalType *intervalObj = static_cast<WavIntervalType *>(cur);
        IWavIntervalSPtr interval = intervalObj->GetValue();

        WavEffects effect;

        std::string eff = effectStr->GetValue();
        if (eff == "normal")
            effect = WavEffects::NORMAL;
        else if (eff == "bass")
            effect = WavEffects::BASS;
        else if (eff == "hach_lada")
            effect = WavEffects::HACH_LADA;
        else if (eff == "raise_high")
            effect = WavEffects::RAISE_HIGH;
        else if (eff == "distorsion")
            effect = WavEffects::DISTORTION;
        else
            throw RunTimeExc("Invalid effect name" + eff);

        try
        {
            interval->SetEffect(effect);
        }
        catch (const std::exception &exc)
        {
            throw RunTimeExc(std::string("Error in setting effect to interval: ") + exc.what());
        }

        return nullptr;
    }

    WavFileType::WavFileType(wav_lib::IWavFileSPtr v) : value(std::move(v))
    {
        type = WAVFILE_TYPE_NAME;
        this->regMethod("get_interval", getInterval);
        this->regMethod("write_interval", writeInterval);
    }

    WavIntervalType::WavIntervalType(wav_lib::IWavIntervalSPtr v)
        : value(std::move(v))
    {
        type = INTERVAL_TYPE_NAME;
        this->regMethod("set_effect", this->setEffect);
    }

} // namespace tree_executor
