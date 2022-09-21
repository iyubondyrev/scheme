#include "object.h"

// Arithmetics
// "+"
std::string Sum::GetStringForm(bool) {
    return "+";
}

Object* Sum::Apply(Scope* scope, Object* obj) {
    int res = 0;
    if (obj == nullptr) {
        return Heap::Instance()->Allocate<Number>(res);
    }
    for (size_t i = 0; i < GetSize(obj); ++i) {
        auto list_elem = GetElem(obj, i);
        if (list_elem == nullptr) {
            throw RuntimeError("");
        }
        list_elem = list_elem->Eval(scope);
        if (!Is<Number>(list_elem)) {
            throw RuntimeError(list_elem->GetStringForm() + " is not a number");
        }
        res += As<Number>(list_elem)->GetValue();
    }
    return Heap::Instance()->Allocate<Number>(res);
}

// "*"
std::string Mult::GetStringForm(bool) {
    return "*";
}

Object* Mult::Apply(Scope* scope, Object* obj) {
    int res = 1;
    if (obj == nullptr) {
        return Heap::Instance()->Allocate<Number>(res);
    }
    for (size_t i = 0; i < GetSize(obj); ++i) {
        auto list_elem = GetElem(obj, i);
        if (list_elem == nullptr) {
            throw RuntimeError("");
        }
        list_elem = list_elem->Eval(scope);
        if (!Is<Number>(list_elem)) {
            throw RuntimeError(list_elem->GetStringForm() + " is not a number");
        }
        res *= As<Number>(list_elem)->GetValue();
    }
    return Heap::Instance()->Allocate<Number>(res);
}

// "-"
std::string Sub::GetStringForm(bool) {
    return "-";
}

Object* Sub::Apply(Scope* scope, Object* obj) {
    if (obj == nullptr) {
        throw RuntimeError("Nothing to sub");
    }
    int res;
    for (size_t i = 0; i < GetSize(obj); ++i) {
        auto list_elem = GetElem(obj, i);
        if (list_elem == nullptr) {
            throw RuntimeError("");
        }
        list_elem = list_elem->Eval(scope);
        if (!Is<Number>(list_elem)) {
            throw RuntimeError(list_elem->GetStringForm() + " is not a number");
        }
        if (i == 0) {
            res = As<Number>(list_elem)->GetValue();
            continue;
        }
        res -= As<Number>(list_elem)->GetValue();
    }
    return Heap::Instance()->Allocate<Number>(res);
}

// "/"
std::string Div::GetStringForm(bool) {
    return "-";
}

Object* Div::Apply(Scope* scope, Object* obj) {
    if (obj == nullptr) {
        throw RuntimeError("Nothing to div");
    }
    int res;
    for (size_t i = 0; i < GetSize(obj); ++i) {
        auto list_elem = GetElem(obj, i);
        if (list_elem == nullptr) {
            throw RuntimeError("");
        }
        list_elem = list_elem->Eval(scope);
        if (!Is<Number>(list_elem)) {
            throw RuntimeError(list_elem->GetStringForm() + " is not a number");
        }
        if (i == 0) {
            res = As<Number>(list_elem)->GetValue();
            continue;
        }
        res /= As<Number>(list_elem)->GetValue();
    }
    return Heap::Instance()->Allocate<Number>(res);
}

// "abs"
std::string Abs::GetStringForm(bool) {
    return "abs";
}

Object* Abs::Apply(Scope* scope, Object* obj) {
    if (GetSize(obj) != 1) {
        throw RuntimeError("To many(few) args");
    }
    auto number = GetElem(obj, 0);
    if (!Is<Number>(number)) {
        throw RuntimeError(number->GetStringForm() + " is not a number");
    }
    return Heap::Instance()->Allocate<Number>(std::abs(As<Number>(number)->GetValue()));
}

// Compare
// "="
std::string Eq::GetStringForm(bool) {
    return "=";
}

