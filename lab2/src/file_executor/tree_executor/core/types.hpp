#include "wav/wav.hpp"

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace tree_executor
{
    class ExValue;
    using ExValueUPtr = std::unique_ptr<ExValue>;
    using ExValues = std::vector<ExValueUPtr>;

    using MethodType = std::function<ExValueUPtr(const std::vector<ExValueUPtr> &)>;

    class ExValue
    {
    protected:
        std::unordered_map<std::string, MethodType> methods;

    public:
        virtual void Print(std::ostream &out) const = 0;
        virtual ~ExValue() = default;

        void RunMethod(const std::string &name, const std::vector<ExValueUPtr> &args);

    };

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
