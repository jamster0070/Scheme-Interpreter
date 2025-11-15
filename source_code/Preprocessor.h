#ifndef PREPROCESSOR_HPP
#define PREPROCESSOR_HPP

#include <string>
#include "Tokenizer.h"

void concatenate(std::string &s, const std::string &tok);
void preprocess(Tokenizer &t, std::string& newCommand);

#endif