Object* Eq::Apply(Scope* scope, Object* obj) {
    if (obj == nullptr) {
        return Heap::Instance()->Allocate<Boolean>(true);
    }
    bool ans = true;
    size_t size = GetSize(obj);
    if (size == 1) {
        return Heap::Instance()->Allocate<Boolean>(true);
    }
    auto cur_object = GetElem(obj, 0)->Eval(scope);
    if (!Is<Number>(cur_object)) {
        throw RuntimeError(cur_object->GetStringForm() + " is not a number");
    }
    int cur_value = As<Number>(cur_object)->GetValue();
    for (size_t i = 0; i != size; ++i) {
        cur_object = GetElem(obj, i)->Eval(scope);
        if (!Is<Number>(cur_object)) {
            throw RuntimeError(cur_object->GetStringForm() + " is not a number");
        }
        if (cur_value != As<Number>(cur_object)->GetValue()) {
            ans = false;
            break;
        }
    }
    return Heap::Instance()->Allocate<Boolean>(ans);
}

// ">"
std::string Greater::GetStringForm(bool) {
    return ">";
}

Object* Greater::Apply(Scope* scope, Object* obj) {
    if (obj == nullptr) {
        return Heap::Instance()->Allocate<Boolean>(true);
    }
    bool ans = true;
    size_t size = GetSize(obj);
    if (size == 1) {
        return Heap::Instance()->Allocate<Boolean>(true);
    }
    auto cur_object = GetElem(obj, 0)->Eval(scope);
    if (!Is<Number>(cur_object)) {
        throw RuntimeError(cur_object->GetStringForm() + " is not a number");
    }
    int cur_value = As<Number>(cur_object)->GetValue();
    for (size_t i = 1; i != size; ++i) {
        cur_object = GetElem(obj, i)->Eval(scope);
        if (!Is<Number>(cur_object)) {
            throw RuntimeError(cur_object->GetStringForm() + " is not a number");
        }
        if (cur_value <= As<Number>(cur_object)->GetValue()) {
            ans = false;
            break;
        }
        cur_value = As<Number>(cur_object)->GetValue();
    }
    return Heap::Instance()->Allocate<Boolean>(ans);
}

// "<"
std::string Less::GetStringForm(bool) {
    return "<";
}

Object* Less::Apply(Scope* scope, Object* obj) {
    if (obj == nullptr) {
        return Heap::Instance()->Allocate<Boolean>(true);
    }
    bool ans = true;
    size_t size = GetSize(obj);
    if (size == 1) {
        return Heap::Instance()->Allocate<Boolean>(true);
    }
    auto cur_object = GetElem(obj, 0)->Eval(scope);
    if (!Is<Number>(cur_object)) {
        throw RuntimeError(cur_object->GetStringForm() + " is not a number");
    }
    int cur_value = As<Number>(cur_object)->GetValue();
    for (size_t i = 1; i != size; ++i) {
        cur_object = GetElem(obj, i)->Eval(scope);
        if (!Is<Number>(cur_object)) {
            throw RuntimeError(cur_object->GetStringForm() + " is not a number");
        }
        if (cur_value >= As<Number>(cur_object)->GetValue()) {
            ans = false;
            break;
        }
        cur_value = As<Number>(cur_object)->GetValue();
    }
    return Heap::Instance()->Allocate<Boolean>(ans);
}

// "<="
std::string LessOrEq::GetStringForm(bool) {
    return "<=";
}

Object* LessOrEq::Apply(Scope* scope, Object* obj) {
    if (obj == nullptr) {
        return Heap::Instance()->Allocate<Boolean>(true);
    }
    bool ans = true;
    size_t size = GetSize(obj);
    if (size == 1) {
        return Heap::Instance()->Allocate<Boolean>(true);
    }
    auto cur_object = GetElem(obj, 0)->Eval(scope);
    if (!Is<Number>(cur_object)) {
        throw RuntimeError(cur_object->GetStringForm() + " is not a number");
    }
    int cur_value = As<Number>(cur_object)->GetValue();
    for (size_t i = 0; i != size; ++i) {
        cur_object = GetElem(obj, i)->Eval(scope);
        if (!Is<Number>(cur_object)) {
            throw RuntimeError(cur_object->GetStringForm() + " is not a number");
        }
        if (cur_value > As<Number>(cur_object)->GetValue()) {
            ans = false;
            break;
        }
        cur_value = As<Number>(cur_object)->GetValue();
    }
    return Heap::Instance()->Allocate<Boolean>(ans);
}

