#include "Python3Lexer.h"
#include "Python3Parser.h"

Python3LexerBase::Python3LexerBase(antlr4::CharStream *input) : Lexer(input) {
  tokens = new std::list<antlr4::Token*>();
  indents = new std::stack<int>();
  lastToken = nullptr;
}

Python3LexerBase::~Python3LexerBase() {
  for (auto i = tokens -> begin(); i != tokens -> end(); i ++) {
    delete *i;
  }

  delete tokens;
  delete indents;
}

bool Python3LexerBase::atStartOfInput() {
  return getCharPositionInLine() == 0 && getLine() == 1;
}

void Python3LexerBase::emit(std::unique_ptr<antlr4::Token> newToken) {
  auto copy = new antlr4::CommonToken(newToken.get());
  tokens -> push_back(copy);
  antlr4::Lexer::emit(std::move(newToken));
}

void Python3LexerBase::emitp(antlr4::Token* newToken) {
  auto copy = new antlr4::CommonToken(newToken);
  tokens -> push_back(copy);

  auto uniqueToken = std::unique_ptr<antlr4::Token>(newToken);
  antlr4::Lexer::emit(std::move(uniqueToken));
}

std::unique_ptr<antlr4::Token> Python3LexerBase::nextToken() {
  // Check if the end-of-file is ahead and there are still some DEDENTS expected.
  if (_input -> LA(1) == Python3Parser::EOF && !indents -> empty()) {
    // Remove any trailing EOF tokens from our buffer.
    tokens -> remove_if(
      [](auto&& cur) {
        return cur -> getType() == Python3Parser::EOF;
      }
    );

    // // First emit an extra line break that serves as the end of the statement.
    emitp(commonToken(Python3Parser::NEWLINE, "\n"));

    // Now emit as much DEDENT tokens as needed.
    while (!indents -> empty()) {
      emitp(createDedent());
      indents -> pop();
    }

    // // Put the EOF back on the token stream.
    emitp(commonToken(Python3Parser::EOF, "<EOF>"));
  }

  auto next = antlr4::Lexer::nextToken();

  if (next.get() -> getChannel() == antlr4::Token::DEFAULT_CHANNEL) {
    // Keep track of the last token on the default channel.
    lastToken = new antlr4::CommonToken(next.get());
  }

  if (tokens -> empty()) {
    return next;
   } else {
    auto frontToken = std::unique_ptr<antlr4::Token>(tokens -> front());
    tokens -> pop_front();
    return frontToken;
  }
}

antlr4::CommonToken* Python3LexerBase::commonToken(size_t type, std::string text) {
  int stop = getCharIndex() - 1;
  int start = text.empty() ? stop : stop - text.length() + 1;
  return new antlr4::CommonToken({ this, _input }, type, antlr4::Lexer::DEFAULT_TOKEN_CHANNEL, start, stop);
}

antlr4::CommonToken* Python3LexerBase::createDedent() {
  auto dedent = commonToken(Python3Parser::DEDENT, "");
  dedent -> setLine(lastToken -> getLine());
  return dedent;
}

// Calculates the indentation of the provided spaces, taking the
// following rules into account:
//
// "Tabs are replaced (from left to right) by one to eight spaces
//  such that the total number of characters up to and including
//  the replacement is a multiple of eight [...]"
//
//  -- https://docs.python.org/3.1/reference/lexical_analysis.html#indentation
int Python3LexerBase::getIndentationCount(std::string spaces) {
  int count = 0;

  for (int i = 0; i < spaces.length(); i ++) {
    switch (spaces.at(i)) {
      case '\t':
        count += 8 - (count % 8);
        break;

      default:
        // A normal space char.
        count ++;
    }
  }

  return count;
}
