#include <assert.h>
#include <cstring>
#include <exception>
#include <iostream>

#include "cmd_settings.h"

#define MIN_VALUE_OPT_LEN 3
#define OPT_KEY_OFFSET 2

namespace lab0
{
    class Option
    {
    private:
        char *key;
        char *value;

    public:
        Option(const char *argValue)
        {
            const char *eqPos = get_char_pos(argValue, '=');
            assert(eqPos != NULL);

            argValue += OPT_KEY_OFFSET;

            size_t argLength = strlen(argValue);
            if (argLength > MAX_ARGUMENT_SIZE)
            {
                throw InvalidArguments("Argument is very long", argValue);
            }

            size_t keyLength = eqPos - argValue;
            if (keyLength == 0)
            {
                throw InvalidArguments("No key in argument", argValue);
            }
            this->key = (char *)malloc((keyLength + 1) * sizeof(char));
            strncpy(this->key, argValue, keyLength);
            this->key[keyLength] = '\0';

            size_t valueLength = argLength - keyLength - 1;
            if (valueLength <= 0)
            {
                throw InvalidArguments("No value in argument", argValue);
            }

            this->value = (char *)malloc((valueLength + 1) * sizeof(char));
            strncpy(this->value, eqPos + 1, valueLength);
            this->key[valueLength] = '\0';
        }

        bool check_key(const char *check)
        {
            return strncmp(check, this->key, MAX_ARGUMENT_SIZE) == 0;
        }

        static bool arg_is_option(const char *argValue)
        {
            if (strlen(argValue) < MIN_VALUE_OPT_LEN)
                return false;

            if (argValue[0] == '-' && argValue[1] == '-' && get_char_pos(argValue, '=') != NULL)
                return true;
            return false;
        }

        Option::~Option()
        {
            free(key);
            free(value);
        }
    };

    static const char *get_char_pos(const char *str, char ch)
    {
        size_t sLength = strlen(str);
        for (int i = 0; i < sLength; i++)
        {
            if (*str == ch)
                return str;
            str++;
        }
        return NULL;
    }

    CmdSettings::CmdSettings(int argc, const char *argv[])
    {
        for (int i = 1; i < argc; i++)
        {
            if (Option::arg_is_option(argv[i]))
            {
                this->options.push_back(new Option(argv[i]));
            }
        }
    }

}