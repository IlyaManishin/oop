#include "file_parser/parser.hpp"

#include <gtest/gtest.h>

using namespace file_parser;

static std::string P(const std::string &name)
{
    return "./tests_data/file_parser/" + name;
}

TEST(ParserTest, SimpleAssign)
{
    AstParser parser(P("simple_assign.txt"));
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
    AstParser parser(P("func_and_method.txt"));
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
    AstParser parser(P("if_block.txt"));
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

    ASSERT_TRUE(ifst->statements);
    ASSERT_EQ(ifst->statements->size(), 2u);

    EXPECT_TRUE(std::holds_alternative<FuncRunUPtr>((*ifst->statements)[0]->value));
    EXPECT_TRUE(std::holds_alternative<AssignUPtr>((*ifst->statements)[1]->value));
}

TEST(ParserTest, RecursiveIfBlock)
{
    AstParser parser(P("complex_recursive.txt"));
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
    ASSERT_TRUE(ifst->statements);
    ASSERT_EQ(ifst->statements->size(), 4u);

    // log("start")
    EXPECT_TRUE(std::holds_alternative<FuncRunUPtr>((*ifst->statements)[0]->value));
    auto &logCall = std::get<FuncRunUPtr>((*ifst->statements)[0]->value);
    EXPECT_EQ(logCall->fCall->name, "log");
    ASSERT_EQ(logCall->fCall->args->size(), 1u);
    EXPECT_TRUE(std::holds_alternative<std::string>((*logCall->fCall->args)[0]->value));

    // x = compute(3.14)
    EXPECT_TRUE(std::holds_alternative<AssignUPtr>((*ifst->statements)[1]->value));
    auto &assignX = std::get<AssignUPtr>((*ifst->statements)[1]->value);
    EXPECT_EQ(assignX->ident, "x");
    EXPECT_EQ(assignX->right->name, "compute");
    ASSERT_EQ(assignX->right->args->size(), 1u);
    EXPECT_TRUE(std::holds_alternative<float>((*assignX->right->args)[0]->value));

    // if x_greater(x, 10):
    EXPECT_TRUE(std::holds_alternative<IfStatUPtr>((*ifst->statements)[2]->value));
    auto &nestedIf = std::get<IfStatUPtr>((*ifst->statements)[2]->value);
    EXPECT_EQ(nestedIf->condition->name, "x_greater");
    ASSERT_EQ(nestedIf->condition->args->size(), 2u);

    EXPECT_TRUE(std::holds_alternative<std::string>((*nestedIf->condition->args)[0]->value));
    EXPECT_TRUE(std::holds_alternative<float>((*nestedIf->condition->args)[1]->value));

    // Проверка тела вложенного if
    ASSERT_TRUE(nestedIf->statements);
    ASSERT_EQ(nestedIf->statements->size(), 1u);

    EXPECT_TRUE(std::holds_alternative<MethodRunUPtr>((*nestedIf->statements)[0]->value));
    auto &methodCall = std::get<MethodRunUPtr>((*nestedIf->statements)[0]->value);
    EXPECT_EQ(methodCall->object, "obj");
    EXPECT_EQ(methodCall->call->name, "update");
    ASSERT_EQ(methodCall->call->args->size(), 2u);
    EXPECT_TRUE(std::holds_alternative<std::string>((*methodCall->call->args)[1]->value));

    // y = calc_value("test", 7)
    EXPECT_TRUE(std::holds_alternative<AssignUPtr>((*ifst->statements)[3]->value));
    auto &assignY = std::get<AssignUPtr>((*ifst->statements)[3]->value);
    EXPECT_EQ(assignY->ident, "y");
    EXPECT_EQ(assignY->right->name, "calc_value");
    ASSERT_EQ(assignY->right->args->size(), 2u);
    EXPECT_TRUE(std::holds_alternative<std::string>((*assignY->right->args)[0]->value));
    EXPECT_TRUE(std::holds_alternative<float>((*assignY->right->args)[1]->value));
}
