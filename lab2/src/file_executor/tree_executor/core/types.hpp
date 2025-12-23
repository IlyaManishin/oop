#include "wav/wav.hpp"

#include <iostream>
#include <memory>
#include <string>

namespace tree_executor
{
    class ExValue
    {
    public:
        virtual void Print(std::ostream &out) const = 0;
        virtual void RunMethod(const std::string &name) = 0; // args???
        virtual ~ExValue() = default;
    };

    using ExValueUPtr = std::unique_ptr<ExValue>;

    class FloatType : public ExValue
    {
    private:
        float value;

    public:
        FloatType(float v) : value(v) {}
        float GetValue() const { return value; }

        void Print(std::ostream &out) const override
        {
            out << std::to_string(value);
        }
    };

    class StringType : public ExValue
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

    class WavFileType : public ExValue
    {
    private:
        wav_lib::WavFileSPtr value;

    public:
        WavFileType(wav_lib::WavFileSPtr v) : value(std::move(v)) {}
        wav_lib::WavFileSPtr GetValue() const { return value; }

        void Print(std::ostream &out) const override
        {
            this->value->PrintInfo(out);
        }
    };

    class WavIntervalType : public ExValue
    {
    private:
        wav_lib::WavFileSPtr value;

    public:
        WavIntervalType(wav_lib::WavFileSPtr v) : value(std::move(v)) {}
        wav_lib::WavFileSPtr GetValue() const { return value; }

        void Print(std::ostream &out) const override
        {
            this->value->PrintInfo(out);
        }
        ~WavIntervalType() override
        {
            this->value->Save();
        }
    };

} // namespace tree_executor
