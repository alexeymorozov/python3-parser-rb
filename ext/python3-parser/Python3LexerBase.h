#pragma once

#include <list>
#include <stack>

#include "antlr4-runtime.h"

class Python3LexerBase : public antlr4::Lexer {
public:
  Python3LexerBase(antlr4::CharStream *input);
  ~Python3LexerBase();

  virtual void emit(std::unique_ptr<antlr4::Token> newToken) override;
  virtual std::unique_ptr<antlr4::Token> nextToken() override;

  bool atStartOfInput();

  static int getIndentationCount(std::string spaces);

protected:
  std::unique_ptr<antlr4::CommonToken> commonToken(size_t type, std::string text);
  std::unique_ptr<antlr4::CommonToken> createDedent();

  // A queue where extra tokens are pushed on (see the NEWLINE lexer rule).
  std::list<antlr4::Token*>* tokens;
  // The stack that keeps track of the indentation level.
  std::stack<int>* indents;
  // The amount of opened braces, brackets and parenthesis.
  int opened = 0;
  // The most recently produced token.
  antlr4::Token* lastToken;
};
