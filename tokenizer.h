#pragma once
#include "error.h"
#include <variant>
#include <optional>
#include <istream>
#include <stdexcept>
#include <exception>

struct SymbolToken {
    std::string name;

    bool operator==(const SymbolToken& other) const {
        return name == other.name;
    };

    bool operator!=(const SymbolToken& other) const {
        return name == other.name;
    };
};

struct QuoteToken {
    bool operator==(const QuoteToken&) const {
        return true;
    };

    bool operator!=(const QuoteToken&) const {
        return false;
    };
};

struct DotToken {
    bool operator==(const DotToken&) const {
        return true;
    };

    bool operator!=(const DotToken&) const {
        return false;
    };
};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
    int value;

    bool operator==(const ConstantToken& other) const {
        return value == other.value;
    };

    bool operator!=(const ConstantToken& other) const {
        return value != other.value;
    };
};

using Token = std::variant<ConstantToken, BracketToken, SymbolToken, QuoteToken, DotToken>;

class Tokenizer {
private:
    static bool IsCorrectCharToStart(char ch) {
        if (std::isupper(ch) || std::islower(ch) || ch == '<' || ch == '>' || ch == '=' ||
            ch == '/' || ch == '*' || ch == '#') {
            return true;
        }
        return false;
    }

    static bool IsCorrectCharToContinue(char ch) {
        if (std::isupper(ch) || std::islower(ch) || std::isdigit(ch) || ch == '<' || ch == '>' ||
            ch == '/' || ch == '=' || ch == '*' || ch == '#' || ch == '?' || ch == '!' ||
            ch == '-') {
            return true;
        }
        return false;
    }

    void GetInt(std::string& storage) {
        while (std::isdigit(char(in_->peek()))) {
            char digit;
            in_->get(digit);
            storage.push_back(digit);
        }
    }

    void GetSymbol(std::string& storage) {
        while (in_->peek() != -1 && !std::isspace(char(in_->peek())) && in_->peek() != int('(') &&
               in_->peek() != int(')')) {
            char ch;
            in_->get(ch);
            if (!IsCorrectCharToContinue(ch)) {
                throw SyntaxError("bad_token");
            }
            storage.push_back(ch);
        }
    }

    void SkipSpaces() {
        while (std::isspace(char(in_->peek()))) {
            in_->get();
        }
    }

public:
    Tokenizer() = default;

    Tokenizer(std::istream* in) : in_(in) {
        Next();
    }

    void Reset(std::istream* in) {
        in_ = in;
        is_end_ = false;
        Next();
    }

    bool IsEnd() {
        return is_end_;
    }

    void Next() {
        SkipSpaces();
        if (in_->peek() == -1) {
            is_end_ = true;
            return;
        }
        if (in_->peek() == int('(') || in_->peek() == int(')')) {
            char bracket;
            in_->get(bracket);
            cur_token_ = bracket == ')' ? BracketToken::CLOSE : BracketToken::OPEN;
            return;
        }
        if (in_->peek() == int('.')) {
            char dot;
            in_->get(dot);
            cur_token_ = DotToken{};
            return;
        }
        if (in_->peek() == int('\'')) {
            char quote;
            in_->get(quote);
            cur_token_ = QuoteToken{};
            return;
        }
        if (std::isdigit(char(in_->peek()))) {
            std::string value_str;
            GetInt(value_str);
            int value = std::stoi(value_str);
            cur_token_ = ConstantToken{value};
            return;
        }
        if (char(in_->peek()) == '-' || char(in_->peek()) == '+') {
            char sign;
            in_->get(sign);
            std::string value_str;
            value_str.push_back(sign);
            if (!std::isdigit(in_->peek())) {
                cur_token_ = SymbolToken{value_str};
                return;
            }
            GetInt(value_str);
            int value_int = std::stoi(value_str);
            cur_token_ = ConstantToken{value_int};
            return;
        }
        if (!IsCorrectCharToStart(char(in_->peek()))) {
            throw SyntaxError("Bad token");
        }
        std::string symbol;
        GetSymbol(symbol);
        cur_token_ = SymbolToken{symbol};
    }

    Token GetToken() {
        return cur_token_;
    };

    bool is_end_ = false;
    Token cur_token_;
    std::istream* in_;
};