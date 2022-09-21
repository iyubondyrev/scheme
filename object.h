#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
#include "error.h"

class Object;

class Scope;

class Heap {
private:
    std::vector<std::unique_ptr<Object>> storage_;
    Heap(){};

    static std::unique_ptr<Heap> heap;

public:
    template <class T, class... Args>
    Object* Allocate(Args&&... args) {
        storage_.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        return storage_.back().get();
    }

    void CleanUp(const std::unordered_set<Object*>& save_us);

    static Heap* Instance();
};

// Main Object class
class Object {
public:
    virtual ~Object() = default;
    virtual Object* Eval(Scope* scope) = 0;
    virtual std::string GetStringForm(bool inner_call = false) = 0;
};

// Helpers
//----------------------------------------------------------------------------------
bool IsList(Object* obj);

bool IsPair(Object* obj);

Object* GetElem(Object* obj, size_t ind);

size_t GetSize(Object* obj);

Object* Append(Object* to, Object* elem);

bool IsNumber(const std::string& str);

template <class T>
T* As(Object* obj) {
    T* res = dynamic_cast<T*>(obj);
    return res;
}

template <class T>
bool Is(Object* obj) {
    if (obj == nullptr) {
        return false;
    }
    if (dynamic_cast<T*>(obj)) {
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------------

// Functions
class Function : public Object {
public:
    Object* Eval(Scope* scope) override {
        return this;
    }
    virtual Object* Apply(Scope* scope, Object* obj) = 0;
};

// Arithmetics
class Sum : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class Mult : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class Sub : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class Div : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class Abs : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

// Compare
class Eq : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class Greater : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class Less : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class LessOrEq : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class GreaterOrEq : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class Min : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class Max : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

// Predicates
class SymbolPredicate : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class ListPredicate : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class PairPredicate : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class BooleanPredicate : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class NumberPredicate : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class NullPredicate : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

// Logic
class Not : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class And : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class Or : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

// List & Pair operations
class MakeList : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class MakePair : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class ListRef : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class ListTail : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class Car : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class Cdr : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

// Quote
class Quote : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class QuoteShort : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

// Define & Set
class Define : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class Set : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class SetCar : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

class SetCdr : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

// If
class IfStatement : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};

// Objects
class Number : public Object {
private:
    int value_;

public:
    explicit Number(int value);

    explicit Number(const std::string& value_string);

    Object* Eval(Scope* scope) override;

    std::string GetStringForm(bool inner_call = false) override;

    int GetValue() const;
};

class Symbol : public Object {
private:
    std::string name_;

public:
    explicit Symbol(const std::string& string);

    Object* Eval(Scope* scope) override;

    std::string GetStringForm(bool inner_call = false) override;

    const std::string& GetName() const;
};

class Boolean : public Object {
private:
    std::string value_;

public:
    explicit Boolean(const std::string& value);

    explicit Boolean(bool value);

    Object* Eval(Scope* scope) override;

    std::string GetStringForm(bool inner_call = false) override;

    bool GetValue();
};

class Cell : public Object {
private:
    Object* first_;
    Object* second_;

public:
    std::string GetStringForm(bool inner_call = false) override;

    Cell(Object* first, Object* second) : first_(first), second_(second) {
    }

    void SetFirst(Object* obj) {
        first_ = obj;
    }

    void SetSecond(Object* obj) {
        second_ = obj;
    }

    Object* GetFirst() const {
        if (!first_) {
            return nullptr;
        }
        return first_;
    };

    Object* GetSecond() const {
        if (!second_) {
            return nullptr;
        }
        return second_;
    };

    Object* Eval(Scope* scope) override;
};

class Scope {
private:
    Scope* parent_scope_ = nullptr;
    std::map<std::string, Object*> objects_;

public:
    Scope(Scope* parent_scope = nullptr) : parent_scope_(parent_scope) {
    }

    Object* LookUpLocal(const std::string& name);

    Object* LookUp(const std::string& name);

    Object* LookUpAbove(const std::string& name);

    Object* LookUpInParent(const std::string& name);

    void Define(const std::string& name, Object* obj);

    void Reset(const std::string& name, Object* obj);

    void AddCellTree(Object* cell, std::unordered_set<Object*>& set);

    Scope* GetParent() {
        return parent_scope_;
    }

    std::unordered_set<Object*> GetSaveList(Object* lambda_ptr = nullptr);

    const std::map<std::string, Object*>& GetMap() const {
        return objects_;
    }
};

// Lambdas
class Lambda : public Function {
private:
    std::vector<std::string> args_;
    std::string name_;
    Scope local_scope_;
    Object* action_;

public:
    Lambda(Scope* parent_scope, const std::vector<std::string>& args, Object* action,
           const std::string& name = "")
        : args_(args), name_(name), local_scope_(parent_scope), action_(action){};

    std::string GetStringForm(bool inner_call = false) override {
        return "";
    }

    Object* Apply(Scope* scope, Object* obj) override;

    Object* GetAction() {
        return action_;
    }

    size_t GetArgsNum() {
        return args_.size();
    }

    Scope* GetLocalScope() {
        return &local_scope_;
    }
};

class MakeLambda : public Function {
public:
    std::string GetStringForm(bool inner_call = false) override;

    Object* Apply(Scope* scope, Object* obj) override;
};