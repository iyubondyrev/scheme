#include "object.h"

// Number impl
Number::Number(int value) : value_(value){};

Number::Number(const std::string& value_string) : value_(std::stoi(value_string)){};

Object* Number::Eval(Scope* scope) {
    return Heap::Instance()->Allocate<Number>(this->GetValue());
}

std::string Number::GetStringForm(bool) {
    return std::to_string(value_);
}

int Number::GetValue() const {
    return value_;
}

// Symbol impl
Symbol::Symbol(const std::string& string) : name_(string) {
}

Object* Symbol::Eval(Scope* scope) {
    auto obj = scope->LookUp(name_);
    if (obj == nullptr) {
        throw NameError("No such name");
    }
    return obj;
}

std::string Symbol::GetStringForm(bool) {
    return name_;
}

const std::string& Symbol::GetName() const {
    return name_;
}

// Boolean impl
Boolean::Boolean(bool value) : value_(value ? "#t" : "#f") {
}

Boolean::Boolean(const std::string& value) : value_(value) {
}

Object* Boolean::Eval(Scope* scope) {
    return Heap::Instance()->Allocate<Boolean>(value_);
}

std::string Boolean::GetStringForm(bool) {
    return value_;
}

bool Boolean::GetValue() {
    return value_ == "#t";
}

// Cell impl
Object* Cell::Eval(Scope* scope) {
    if (GetFirst() == nullptr) {
        throw RuntimeError("");
    }
    auto func = GetFirst()->Eval(scope);
    if (!Is<Function>(func)) {
        throw RuntimeError("Not a func");
    }
    if (Is<Lambda>(func)) {
        if (scope->GetParent()) {
            Scope new_scope(scope);
            return As<Function>(func)->Apply(&new_scope, GetSecond());
        } else {
            return As<Function>(func)->Apply(As<Lambda>(func)->GetLocalScope(), GetSecond());
        }
    }
    return As<Function>(func)->Apply(scope, GetSecond());
}

std::string Cell::GetStringForm(bool inner_call) {
    std::string result;
    if (!inner_call) {
        result += "(";
    }
    if (GetFirst() == nullptr) {
        result += "()";
    } else {
        if (!Is<Cell>(GetFirst())) {
            result += GetFirst()->GetStringForm(true);
        } else {
            if (GetSecond() != nullptr) {
                result += "(" + GetFirst()->GetStringForm(true) + ")";
            } else {
                result += GetFirst()->GetStringForm(true);
            }
        }
    }
    if (GetSecond() == nullptr) {
        if (!inner_call) {
            result += ")";
        }
        return result;
    }
    if (!Is<Cell>(GetSecond())) {
        result += " . " + GetSecond()->GetStringForm(true);
    } else {
        result += " " + GetSecond()->GetStringForm(true);
    }
    if (!inner_call) {
        result += ")";
    }
    return result;
}

// Heap impl
void Heap::CleanUp(const std::unordered_set<Object*>& save_us) {
    std::vector<std::unique_ptr<Object>> new_storage;
    for (auto& object : storage_) {
        if (save_us.contains(object.get())) {
            new_storage.emplace_back(object.release());
        }
    }
    storage_ = std::move(new_storage);
}

std::unique_ptr<Heap> Heap::heap = nullptr;

Heap* Heap::Instance() {
    if (heap == nullptr) {
        heap.reset(new Heap{});
    }
    return heap.get();
}

// Scope impl
Object* Scope::LookUpLocal(const std::string& name) {
    if (!objects_.contains(name)) {
        return nullptr;
    }
    return objects_[name];
}

Object* Scope::LookUp(const std::string& name) {
    if (objects_.contains(name)) {
        return objects_[name];
    }
    if (parent_scope_ == nullptr) {
        return nullptr;
    }
    return parent_scope_->LookUp(name);
}

Object* Scope::LookUpAbove(const std::string& name) {
    if (parent_scope_ == nullptr) {
        return nullptr;
    }
    return parent_scope_->LookUp(name);
}

Object* Scope::LookUpInParent(const std::string& name) {
    if (parent_scope_ == nullptr) {
        return nullptr;
    }
    return parent_scope_->LookUpLocal(name);
}

void Scope::Define(const std::string& name, Object* obj) {
    objects_[name] = obj;
}

void Scope::Reset(const std::string& name, Object* obj) {
    if (!objects_.contains(name)) {
        throw RuntimeError(name + " is not defined");
    }
    objects_[name] = obj;
}

void Scope::AddCellTree(Object* cell, std::unordered_set<Object*>& set) {
    auto left = As<Cell>(cell)->GetFirst();
    auto right = As<Cell>(cell)->GetSecond();
    set.insert(left);
    set.insert(right);
    if (Is<Cell>(left)) {
        if (left != cell) {
            AddCellTree(left, set);
        }
    }
    if (Is<Cell>(right)) {
        if (right != cell) {
            AddCellTree(right, set);
        }
    }
}

std::unordered_set<Object*> Scope::GetSaveList(Object* lambda_ptr) {
    std::unordered_set<Object*> res;
    for (auto& pair : objects_) {
        auto ptr = pair.second;
        res.insert(ptr);
        if (Is<Lambda>(ptr) && ptr != lambda_ptr) {
            auto action = As<Lambda>(ptr)->GetAction();

            auto local_scope = As<Lambda>(ptr)->GetLocalScope();
            std::set<Object*> local_variables;
            while (local_scope->GetParent() != nullptr) {
                for (auto& other_pair : local_scope->GetMap()) {
                    local_variables.insert(other_pair.second);
                }
                local_scope = local_scope->GetParent();
            }
            ptr = action;
            res.insert(ptr);
            res.insert(local_variables.begin(), local_variables.end());
        }
        if (Is<Cell>(ptr)) {
            AddCellTree(ptr, res);
        }
    }
    return res;
}