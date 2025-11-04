#include "../src/cmd_parser/cmd_parser.hpp"

#include <gtest/gtest.h>
#include <variant>
#include <string>

using namespace cmd;

enum class CMD
{
    HELP,
    MIX,
    MUTE,
    CHANGE_SPEED
};

class CommandParserTest : public ::testing::Test
{
protected:
    CommandParser parser;

    void SetUp() override
    {
        parser.registerCommand("help", static_cast<int>(CMD::HELP));
        parser.registerCommand("mix", static_cast<int>(CMD::MIX));
        parser.registerCommand("mute", static_cast<int>(CMD::MUTE));
        parser.registerCommand("speed", static_cast<int>(CMD::CHANGE_SPEED));
    }
};

TEST_F(CommandParserTest, HelpCommandNoArgs)
{
    const char *argv[] = {"program", "help"};
    auto cmd = parser.parse(2, const_cast<char **>(argv));
    EXPECT_EQ(cmd->commandId, static_cast<int>(CMD::HELP));
    EXPECT_TRUE(cmd->args.empty());
}

TEST_F(CommandParserTest, MixCommandWithArgs)
{
    const char *argv[] = {"program", "mix", "42", "0.5", "hello"};
    auto cmd = parser.parse(5, const_cast<char **>(argv));
    EXPECT_EQ(cmd->commandId, static_cast<int>(CMD::MIX));
    ASSERT_EQ(cmd->args.size(), 3);

    EXPECT_FLOAT_EQ(std::get<float>(cmd->args[0]), 42.0f);
    EXPECT_FLOAT_EQ(std::get<float>(cmd->args[1]), 0.5f);
    EXPECT_EQ(std::get<std::string>(cmd->args[2]), "hello");
}

TEST_F(CommandParserTest, UnknownCommandThrows)
{
    const char *argv[] = {"program", "unknown"};
    EXPECT_THROW(parser.parse(2, const_cast<char **>(argv)), std::runtime_error);
}

TEST_F(CommandParserTest, NoCommandThrows)
{
    const char *argv[] = {"program"};
    EXPECT_THROW(parser.parse(1, const_cast<char **>(argv)), std::runtime_error);
}