// ">="
std::string GreaterOrEq::GetStringForm(bool) {
    return ">=";
}

Object* GreaterOrEq::Apply(Scope* scope, Object* obj) {
    if (obj == nullptr) {
        return Heap::Instance()->Allocate<Boolean>(true);
    }
    bool ans = true;
    size_t size = GetSize(obj);
    if (size == 1) {
        return Heap::Instance()->Allocate<Boolean>(true);
    }
    auto cur_object = GetElem(obj, 0)->Eval(scope);
    if (!Is<Number>(cur_object)) {
        throw RuntimeError(cur_object->GetStringForm() + " is not a number");
    }
    int cur_value = As<Number>(cur_object)->GetValue();
    for (size_t i = 0; i != size; ++i) {
        cur_object = GetElem(obj, i)->Eval(scope);
        if (!Is<Number>(cur_object)) {
            throw RuntimeError(cur_object->GetStringForm() + " is not a number");
        }
        if (cur_value < As<Number>(cur_object)->GetValue()) {
            ans = false;
            break;
        }
        cur_value = As<Number>(cur_object)->GetValue();
    }
    return Heap::Instance()->Allocate<Boolean>(ans);
}

// "min"
std::string Min::GetStringForm(bool) {
    return "min";
}

Object* Min::Apply(Scope* scope, Object* obj) {
    if (obj == nullptr) {
        throw RuntimeError("Not enough args");
    }
    int res = INT32_MAX;
    size_t size = GetSize(obj);
    for (size_t i = 0; i != size; ++i) {
        auto cur_object = GetElem(obj, i)->Eval(scope);
        if (!Is<Number>(cur_object)) {
            throw RuntimeError(cur_object->GetStringForm() + " is not a number");
        }
        res = std::min(res, As<Number>(cur_object)->GetValue());
    }
    return Heap::Instance()->Allocate<Number>(res);
}

// "max"
std::string Max::GetStringForm(bool) {
    return "max";
}

Object* Max::Apply(Scope* scope, Object* obj) {
    if (obj == nullptr) {
        throw RuntimeError("Not enough args");
    }
    int res = INT32_MIN;
    size_t size = GetSize(obj);
    for (size_t i = 0; i != size; ++i) {
        auto cur_object = GetElem(obj, i)->Eval(scope);
        if (!Is<Number>(cur_object)) {
            throw RuntimeError(cur_object->GetStringForm() + " is not a number");
        }
        res = std::max(res, As<Number>(cur_object)->GetValue());
    }
    return Heap::Instance()->Allocate<Number>(res);
}

// Predicates
// "symbol?"
std::string SymbolPredicate::GetStringForm(bool inner_call) {
    return "symbol?";
}

Object* SymbolPredicate::Apply(Scope* scope, Object* obj) {
    if (!IsList(obj)) {
        throw RuntimeError("");
    }
    if (GetSize(obj) != 1) {
        throw RuntimeError("To many(few) args");
    }
    auto symbol_to_check = GetElem(obj, 0);
    return Heap::Instance()->Allocate<Boolean>(Is<Symbol>(symbol_to_check->Eval(scope)));
}

// "list?"
std::string ListPredicate::GetStringForm(bool) {
    return "list?";
}

Object* ListPredicate::Apply(Scope* scope, Object* obj) {
    if (!IsList(obj)) {
        throw RuntimeError("");
    }
    if (GetSize(obj) != 1) {
        throw RuntimeError("To many(few) args");
    }
    auto list_to_check = GetElem(obj, 0);
    if (list_to_check == nullptr) {
        return Heap::Instance()->Allocate<Boolean>(true);
    }
    return Heap::Instance()->Allocate<Boolean>(IsList(list_to_check->Eval(scope)));
}

