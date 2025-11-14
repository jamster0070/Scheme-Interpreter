# Scheme Interpreter
Author: Lee Joon Ha


## 1. Build Environment

This project was developed and tested in the following environmnet.

* ** Operating System (OS) **
macOS Sequoia 15.6.1

* ** Compiler Version **
Apple clang version 15.0.0 (clang-1500.0.40.1)
Target: arm64-apple-darwin24.6.0


## 2. How to Compile 

To compile the project, run the following command from within the `source_code` directory:

```bash
g++ -std=c++20 -o scheme_interpreter main.cpp Tokenizer.cpp Parser.cpp Memory.cpp HashTable.cpp Preprocessor.cpp ElemStack.cpp Evaluator.cpp


