#include "file_parser/parser.hpp"
#include "test_paths.hpp"

#include <exception>
#include <gtest/gtest.h>
#include <iostream>

using namespace file_parser;

static std::string Path(const std::string &name)
{
    return FILE_PARSER_DIR + name;
}

static std::string InvalidScriptPath(const std::string &name)
{
    return FILE_PARSER_DIR + "invalid_scripts/" + name;
}

TEST(ParserTest, SimpleAssign)
{
    AstParser parser(Path("simple_assign.txt"));
    auto tree = parser.ParseFileTree();

    ASSERT_TRUE(tree);
    ASSERT_TRUE(tree->statements);
    ASSERT_EQ(tree->statements->size(), 1u);

    auto &stmt = (*tree->statements)[0];
    EXPECT_TRUE(std::holds_alternative<AssignUPtr>(stmt->value));

    auto &assign = std::get<AssignUPtr>(stmt->value);
    EXPECT_EQ(assign->ident, "x");
    EXPECT_EQ(assign->right->name, "foo");
    ASSERT_TRUE(assign->right->args);
    EXPECT_EQ(assign->right->args->size(), 2u);
}

TEST(ParserTest, FuncAndMethod)
{
    AstParser parser(Path("func_and_method.txt"));
    auto tree = parser.ParseFileTree();

    ASSERT_TRUE(tree);
    ASSERT_TRUE(tree->statements);
    ASSERT_EQ(tree->statements->size(), 2u);

    EXPECT_TRUE(std::holds_alternative<FuncRunUPtr>((*tree->statements)[0]->value));
    EXPECT_TRUE(std::holds_alternative<MethodRunUPtr>((*tree->statements)[1]->value));

    auto &method = std::get<MethodRunUPtr>((*tree->statements)[1]->value);
    EXPECT_EQ(method->object, "obj");
    EXPECT_EQ(method->call->name, "run");
    ASSERT_TRUE(method->call->args);
    EXPECT_EQ(method->call->args->size(), 1u);
}

TEST(ParserTest, IfBlock)
{
    AstParser parser(Path("if_block.txt"));
    auto tree = parser.ParseFileTree();

    ASSERT_TRUE(tree);
    ASSERT_TRUE(tree->statements);
    ASSERT_EQ(tree->statements->size(), 1u);

    auto &stmt = (*tree->statements)[0];
    ASSERT_TRUE(std::holds_alternative<IfStatUPtr>(stmt->value));

    auto &ifst = std::get<IfStatUPtr>(stmt->value);

    EXPECT_EQ(ifst->condition->name, "check");
    ASSERT_TRUE(ifst->condition->args);
    ASSERT_EQ(ifst->condition->args->size(), 1u);

    ASSERT_TRUE(ifst->ifStmts);
    ASSERT_EQ(ifst->ifStmts->size(), 2u);

    EXPECT_TRUE(std::holds_alternative<FuncRunUPtr>((*ifst->ifStmts)[0]->value));
    EXPECT_TRUE(std::holds_alternative<AssignUPtr>((*ifst->ifStmts)[1]->value));
}

