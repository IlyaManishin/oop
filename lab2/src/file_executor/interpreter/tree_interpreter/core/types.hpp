#pragma once

#include "wav/wav.hpp"

#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace tree_executor
{
    inline const char *BASE_TYPE_NAME = "base";
    inline const char *FLOAT_TYPE_NAME = "float";
    inline const char *STRING_TYPE_NAME = "string";
    inline const char *BOOL_TYPE_NAME = "bool";
    inline const char *WAVFILE_TYPE_NAME = "wav_file";
    inline const char *WAVINTERVAL_TYPE_NAME = "wav_interval";

    class ExObj;
    using ExObjUPtr = std::unique_ptr<ExObj>;
    using ExObjPtr = const ExObj *;

    using ExObjs = std::vector<ExObjUPtr>;

    using MethodType = std::function<ExObjUPtr(const std::vector<ExObjUPtr> &)>;

    class ExObj
    {
    protected:
        std::unordered_map<std::string, MethodType> methods;
        const char *type = BASE_TYPE_NAME;

    public:
        virtual void Print(std::ostream &out) const = 0;
        const char *GetType() const { return type; }

        virtual ~ExObj() = default;

        void RunMethod(const std::string &name, const std::vector<ExObjUPtr> &args);
    };

    class FloatType : public ExObj
    {
    private:
        float value;

    public:
        FloatType(float v) : value(v) { type = FLOAT_TYPE_NAME; }
        float GetValue() const { return value; }

        void Print(std::ostream &out) const override
        {
            std::ostringstream ss;
            ss.setf(std::ios::fixed);
            ss.precision(2);
            ss << value;
            out << ss.str();
        }
    };

    class StringType : public ExObj
    {
    private:
        const std::string value;

    public:
        StringType(std::string v) : value(std::move(v)) { type = STRING_TYPE_NAME; }
        const std::string &GetValue() const { return value; }

        void Print(std::ostream &out) const override
        {
            out << value;
        }
    };

    class BoolType : public ExObj
    {
    private:
        bool value;

    public:
        BoolType(bool v) : value(v) { type = BOOL_TYPE_NAME; }
        float GetValue() const { return value; }

        void Print(std::ostream &out) const override
        {
            if (this->value)
                out << "True";
            else
                out << "False";
        }
    };

    class WavFileType : public ExObj
    {
    private:
        wav_lib::WavFileSPtr value;

    public:
        WavFileType(wav_lib::WavFileSPtr v) : value(std::move(v)) { type = WAVFILE_TYPE_NAME; }
        wav_lib::WavFileSPtr GetValue() const { return value; }

        void Print(std::ostream &out) const override
        {
            this->value->PrintInfo(out);
        }

        ~WavFileType() override
        {
            this->value->Save();
        }
    };

    class WavIntervalType : public ExObj
    {
    private:
        wav_lib::WavFileSPtr value;

    public:
        WavIntervalType(wav_lib::WavFileSPtr v) : value(std::move(v)) { type = WAVINTERVAL_TYPE_NAME; }
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

    class ArgsFrame
    {
    public:
        std::vector<ExObjPtr> allArgs;
        ExObjs valueArgs;
    };
    using ArgsFrameUPtr = std::unique_ptr<ArgsFrame>;

} // namespace tree_executor
