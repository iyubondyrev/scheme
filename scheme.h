#pragma once
#include "tokenizer.h"
#include "parser.h"
#include <string>
#include <sstream>
#include <memory>
#include <map>

class Interpreter {
private:
    Tokenizer tokenizer_;
    Heap* heap_;
    Scope global_scope_;
    int cnt_ = 0;

public:
    Interpreter() : heap_(Heap::Instance()) {
        global_scope_.Define("+", heap_->Allocate<Sum>());
        global_scope_.Define("*", heap_->Allocate<Mult>());
        global_scope_.Define("-", heap_->Allocate<Sub>());
        global_scope_.Define("/", heap_->Allocate<Div>());
        global_scope_.Define("abs", heap_->Allocate<Abs>());
        global_scope_.Define("=", heap_->Allocate<Eq>());
        global_scope_.Define(">", heap_->Allocate<Greater>());
        global_scope_.Define("<", heap_->Allocate<Less>());
        global_scope_.Define(">=", heap_->Allocate<GreaterOrEq>());
        global_scope_.Define("<=", heap_->Allocate<LessOrEq>());
        global_scope_.Define("min", heap_->Allocate<Min>());
        global_scope_.Define("max", heap_->Allocate<Max>());
        global_scope_.Define("list?", heap_->Allocate<ListPredicate>());
        global_scope_.Define("pair?", heap_->Allocate<PairPredicate>());
        global_scope_.Define("boolean?", heap_->Allocate<BooleanPredicate>());
        global_scope_.Define("null?", heap_->Allocate<NullPredicate>());
        global_scope_.Define("number?", heap_->Allocate<NumberPredicate>());
        global_scope_.Define("not", heap_->Allocate<Not>());
        global_scope_.Define("and", heap_->Allocate<And>());
        global_scope_.Define("or", heap_->Allocate<Or>());
        global_scope_.Define("list", heap_->Allocate<MakeList>());
        global_scope_.Define("list-ref", heap_->Allocate<ListRef>());
        global_scope_.Define("list-tail", heap_->Allocate<ListTail>());
        global_scope_.Define("cons", heap_->Allocate<MakePair>());
        global_scope_.Define("car", heap_->Allocate<Car>());
        global_scope_.Define("cdr", heap_->Allocate<Cdr>());
        global_scope_.Define("'", heap_->Allocate<QuoteShort>());
        global_scope_.Define("quote", heap_->Allocate<Quote>());
        global_scope_.Define("symbol?", heap_->Allocate<SymbolPredicate>());
        global_scope_.Define("define", heap_->Allocate<Define>());
        global_scope_.Define("set!", heap_->Allocate<Set>());
        global_scope_.Define("set-car!", heap_->Allocate<SetCar>());
        global_scope_.Define("set-cdr!", heap_->Allocate<SetCdr>());
        global_scope_.Define("if", heap_->Allocate<IfStatement>());
        global_scope_.Define("lambda", heap_->Allocate<MakeLambda>());
    }

    std::string Run(const std::string& expression) {
        ++cnt_;
        std::stringstream ss{expression};
        tokenizer_.Reset(&ss);
        auto root = Read(&tokenizer_);
        if (root == nullptr) {
            throw RuntimeError("");
        }
        auto res = root->Eval(&global_scope_);
        if (res == nullptr) {
            return "()";
        }
        auto ans = res->GetStringForm();
        auto save_list = global_scope_.GetSaveList();
        heap_->CleanUp(save_list);
        return ans;
    }
};