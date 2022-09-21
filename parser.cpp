#include "parser.h"

Object* ReadToken(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        throw SyntaxError("Unexpected end");
    }
    auto cur_token = tokenizer->GetToken();
    tokenizer->Next();
    if (cur_token == Token{BracketToken::OPEN}) {
        auto res = ReadList(tokenizer);
        if (tokenizer->IsEnd()) {
            throw SyntaxError("Unexpected end");
        }
        cur_token = tokenizer->GetToken();
        if (cur_token != Token{BracketToken::CLOSE}) {
            throw SyntaxError("Bad list");
        }
        tokenizer->Next();
        return res;
    }
    if (cur_token == Token{BracketToken::CLOSE}) {
        throw SyntaxError("Bad brackets");
    }
    if (auto num_token = std::get_if<ConstantToken>(&cur_token)) {
        return Heap::Instance()->Allocate<Number>(num_token->value);
    }
    if (auto symbol_token = std::get_if<SymbolToken>(&cur_token)) {
        if (symbol_token->name == "#t" || symbol_token->name == "#f") {
            return Heap::Instance()->Allocate<Boolean>(symbol_token->name);
        }
        return Heap::Instance()->Allocate<Symbol>(symbol_token->name);
    }
    if (auto dot_token = std::get_if<DotToken>(&cur_token)) {
        return Heap::Instance()->Allocate<Symbol>(".");
    }
    if (auto quote_token = std::get_if<QuoteToken>(&cur_token)) {
        auto first = Heap::Instance()->Allocate<Symbol>("'");
        auto second = ReadToken(tokenizer);
        return Heap::Instance()->Allocate<Cell>(first, second);
    }
    return nullptr;
}

Object* Read(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        throw SyntaxError("Unexpected end");
    }
    Object* res;
    auto cur_token = tokenizer->GetToken();
    if (cur_token == Token{DotToken()}) {
        throw SyntaxError("Bad dot");
    }
    while (!tokenizer->IsEnd()) {
        res = ReadToken(tokenizer);
    }
    return res;
}

Object* ReadList(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        throw SyntaxError("Bad brackets");
    }
    auto cur_token = tokenizer->GetToken();
    if (cur_token == Token{BracketToken::CLOSE}) {
        return nullptr;
    }
    if (cur_token == Token{DotToken()}) {
        throw SyntaxError("");
    }
    auto left = ReadToken(tokenizer);
    cur_token = tokenizer->GetToken();
    Object* right;
    if (cur_token != Token{DotToken{}}) {
        right = ReadList(tokenizer);
    } else {
        tokenizer->Next();
        right = ReadToken(tokenizer);
    }
    return Heap::Instance()->Allocate<Cell>(left, right);
}