// "pair?"
std::string PairPredicate::GetStringForm(bool) {
    return "pair?";
}

Object* PairPredicate::Apply(Scope* scope, Object* obj) {
    if (!IsList(obj)) {
        throw RuntimeError("");
    }
    if (GetSize(obj) != 1) {
        throw RuntimeError("To many(few) args");
    }
    auto pair_to_check = GetElem(obj, 0);
    if (pair_to_check == nullptr) {
        return Heap::Instance()->Allocate<Boolean>(false);
    }
    return Heap::Instance()->Allocate<Boolean>(IsPair(pair_to_check->Eval(scope)));
}

// "boolean?"
std::string BooleanPredicate::GetStringForm(bool) {
    return "boolean?";
}

Object* BooleanPredicate::Apply(Scope* scope, Object* obj) {
    if (!IsList(obj)) {
        throw RuntimeError("");
    }
    if (GetSize(obj) != 1) {
        throw RuntimeError("To many(few) args");
    }
    auto bool_to_check = GetElem(obj, 0);
    if (bool_to_check == nullptr) {
        return Heap::Instance()->Allocate<Boolean>(false);
    }
    return Heap::Instance()->Allocate<Boolean>(Is<Boolean>(bool_to_check->Eval(scope)));
}

// "number?"
std::string NumberPredicate::GetStringForm(bool) {
    return "number?";
}

Object* NumberPredicate::Apply(Scope* scope, Object* obj) {
    if (!IsList(obj)) {
        throw RuntimeError("");
    }
    if (GetSize(obj) != 1) {
        throw RuntimeError("To many(few) args");
    }
    auto num_to_check = GetElem(obj, 0);
    if (num_to_check == nullptr) {
        return Heap::Instance()->Allocate<Boolean>(false);
    }
    return Heap::Instance()->Allocate<Boolean>(Is<Number>(num_to_check->Eval(scope)));
}

// "null?"
std::string NullPredicate::GetStringForm(bool) {
    return "null?";
}

Object* NullPredicate::Apply(Scope* scope, Object* obj) {
    if (!IsList(obj)) {
        throw RuntimeError("");
    }
    if (GetSize(obj) != 1) {
        throw RuntimeError("To many(few) args");
    }
    auto null_to_check = GetElem(obj, 0);
    if (null_to_check == nullptr) {
        return Heap::Instance()->Allocate<Boolean>(true);
    }
    return Heap::Instance()->Allocate<Boolean>(null_to_check->Eval(scope) == nullptr);
}

// Logic
// "not"
std::string Not::GetStringForm(bool) {
    return "not";
}

Object* Not::Apply(Scope* scope, Object* obj) {
    if (!IsList(obj)) {
        throw RuntimeError("");
    }
    if (GetSize(obj) != 1) {
        throw RuntimeError("To many(few) args");
    }
    auto elem_to_inverse = GetElem(obj, 0)->Eval(scope);
    if (Is<Boolean>(elem_to_inverse)) {
        return Heap::Instance()->Allocate<Boolean>(elem_to_inverse->GetStringForm() != "#t");
    } else {
        return Heap::Instance()->Allocate<Boolean>(false);
    }
}

// "and"
std::string And::GetStringForm(bool) {
    return "and";
}

Object* And::Apply(Scope* scope, Object* obj) {
    if (!IsList(obj)) {
        throw RuntimeError("");
    }
    bool flag = true;
    if (obj == nullptr) {
        return Heap::Instance()->Allocate<Boolean>(true);
    }
    size_t size = GetSize(obj);
    for (size_t i = 0; i != size; ++i) {
        auto cur_obj = GetElem(obj, i)->Eval(scope);
        if (cur_obj->GetStringForm() == "#f") {
            flag = false;
            break;
        }
    }
    if (!flag) {
        return Heap::Instance()->Allocate<Boolean>(false);
    }
    auto last_elem = GetElem(obj, size - 1)->Eval(scope);
    return last_elem;
}

