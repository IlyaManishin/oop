#include <assert.h>
#include <cstring>
#include <exception>
#include <iostream>

#include "cmd_reader.h"

#define OPT_KEY_OFFSET 1

#define DEFAULT_DEST_PATH "output.csv"
#define DEFAULT_DELIM ';'
#define DEFAULT_MAX_COLUMNS 100

namespace lab0
{
    enum
    {
        MAX_ARGUMENT_SIZE = 128,
    };
    static const char OPTION_PREFIX = '-';

    class Option
    {
    private:
        bool hasValue;
        char *key;
        char *value;

        const char *get_char_pos(const char *str, char ch)
        {
            size_t sLength = strlen(str);
            for (int i = 0; i < sLength; i++)
            {
                if (*str == ch)
                    return str;
                str++;
            }
            return nullptr;
        }

    public:
        Option(const char *argValue)
        {
            argValue += OPT_KEY_OFFSET;
            size_t argLength = strlen(argValue);
            if (argLength > MAX_ARGUMENT_SIZE)
                throw InvalidArguments("Argument is very long", argValue);

            const char *eqPos = get_char_pos(argValue, '=');
            if (eqPos == nullptr)
                eqPos = argValue + argLength;

            size_t keyLength = eqPos - argValue;
            if (keyLength == 0)
                throw InvalidArguments("No key in argument", argValue);

            key = (char *)malloc(keyLength + 1);
            memcpy(key, argValue, keyLength);
            key[keyLength] = '\0';

            if (eqPos == argValue + argLength)
            {
                hasValue = false;
                value = nullptr;
            }
            else
            {
                size_t valueLength = argLength - keyLength - 1;
                value = (char *)malloc(valueLength + 1);
                memcpy(value, eqPos + 1, valueLength);
                value[valueLength] = '\0';
                hasValue = true;
            }
        }

        bool check_key(const char *check)
        {
            return (strcmp(check, this->key) == 0);
        }

        const char *get_value()
        {
            return this->value;
        }

        static bool arg_is_option(const char *argValue)
        {
            if (strlen(argValue) < OPT_KEY_OFFSET + 1)
                return false;

            bool res = (argValue[0] == OPTION_PREFIX);
            return res;
        }

        ~Option()
        {
            free(key);
            free(value);
        }
    };

    CmdReader::CmdReader(int argc, const char *argv[])
    {
        for (int i = 1; i < argc; i++)
        {
            if (Option::arg_is_option(argv[i]))
            {
                this->options.push_back(new Option(argv[i]));
            }
            else
            {
                this->args.push_back(argv[i]);
            }
        }
    }

    CmdReader::~CmdReader()
    {
        for (Option *opt : options)
        {
            delete opt;
        }
    }

    const char *CmdReader::get_option_value(const char *key)
    {
        for (Option *opt : this->options)
        {
            if (opt->check_key(key))
                return opt->get_value();
        }
        return nullptr;
    }

    bool CmdReader::check_option(const char *key)
    {
        for (Option *opt : this->options)
        {
            if (opt->check_key(key))
                return true;
        }
        return false;
    }

    int CmdReader::try_get_int_arg(const char *str, int baseValue)
    {
        if (str == nullptr)
            return baseValue;
        int res = atoi(str);
        if (res == 0)
            res = baseValue;
        return res;
    }

    TProgramSettings CmdReader::get_settings()
    {
        TProgramSettings res;
        res.isError = false;

        res.filePath = this->get_arg_value(0);
        if (res.filePath == nullptr)
        {
            res.isError = true;
            res.errMsg = "Error: No input file";
            return res;
        }
        res.destPath = this->get_option_value("dest");
        if (res.destPath == nullptr)
            res.destPath = DEFAULT_DEST_PATH;

        const char *maxColS = this->get_option_value("h");
        res.maxColumns = try_get_int_arg(maxColS, DEFAULT_MAX_COLUMNS);

        const char *delimS = this->get_option_value("d");
        if (delimS == nullptr || strlen(delimS) == 0)
            res.delim = DEFAULT_DELIM;
        else
            res.delim = delimS[0];

        return res;
    }

    const char *CmdReader::get_arg_value(int index)
    {
        if (index >= this->args.size())
            return nullptr;
        return this->args[index];
    }

}