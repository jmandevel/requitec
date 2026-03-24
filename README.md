<!--
SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>

SPDX-License-Identifier: MIT
-->

# requitec

__NOTICE: This repository contains a compiler front-end that is in the early stages of development. Its buggy, incomplete, and hard to work with. Use it at your own risk!__

Requite is a statically typed systems programming langauge with a homoiconic syntax. Look in the [test_sources](test_sources) folder for example source code files.

```
[import "std"];

[entry
    .std.print(line="Hello, world!");
];
```

## Language Goals

Requite is a programming language that is designed to fill the same niche as C++. However, it has a number of design goals that make it uniquely suited for systems programming. Not all of these goals are fully implemented yet.

1. Focus on Readability
    * Sacrificed writability
    * Low multiplicity
    * Lots of keywords
    * No reserved words
    * Interpolated strings
    * Extension methods
    * Named and positional function arguments
    * Deductive type inferencing
    * Consistent left-to-right associativity
    * Descriptive casts
2. Expected Behaviour
    * Strong type system
    * No optimizations with side effects
    * Consistent move semantics
    * No implicit conversions
    * No operator overloads
    * Absolute symbol paths
    * Enforced explicit resource destruction
    * No exceptions
3. Powerful features
    * Easy reflection
    * Code generation
    * Compile-time code execution
    * Macro-like templates
    * Rust-like enumerations
    * Code based error handling
    * Range based looping
4. Safe Defaults
    * Constant values by default
    * Private object members by default
    * No-discard returns by default
5. Easy Interoperability with C
    * Explicit mangled names
    * Can include and utilize C headers
    * Can convert C into Requite
    * Can convert Requite into C

## How it Works

The requitec compiler is an LLVM front-end application written in C++. All source code for the front-end is contained within this repository. While language specific functionality is handled within this project, all back-end functionality related to generating machine code for specific target machines is handled by LLVM.

The requitec front-end compiles source files in 6 stages.

1. Validation - In the validation stage, the raw source file text is checked for Unicode errors and unsupported characters.
2. Tokenization - In the tokenization stage, raw source file text is seperated into a list of tokens that correlate to different lexemes.
3. Parsing - In the parsing stage, the list of tokens emitted by the tokenizer are parsed into an Abstract-Syntax Tree (AST) structure that represents Requite code in a format that is efficient for the compiler to manipulate.
4. Situation - In the situation stage, the compiler performs a pre-order traversal in order to do error checking and modification of the AST. This stage is important because it greatly simplifies later stages.
5. Generation - In the generation stage, the compiler traverses the outermost scopes of Requite source files and builds symbol tables of global symbols. Symbols are lazily evaluated, and only the kind and name of each symbol is recorded at this stage unless more work is necessary to fully build out all tables. Then, further tabulation is done per function of the input source file. When tabulating source files, Requite Intermediate Representation (RQIR) is built, which is used for symbolic execution. Functions from imported source files are generated only if necessary.
6. IR Building - In the building stage, the symbol tables and RQIR are used to build LLVM Intermediate Representation (IR), a special language that can be input into LLVM in order to generate assembly and object files.

## How to Use

To try requitec for yourself, you need to compile this project. The only dependency required is LLVM. LLVM development builds are not distributed for Windows, so you must either use a release build or compile LLVM yourself. [This repository](https://github.com/jmandevel/requitec-windows-release) is designed to help with setup, but some skill with CMake and package management is required.

The requitec compiler can be controlled from the command line. It is easy to set up a project using Makefiles. You can see an example [here](test_sources/Makefile).

### Options

| Option / Flag         | Description                                                    | Required | Default   |
|---------------------- |---------------------------------------------------------------|----------|-----------|
| `<input file>`        | Path to the input source file (positional argument)           | Yes      |           |
| `--no-comment`        | Do not add comments to output files                           | No       | false     |
| `-o <output file>`    | Path to the output build file                                 | Yes      |           |
| `-I <dir>`            | Import directories (can be specified multiple times)           | No       |           |
| `--emit <mode>`       | Choose the type of target to build (see below)                | No       | object    |

#### Emit Modes

The `--emit` option supports the following modes:

* `tokens`    : Output CSV token data
* `parsed`    : Output intermediate Requite source code after parsing
* `situated`  : Output intermediate Requite source code after situating
* `symbols`   : Output a markup language file listing user symbols
* `ir`        : Output an LLVM IR source file
* `assembly`  : Output an assembly source file
* `object`    : Output an object file (default)

---