TEST(ParserTest, RecursiveIfBlock)
{
    AstParser parser(Path("recursive_if_block.txt"));
    auto tree = parser.ParseFileTree();

    ASSERT_TRUE(tree);
    ASSERT_TRUE(tree->statements);
    ASSERT_EQ(tree->statements->size(), 1u);

    auto &stmt = (*tree->statements)[0];
    ASSERT_TRUE(std::holds_alternative<IfStatUPtr>(stmt->value));
    auto &ifst = std::get<IfStatUPtr>(stmt->value);

    // Проверка условия верхнего if
    EXPECT_EQ(ifst->condition->name, "check_status");
    ASSERT_TRUE(ifst->condition->args);
    ASSERT_EQ(ifst->condition->args->size(), 1u);
    EXPECT_TRUE(std::holds_alternative<float>((*ifst->condition->args)[0]->value));

    // Проверка тела верхнего if
    ASSERT_TRUE(ifst->ifStmts);
    ASSERT_EQ(ifst->ifStmts->size(), 4u);

    // log("start")
    EXPECT_TRUE(std::holds_alternative<FuncRunUPtr>((*ifst->ifStmts)[0]->value));
    auto &logCall = std::get<FuncRunUPtr>((*ifst->ifStmts)[0]->value);
    EXPECT_EQ(logCall->fCall->name, "log");
    ASSERT_EQ(logCall->fCall->args->size(), 1u);
    EXPECT_TRUE(std::holds_alternative<std::string>((*logCall->fCall->args)[0]->value));

    // x = compute(3.14)
    EXPECT_TRUE(std::holds_alternative<AssignUPtr>((*ifst->ifStmts)[1]->value));
    auto &assignX = std::get<AssignUPtr>((*ifst->ifStmts)[1]->value);
    EXPECT_EQ(assignX->ident, "x");
    EXPECT_EQ(assignX->right->name, "compute");
    ASSERT_EQ(assignX->right->args->size(), 1u);
    EXPECT_TRUE(std::holds_alternative<float>((*assignX->right->args)[0]->value));

    // if x_greater(x, 10):
    EXPECT_TRUE(std::holds_alternative<IfStatUPtr>((*ifst->ifStmts)[2]->value));
    auto &nestedIf = std::get<IfStatUPtr>((*ifst->ifStmts)[2]->value);
    EXPECT_EQ(nestedIf->condition->name, "x_greater");
    ASSERT_EQ(nestedIf->condition->args->size(), 2u);

    EXPECT_TRUE(std::holds_alternative<std::string>((*nestedIf->condition->args)[0]->value));
    EXPECT_TRUE(std::holds_alternative<float>((*nestedIf->condition->args)[1]->value));

    // Проверка тела вложенного if
    ASSERT_TRUE(nestedIf->ifStmts);
    ASSERT_EQ(nestedIf->ifStmts->size(), 1u);

    EXPECT_TRUE(std::holds_alternative<MethodRunUPtr>((*nestedIf->ifStmts)[0]->value));
    auto &methodCall = std::get<MethodRunUPtr>((*nestedIf->ifStmts)[0]->value);
    EXPECT_EQ(methodCall->object, "obj");
    EXPECT_EQ(methodCall->call->name, "update");
    ASSERT_EQ(methodCall->call->args->size(), 2u);
    EXPECT_TRUE(std::holds_alternative<std::string>((*methodCall->call->args)[1]->value));

    // y = calc_value("test", 7)
    EXPECT_TRUE(std::holds_alternative<AssignUPtr>((*ifst->ifStmts)[3]->value));
    auto &assignY = std::get<AssignUPtr>((*ifst->ifStmts)[3]->value);
    EXPECT_EQ(assignY->ident, "y");
    EXPECT_EQ(assignY->right->name, "calc_value");
    ASSERT_EQ(assignY->right->args->size(), 2u);
    EXPECT_TRUE(std::holds_alternative<std::string>((*assignY->right->args)[0]->value));
    EXPECT_TRUE(std::holds_alternative<float>((*assignY->right->args)[1]->value));
}

TEST(ParserTest, IfElseBlock)
{
    AstParser parser(Path("if_else_block.txt"));
    auto tree = parser.ParseFileTree();

    ASSERT_TRUE(tree);
    ASSERT_TRUE(tree->statements);
    ASSERT_EQ(tree->statements->size(), 1u);

    auto &stmt = (*tree->statements)[0];
    ASSERT_TRUE(std::holds_alternative<IfStatUPtr>(stmt->value));
    auto &ifst = std::get<IfStatUPtr>(stmt->value);

    EXPECT_EQ(ifst->condition->name, "is_ready");
    ASSERT_EQ(ifst->condition->args->size(), 1u);
    EXPECT_TRUE(std::holds_alternative<float>((*ifst->condition->args)[0]->value));

    ASSERT_TRUE(ifst->ifStmts);
    ASSERT_EQ(ifst->ifStmts->size(), 2u);

    EXPECT_TRUE(std::holds_alternative<FuncRunUPtr>((*ifst->ifStmts)[0]->value));
    auto &okLog = std::get<FuncRunUPtr>((*ifst->ifStmts)[0]->value);
    EXPECT_EQ(okLog->fCall->name, "log_ok");

    EXPECT_TRUE(std::holds_alternative<AssignUPtr>((*ifst->ifStmts)[1]->value));
    auto &assignA = std::get<AssignUPtr>((*ifst->ifStmts)[1]->value);
    EXPECT_EQ(assignA->ident, "a");
    EXPECT_EQ(assignA->right->name, "compute");

    ASSERT_TRUE(ifst->elseStmts);
    ASSERT_EQ(ifst->elseStmts->size(), 1u);

    EXPECT_TRUE(std::holds_alternative<FuncRunUPtr>((*ifst->elseStmts)[0]->value));
    auto &failLog = std::get<FuncRunUPtr>((*ifst->elseStmts)[0]->value);
    EXPECT_EQ(failLog->fCall->name, "log_fail");
}

