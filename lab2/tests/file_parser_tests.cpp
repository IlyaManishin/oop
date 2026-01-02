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

    ASSERT_TRUE(std::holds_alternative<FuncCallUPtr>(assign->right->value));
    auto &funcCall = std::get<FuncCallUPtr>(assign->right->value);

    EXPECT_EQ(funcCall->name, "foo");
    ASSERT_TRUE(funcCall->args);
    EXPECT_EQ(funcCall->args->size(), 2u);
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

    auto &cond = ifst->condition;
    EXPECT_EQ(cond->name, "check_status");
    ASSERT_TRUE(cond->args);
    ASSERT_EQ(cond->args->size(), 1u);
    auto &arg0 = *(*cond->args)[0]; // ArgUPtr -> Arg
    EXPECT_EQ(arg0.type, file_parser::Arg::Type::NUMBER);
    EXPECT_FLOAT_EQ(std::get<float>(arg0.value), 42.0f); // пример для числа

    ASSERT_TRUE(ifst->ifStmts);
    ASSERT_EQ(ifst->ifStmts->size(), 4u);

    // log("start")
    auto &logCallStmt = (*ifst->ifStmts)[0];
    EXPECT_TRUE(std::holds_alternative<FuncRunUPtr>(logCallStmt->value));
    auto &logCall = std::get<FuncRunUPtr>(logCallStmt->value);
    EXPECT_EQ(logCall->fCall->name, "log");
    ASSERT_EQ(logCall->fCall->args->size(), 1u);
    auto &logArg = *(*logCall->fCall->args)[0];
    EXPECT_TRUE(std::holds_alternative<std::string>(logArg.value));

    // x = compute(3.14)
    auto &assignXStmt = (*ifst->ifStmts)[1];
    EXPECT_TRUE(std::holds_alternative<AssignUPtr>(assignXStmt->value));
    auto &assignX = std::get<AssignUPtr>(assignXStmt->value);
    EXPECT_EQ(assignX->ident, "x");
    auto &rightX = std::get<FuncCallUPtr>(assignX->right->value);
    EXPECT_EQ(rightX->name, "compute");
    ASSERT_EQ(rightX->args->size(), 1u);
    auto &argX0 = *(*rightX->args)[0];
    EXPECT_TRUE(std::holds_alternative<float>(argX0.value));

    // if x_greater(x, 10):
    auto &nestedIfStmt = (*ifst->ifStmts)[2];
    EXPECT_TRUE(std::holds_alternative<IfStatUPtr>(nestedIfStmt->value));
    auto &nestedIf = std::get<IfStatUPtr>(nestedIfStmt->value);
    EXPECT_EQ(nestedIf->condition->name, "x_greater");
    ASSERT_EQ(nestedIf->condition->args->size(), 2u);
    auto &nestedArg0 = *(*nestedIf->condition->args)[0];
    auto &nestedArg1 = *(*nestedIf->condition->args)[1];
    EXPECT_TRUE(std::holds_alternative<std::string>(nestedArg0.value));
    EXPECT_TRUE(std::holds_alternative<float>(nestedArg1.value));

    ASSERT_TRUE(nestedIf->ifStmts);
    ASSERT_EQ(nestedIf->ifStmts->size(), 1u);
    auto &methodStmt = (*nestedIf->ifStmts)[0];
    EXPECT_TRUE(std::holds_alternative<MethodRunUPtr>(methodStmt->value));
    auto &methodCall = std::get<MethodRunUPtr>(methodStmt->value);
    EXPECT_EQ(methodCall->object, "obj");
    EXPECT_EQ(methodCall->call->name, "update");
    ASSERT_EQ(methodCall->call->args->size(), 2u);
    auto &methodArg1 = *(*methodCall->call->args)[1];
    EXPECT_TRUE(std::holds_alternative<std::string>(methodArg1.value));

    // y = calc_value("test", 7)
    auto &assignYStmt = (*ifst->ifStmts)[3];
    EXPECT_TRUE(std::holds_alternative<AssignUPtr>(assignYStmt->value));
    auto &assignY = std::get<AssignUPtr>(assignYStmt->value);
    EXPECT_EQ(assignY->ident, "y");
    auto &rightY = std::get<FuncCallUPtr>(assignY->right->value);
    EXPECT_EQ(rightY->name, "calc_value");
    ASSERT_EQ(rightY->args->size(), 2u);
    auto &argY0 = *(*rightY->args)[0];
    auto &argY1 = *(*rightY->args)[1];
    EXPECT_TRUE(std::holds_alternative<std::string>(argY0.value));
    EXPECT_TRUE(std::holds_alternative<float>(argY1.value));
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
    auto &condArg = *(*ifst->condition->args)[0];
    EXPECT_TRUE(std::holds_alternative<float>(condArg.value));

    ASSERT_TRUE(ifst->ifStmts);
    ASSERT_EQ(ifst->ifStmts->size(), 2u);

    auto &okLogStmt = (*ifst->ifStmts)[0];
    EXPECT_TRUE(std::holds_alternative<FuncRunUPtr>(okLogStmt->value));
    auto &okLog = std::get<FuncRunUPtr>(okLogStmt->value);
    EXPECT_EQ(okLog->fCall->name, "log_ok");

    auto &assignAStmt = (*ifst->ifStmts)[1];
    EXPECT_TRUE(std::holds_alternative<AssignUPtr>(assignAStmt->value));
    auto &assignA = std::get<AssignUPtr>(assignAStmt->value);
    EXPECT_EQ(assignA->ident, "a");
    auto &rightA = std::get<FuncCallUPtr>(assignA->right->value);
    EXPECT_EQ(rightA->name, "compute");

    ASSERT_TRUE(ifst->elseStmts);
    ASSERT_EQ(ifst->elseStmts->size(), 1u);
    auto &failLogStmt = (*ifst->elseStmts)[0];
    EXPECT_TRUE(std::holds_alternative<FuncRunUPtr>(failLogStmt->value));
    auto &failLog = std::get<FuncRunUPtr>(failLogStmt->value);
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

    ASSERT_TRUE(std::holds_alternative<FuncCallUPtr>(assign->right->value));
    auto &call = std::get<FuncCallUPtr>(assign->right->value);
    EXPECT_EQ(call->name, "func");

    ASSERT_TRUE(call->args);
    ASSERT_EQ(call->args->size(), 6u);

    auto &args = *call->args;

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

        EXPECT_NE(msg.find("expression"), std::string::npos);
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