// "or"
std::string Or::GetStringForm(bool) {
    return "or";
}

Object* Or::Apply(Scope* scope, Object* obj) {
    if (!IsList(obj)) {
        throw RuntimeError("");
    }
    if (obj == nullptr) {
        return Heap::Instance()->Allocate<Boolean>(false);
    }
    bool flag = false;
    size_t size = GetSize(obj);
    size_t last_ind;
    for (size_t i = 0; i != size; ++i) {
        last_ind = i;
        auto cur_obj = GetElem(obj, i)->Eval(scope);
        if (cur_obj->GetStringForm() != "#f") {
            flag = true;
            break;
        }
    }
    if (!flag) {
        return Heap::Instance()->Allocate<Boolean>(false);
    }
    auto last_eval_elem = GetElem(obj, last_ind)->Eval(scope);
    return last_eval_elem;
}

// List & Pair operations
// "list"
std::string MakeList::GetStringForm(bool) {
    return "list";
}

Object* MakeList::Apply(Scope* scope, Object* obj) {
    if (GetSize(obj) == 0) {
        return nullptr;
    }
    Object* res = nullptr;
    size_t size = GetSize(obj);
    for (size_t i = 0; i != size; ++i) {
        res = Append(res, GetElem(obj, i)->Eval(scope));
    }
    return res;
}

// "cons"
std::string MakePair::GetStringForm(bool) {
    return "cons";
}

Object* MakePair::Apply(Scope* scope, Object* obj) {
    if (GetSize(obj) != 2) {
        throw RuntimeError("To many(few) args");
    }
    Object* res = Heap::Instance()->Allocate<Cell>(nullptr, nullptr);
    As<Cell>(res)->SetFirst(GetElem(obj, 0)->Eval(scope));
    As<Cell>(res)->SetSecond(GetElem(obj, 1)->Eval(scope));
    return res;
}

// "list-ref"
std::string ListRef::GetStringForm(bool) {
    return "list-ref";
}

Object* ListRef::Apply(Scope* scope, Object* obj) {
    if (GetSize(obj) != 2) {
        throw RuntimeError("To many(few) args");
    }
    if (GetElem(obj, 0) == nullptr) {
        throw RuntimeError("");
    }
    auto list = GetElem(obj, 0)->Eval(scope);
    if (list == nullptr) {
        throw RuntimeError("Empty list");
    }
    if (!IsList(list)) {
        throw RuntimeError(list->GetStringForm() + " is not a list");
    }
    auto ind_holder = GetElem(obj, 1)->Eval(scope);
    if (!Is<Number>(ind_holder)) {
        throw RuntimeError(ind_holder->GetStringForm() + " is not a number");
    }
    int ind = As<Number>(ind_holder)->GetValue();
    return GetElem(list, ind);
}

// "list-tail"
std::string ListTail::GetStringForm(bool) {
    return "list-tail";
}

Object* ListTail::Apply(Scope* scope, Object* obj) {
    if (GetSize(obj) != 2) {
        throw RuntimeError("To many(few) args");
    }
    auto ind_holder = GetElem(obj, 1)->Eval(scope);
    if (!Is<Number>(ind_holder)) {
        throw RuntimeError(ind_holder->GetStringForm() + " is not a number");
    }
    int ind = As<Number>(ind_holder)->GetValue();
    if (GetElem(obj, 0) == nullptr || GetElem(obj, 0)->Eval(scope) == nullptr) {
        if (ind == 0) {
            return nullptr;
        } else {
            throw RuntimeError("List index out of range");
        }
    }
    auto list = GetElem(obj, 0)->Eval(scope);
    if (!IsList(list)) {
        throw RuntimeError(list->GetStringForm() + " is not a list");
    }
    if (ind > GetSize(list)) {
        throw RuntimeError("");
    }
    int cnt = 0;
    while (cnt != ind) {
        list = As<Cell>(list)->GetSecond();
        ++cnt;
    }
    return list;
}

