#include "file_parser/parser.hpp"

#include <gtest/gtest.h>

using namespace file_parser;

static std::string P(const std::string &name)
{
    return "./tests_data/file_parser/" + name;
}

TEST(ParserTest, SimpleAssign)
{
    Parser parser(P("simple_assign.txt"));
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
    Parser parser(P("func_and_method.txt"));
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
    Parser parser(P("if_block.txt"));
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
