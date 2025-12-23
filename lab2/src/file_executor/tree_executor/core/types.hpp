#include "wav/wav.hpp"

#include <iostream>
#include <memory>
#include <string>

namespace file_executor
{
    class Variable
    {
    public:
        virtual void Print(std::ostream &out) const = 0;
        virtual ~Variable() = default;
    };

    using VariableUPtr = 

    class FloatType : public Variable
    {
    private:
        float value;

    public:
        FloatType(float v) : value(v) {}
        float GetValue() const { return value; }

        void Print(std::ostream &out) const override
        {
            out << value;
        }
    };

    class StringType : public Variable
    {
    private:
        std::string value;

    public:
        StringType(std::string v) : value(std::move(v)) {}
        const std::string &GetValue() const { return value; }

        void Print(std::ostream &out) const override
        {
            out << value;
        }
    };

    class WavFileType : public Variable
    {
    private:
        wav_lib::WavFileSPtr value;

    public:
        WavFileType(wav_lib::WavFileSPtr v) : value(std::move(v)) {}
        wav_lib::WavFileSPtr GetValue() const { return value; }

        void Print(std::ostream &out) const override
        {
            out << "[WavFile: " << value.get() << "]";
        }
    };

    class WavIntervalType : public Variable
    {
    private:
        wav_lib::WavFileSPtr value;

    public:
        WavIntervalType(wav_lib::WavFileSPtr v) : value(std::move(v)) {}
        wav_lib::WavFileSPtr GetValue() const { return value; }

        void Print(std::ostream &out) const override
        {
            out << "[IWavInterval: " << value.get() << "]";
        }
        ~WavIntervalType() override
        {
            this->value->Save();
        }
    };

} // namespace file_executor
