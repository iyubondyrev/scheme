#pragma once

#include <memory>

#include "object.h"
#include "tokenizer.h"

Object* ReadList(Tokenizer* tokenizer);

Object* ReadToken(Tokenizer* tokenizer);

Object* Read(Tokenizer* tokenizer);
