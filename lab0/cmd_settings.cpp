#include <assert.h>
#include <cstring>
#include <exception>
#include <iostream>

#include "cmd_settings.h"

#define OPT_KEY_OFFSET 1

namespace lab0
{
    static const char OPTION_PREFIX = '-';

    static const char *get_char_pos(const char *str, char ch);

    class Option
    {
    private:
        bool hasValue;
        char *key;
        char *value;

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

    CmdSettings::CmdSettings(int argc, const char *argv[])
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
    CmdSettings::~CmdSettings()
    {
        for (Option *opt : options)
        {
            delete opt;
        }
    }
    const char *CmdSettings::get_option_value(const char *key)
    {
        for (Option *opt : this->options)
        {
            if (opt->check_key(key))
                return opt->get_value();
        }
        return nullptr;
    }

    bool CmdSettings::check_option(const char *key)
    {
        for (Option *opt : this->options)
        {
            if (opt->check_key(key))
                return true;
        }
        return false;
    }

    const char *CmdSettings::get_arg_value(int index)
    {
        if (this->args.size() <= index)
            return nullptr;
        return this->args[index];
    }

    static const char *get_char_pos(const char *str, char ch)
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

}