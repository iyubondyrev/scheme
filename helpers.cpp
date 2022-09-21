#include "object.h"

bool IsNumber(const std::string& str) {
    if (str.empty()) {
        return false;
    }
    if (!std::isdigit(str[0])) {
        if (!(str[0] == '-' || str[0] == '+')) {
            return false;
        }
        if (str.size() == 1) {
            return false;
        }
    }
    for (size_t i = 1; i != str.size(); ++i) {
        if (!std::isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

bool IsList(Object* obj) {
    if (obj == nullptr) {
        return true;
    }
    if (!Is<Cell>(obj)) {
        return false;
    }
    if (As<Cell>(obj)->GetSecond() == nullptr) {
        return true;
    }
    auto right = As<Cell>(obj)->GetSecond();
    return IsList(right);
}

size_t GetSize(Object* obj) {
    if (!IsList(obj)) {
        throw RuntimeError("Not list");
    }
    if (obj == nullptr) {
        return 0;
    }
    size_t res = 1;
    auto cur = obj;
    while (As<Cell>(cur)->GetSecond() != nullptr) {
        ++res;
        cur = As<Cell>(cur)->GetSecond();
    }
    return res;
}

Object* GetElem(Object* obj, size_t ind) {
    if (!IsList(obj)) {
        throw RuntimeError("Not list");
    }
    if (ind >= GetSize(obj)) {
        throw RuntimeError("List index out of range");
    }
    size_t cur_elem = 0;
    auto cur = obj;
    while (cur_elem != ind) {
        cur = As<Cell>(cur)->GetSecond();
        ++cur_elem;
    }
    return As<Cell>(cur)->GetFirst();
}

Object* Append(Object* to, Object* elem) {
    if (to == nullptr) {
        return Heap::Instance()->Allocate<Cell>(elem, nullptr);
    }
    Object* last_cell = to;
    while (As<Cell>(last_cell)->GetSecond() != nullptr) {
        last_cell = As<Cell>(last_cell)->GetSecond();
    }
    As<Cell>(last_cell)->SetSecond(Heap::Instance()->Allocate<Cell>(elem, nullptr));
    return to;
}

bool IsPair(Object* obj) {
    if (obj == nullptr) {
        return false;
    }
    if (!Is<Cell>(obj)) {
        return false;
    }
    if (!Is<Cell>(As<Cell>(obj)->GetSecond())) {
        return true;
    }
    if (As<Cell>(As<Cell>(obj)->GetSecond())->GetSecond() == nullptr) {
        return true;
    }
    return false;
}