TEST(ParserTest, FuncCallAllArgTypes)
{
    AstParser parser(Path("all_types_args.txt"));
    auto tree = parser.ParseFileTree();

    ASSERT_TRUE(tree);
    ASSERT_TRUE(tree->statements);
    ASSERT_EQ(tree->statements->size(), 1u);

    auto &stmt = (*tree->statements)[0];
    ASSERT_TRUE(std::holds_alternative<AssignUPtr>(stmt->value));

    auto &assign = std::get<AssignUPtr>(stmt->value);
    EXPECT_EQ(assign->ident, "var");
    EXPECT_EQ(assign->right->name, "func");

    ASSERT_TRUE(assign->right->args);
    ASSERT_EQ(assign->right->args->size(), 6u);

    auto &args = *assign->right->args;

    EXPECT_TRUE(std::holds_alternative<float>(args[0]->value));
    EXPECT_EQ(std::get<float>(args[0]->value), 0.0f);

    EXPECT_TRUE(std::holds_alternative<float>(args[1]->value));
    EXPECT_EQ(std::get<float>(args[1]->value), 0.5f);

    EXPECT_TRUE(std::holds_alternative<std::string>(args[2]->value));
    EXPECT_EQ(std::get<std::string>(args[2]->value), "str1");

    EXPECT_TRUE(std::holds_alternative<std::string>(args[3]->value));
    EXPECT_EQ(std::get<std::string>(args[3]->value), "str2");

    EXPECT_TRUE(std::holds_alternative<bool>(args[4]->value));
    EXPECT_EQ(std::get<bool>(args[4]->value), true);

    EXPECT_TRUE(std::holds_alternative<bool>(args[5]->value));
    EXPECT_EQ(std::get<bool>(args[5]->value), false);
}

TEST(ParserErrorTest, AssignMissing)
{
    AstParser parser(InvalidScriptPath("assign_missing.txt"));

    try
    {
        parser.ParseFileTree();
        FAIL() << "Expected UnexpectedTokenExc";
    }
    catch (const std::exception &exc)
    {
        std::string msg = exc.what();

        EXPECT_NE(msg.find("func_call"), std::string::npos);
        EXPECT_NE(msg.find("line 2"), std::string::npos);
    }
    catch (...)
    {
        FAIL() << "Expected UnexpectedTokenExc";
    }
}

TEST(ParserErrorTest, IfMissingBlock)
{
    AstParser parser(InvalidScriptPath("if_missing_block.txt"));

    try
    {
        parser.ParseFileTree();
        FAIL() << "Expected UnexpectedTokenExc";
    }
    catch (const std::exception &exc)
    {
        std::string msg = exc.what();
        EXPECT_NE(msg.find("block"), std::string::npos);
        EXPECT_NE(msg.find("line 2"), std::string::npos);
    }
    catch (...)
    {
        FAIL() << "Expected UnexpectedTokenExc";
    }
}

TEST(ParserErrorTest, ElseWrongIndent)
{
    AstParser parser(InvalidScriptPath("else_wrong_indent.txt"));

    try
    {
        parser.ParseFileTree();
        FAIL() << "Expected UnexpectedTokenExc";
    }
    catch (const std::exception &exc)
    {
        std::string msg = exc.what();
        EXPECT_NE(msg.find("indend"), std::string::npos);
        EXPECT_NE(msg.find("line 3"), std::string::npos);
    }
    catch (...)
    {
        FAIL() << "Expected UnexpectedTokenExc";
    }
}

TEST(ParserErrorTest, FuncArgsMissing)
{
    AstParser parser(InvalidScriptPath("args_missing.txt"));

    try
    {
        parser.ParseFileTree();
        FAIL() << "Expected UnexpectedTokenExc";
    }
    catch (const std::exception &exc)
    {
        std::string msg = exc.what();
        EXPECT_NE(msg.find("argument"), std::string::npos);
        EXPECT_EQ(msg.find("arguments"), std::string::npos);
        EXPECT_NE(msg.find("line 1"), std::string::npos);
    }
    catch (...)
    {
        FAIL() << "Expected UnexpectedTokenExc";
    }
}
