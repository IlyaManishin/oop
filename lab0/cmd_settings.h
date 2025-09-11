#include <exception>
#include <string>
#include <vector>

namespace lab0
{
    enum
    {
        MAX_ARGUMENT_SIZE = 128,
    };

    class Option;

    class InvalidArguments : public std::exception
    {
    public:
        InvalidArguments(const char *message, const char *arg)
        {
            this->errMessage = std::string(message) + ": " + arg;
        }
        const char *what() const noexcept override
        {
            return errMessage.c_str();
        }

    private:
        std::string errMessage;
    };

    class CmdSettings
    {

    public:
        CmdSettings(int argc, const char *argv[]);
        ~CmdSettings();

        const char *get_option_value(const char *key);
        bool check_option(const char *key);

        const char *get_arg_value(int index);

    private:
        std::vector<Option *> options;
        std::vector<const char *> args;
    };

}