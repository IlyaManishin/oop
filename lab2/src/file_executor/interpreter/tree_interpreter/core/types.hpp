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
    using ExObjs = std::vector<ExObjUPtr>;

    using ExObjPtr = ExObj *;
    // using MethodType = std::function<ExObjUPtr(const std::vector<ExObjPtr> &)>;
    // using Method = ExObjUPtr (*)(const std::vector<ExObjPtr> &);
    using StaticMethod = ExObjUPtr (*)(ExObj *cur, const std::vector<ExObjPtr> &);

    class ExObj
    {
    protected:
        std::unordered_map<std::string, StaticMethod> methods;
        const char *type = BASE_TYPE_NAME;

        void regMethod(const std::string &name, StaticMethod methodPtr) { this->methods[name] = methodPtr; };

    public:
        virtual void Print(std::ostream &out) const = 0;
        const char *GetType() const { return type; }

        virtual ~ExObj() = default;

        void RunMethod(const std::string &name, const std::vector<ExObjPtr> &args);
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
        wav_lib::IWavFileSPtr value;
        static ExObjUPtr getInterval(ExObj* cur, const std::vector<ExObjPtr> &args);

    public:
        WavFileType(wav_lib::IWavFileSPtr v);
        wav_lib::IWavFileSPtr GetValue() const { return value; }

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
        wav_lib::IWavIntervalSPtr value;

    public:
        WavIntervalType(wav_lib::IWavIntervalSPtr v);
        wav_lib::IWavIntervalSPtr GetValue() const { return value; }

        void Print(std::ostream &out) const override
        {
            this->value->Print(out);
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