// "car"
std::string Car::GetStringForm(bool) {
    return "car";
}

Object* Car::Apply(Scope* scope, Object* obj) {
    if (GetSize(obj) != 1) {
        throw RuntimeError("To many(few) args");
    }
    auto pair = GetElem(obj, 0)->Eval(scope);
    if (pair == nullptr) {
        throw RuntimeError("Empty pair");
    }
    if (!Is<Cell>(pair)) {
        throw RuntimeError(pair->GetStringForm() + " is not a pair");
    }
    return As<Cell>(pair)->GetFirst();
}

// "cdr"
std::string Cdr::GetStringForm(bool) {
    return "cdr";
}

Object* Cdr::Apply(Scope* scope, Object* obj) {
    if (GetSize(obj) != 1) {
        throw RuntimeError("To many(few) args");
    }
    auto pair = GetElem(obj, 0)->Eval(scope);
    if (pair == nullptr) {
        throw RuntimeError("Empty pair");
    }
    if (!Is<Cell>(pair)) {
        throw RuntimeError(pair->GetStringForm() + " is not a pair");
    }
    return As<Cell>(pair)->GetSecond();
}

// Quote
// "quote"
std::string Quote::GetStringForm(bool) {
    return "quote";
}

Object* Quote::Apply(Scope* scope, Object* obj) {
    if (!IsList(obj)) {
        throw RuntimeError("");
    }
    if (GetSize(obj) != 1) {
        throw RuntimeError("To many(few) args");
    }
    return GetElem(obj, 0);
}

// "'"
std::string QuoteShort::GetStringForm(bool) {
    return "'";
}

Object* QuoteShort::Apply(Scope* scope, Object* obj) {
    return obj;
}

// Define & Set
// "define"
std::string Define::GetStringForm(bool inner_call) {
    return "define";
}

Object* Define::Apply(Scope* scope, Object* obj) {
    if (!IsList(obj)) {
        throw SyntaxError("");
    }
    if (GetSize(obj) < 2) {
        throw SyntaxError("To many(few) args");
    }
    auto symbol = GetElem(obj, 0);
    if (Is<Cell>(symbol)) {
        if (!Is<Symbol>(As<Cell>(symbol)->GetFirst())) {
            throw SyntaxError("Not a symbol");
        }
        auto args_list = (As<Cell>(symbol)->GetSecond());
        symbol = (As<Cell>(symbol)->GetFirst());
        auto actions = As<Cell>(obj)->GetSecond();
        auto new_cell = Heap::Instance()->Allocate<Cell>(args_list, actions);
        auto new_lambda = As<Function>(scope->LookUp("lambda"))->Apply(scope, new_cell);
        As<Lambda>(new_lambda)->GetLocalScope()->Define(symbol->GetStringForm(), new_lambda);
        scope->Define(symbol->GetStringForm(), new_lambda);
        return symbol;
    }
    if (!Is<Symbol>(symbol)) {
        throw SyntaxError("Not a symbol");
    }
    if (GetSize(obj) != 2) {
        throw SyntaxError("To many(few) args");
    }
    auto value = GetElem(obj, 1)->Eval(scope);
    if (Is<Lambda>(value)) {
        As<Lambda>(value)->GetLocalScope()->Define(symbol->GetStringForm(), value);
    }
    scope->Define(symbol->GetStringForm(), value);
    return symbol;
}

// "set!"
std::string Set::GetStringForm(bool inner_call) {
    return "set!";
}

Object* Set::Apply(Scope* scope, Object* obj) {
    if (!IsList(obj)) {
        throw SyntaxError("");
    }
    if (GetSize(obj) != 2) {
        throw SyntaxError("To many(few) args");
    }
    auto symbol = GetElem(obj, 0);
    if (Is<Cell>(symbol)) {
        symbol = symbol->Eval(scope);
    }
    if (!Is<Symbol>(symbol)) {
        throw SyntaxError("Not a symbol");
    }
    if (scope->LookUp(symbol->GetStringForm()) == nullptr) {
        throw NameError("Undefined name");
    }
    auto new_value = GetElem(obj, 1)->Eval(scope);
    while (!scope->LookUpLocal(symbol->GetStringForm())) {
        scope = scope->GetParent();
    }
    scope->Define(symbol->GetStringForm(), new_value);
    return symbol;
}

