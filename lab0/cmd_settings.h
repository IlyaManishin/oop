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
        CmdSettings::CmdSettings(int argc, const char *argv[]);

    private:
        std::vector<Option *> options;
    };

}