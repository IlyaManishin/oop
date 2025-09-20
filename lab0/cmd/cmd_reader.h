#include <exception>
#include <string>
#include <vector>

namespace lab0
{
    typedef struct TProgramSettings
    {
        bool isError;
        const char *errMsg;

        const char *filePath;
        const char *destPath;
        int maxColumns;
        char delim;
    } TProgramSettings;

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

    class CmdReader
    {

    public:
        CmdReader(int argc, const char *argv[]);
        ~CmdReader();
        TProgramSettings get_settings();

        const char *get_option_value(const char *key);
        bool check_option(const char *key);

        const char *get_arg_value(int index);

    private:
        std::vector<Option *> options;
        std::vector<const char *> args;
        int try_get_int_arg(const char *str, int baseValue);
    };

}