// "set-car!"
std::string SetCar::GetStringForm(bool inner_call) {
    return "set-car!";
}

Object* SetCar::Apply(Scope* scope, Object* obj) {
    if (!IsList(obj)) {
        throw SyntaxError("");
    }
    if (GetSize(obj) != 2) {
        throw SyntaxError("To many(few) args");
    }
    auto pair = GetElem(obj, 0)->Eval(scope);
    if (!Is<Cell>(pair)) {
        throw RuntimeError("Not a cell");
    }
    auto new_first_value = GetElem(obj, 1)->Eval(scope);
    As<Cell>(pair)->SetFirst(new_first_value);
    return nullptr;
}

// "set-cdr!"
std::string SetCdr::GetStringForm(bool inner_call) {
    return "set-cdr!";
}

Object* SetCdr::Apply(Scope* scope, Object* obj) {
    if (!IsList(obj)) {
        throw SyntaxError("");
    }
    if (GetSize(obj) != 2) {
        throw SyntaxError("To many(few) args");
    }
    auto pair = GetElem(obj, 0)->Eval(scope);
    if (!Is<Cell>(pair)) {
        throw RuntimeError("Not a cell");
    }
    auto new_second_value = GetElem(obj, 1)->Eval(scope);
    As<Cell>(pair)->SetSecond(new_second_value);
    return nullptr;
}

// If
// "if"
std::string IfStatement::GetStringForm(bool inner_call) {
    return "if";
}

Object* IfStatement::Apply(Scope* scope, Object* obj) {
    if (!IsList(obj)) {
        throw SyntaxError("");
    }
    if (GetSize(obj) > 3 || GetSize(obj) < 2) {
        throw SyntaxError("To many(few) args");
    }
    auto statement = GetElem(obj, 0)->Eval(scope);
    if (!Is<Boolean>(statement)) {
        throw SyntaxError("Not a boolean");
    }
    if (As<Boolean>(statement)->GetValue()) {
        return GetElem(obj, 1)->Eval(scope);
    }
    if (GetSize(obj) == 2) {
        return nullptr;
    }
    return GetElem(obj, 2)->Eval(scope);
}

// Lambdas
// "lambda"
std::string MakeLambda::GetStringForm(bool inner_call) {
    return "lambda";
}

Object* MakeLambda::Apply(Scope* scope, Object* obj) {
    if (!IsList(obj)) {
        throw SyntaxError("");
    }
    if (GetSize(obj) < 2) {
        throw SyntaxError("");
    }
    auto args_list = GetElem(obj, 0);
    if (!IsList(args_list)) {
        throw SyntaxError("");
    }
    std::vector<std::string> args_name;
    for (size_t i = 0; i != GetSize(args_list); ++i) {
        auto arg = GetElem(args_list, i);
        if (!Is<Symbol>(arg)) {
            throw SyntaxError("");
        }
        args_name.emplace_back(arg->GetStringForm());
    }
    auto action = As<Cell>(obj)->GetSecond();
    return Heap::Instance()->Allocate<Lambda>(scope, args_name, action);
}

Object* Lambda::Apply(Scope* scope, Object* obj) {
    if (!IsList(obj)) {
        throw SyntaxError("");
    }
    size_t args_size = args_.size();
    if (GetSize(obj) != args_size) {
        throw SyntaxError("");
    }
    for (size_t i = 0; i != args_size; ++i) {
        auto arg = GetElem(obj, i)->Eval(scope);
        scope->Define(args_[i], arg);
    }
    Object* res;
    for (size_t i = 0; i != GetSize(action_); ++i) {
        auto action = GetElem(action_, i);
        res = action->Eval(scope);
    }
    return res;
}
