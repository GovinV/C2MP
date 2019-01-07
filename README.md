# C2MP

Floating Point Operation Compiler

## Authors

DIVRIOTIS Constantin 

FLINT Clément

JUNG Lionel   
      
VETRIVEL Govindaraj

## Synopsis

C2MP (from "C to MPC") is a source to source compiler allowing to translate artihmetic
operations and functions writen in C syntax, into functions from the Multi Point 
Precision (MPC) library. The compiler translates the C code present in a pragma macro.
In the following example, the code between the brackets is compiled:
```

#PRAGMA MPC precision(128) rounding(MPC_RNDZZ)
{
    a = sqrt(pow(b, 5));
    if (a > 10)
    {
        c = 1;
    } else {
        c = 0;
    }
}

```
When compiling a C source file with C2MP, the code outside the pragma is just recopied in
the resulting source file.

You can visit the MPC project website here: http://www.multiprecision.org/mpc

## Installation

Run the following commands :

```

make

./C2MP <file> [-O] [-o <output>] [-v] [-q] [-s]

```
**Options**
* `-O` : add optimization to the generate code
* `-o <output>` : file of the output file
* `-q` : display generated quads
* `-s` : display generated Abstract Syntax Tree
* `-v` : display AST, quads and others informations

**Repository**
* Repository doc        - Doxygen Documentation
* Repository src        - Source File
* Repository include    - Header File
* Repository tests      - Test File


To generate a doxygen documentation, run the following:

```

make doc

```


To run premade tests from the test folder, run:

```

make run

```

## Features

**Capabilities**

Our compiler is handling following instructions:
* Arithmetic operators (+, -, /, *)
* Math functions: pow, sqrt, exp, log, log10, cos, sin, sinh, cosh, sinh and sqr
* Assignments to variables with variables, functions or expressions as rvalues
* Conditions: single if, if binded with else, nested ifs are supported too
* Loops: while, do while and for (note: incrementation is not supported, must use "i=i+1")
* Loops and conditions without brackets (one instuction) are also handled
* Logical operators "and" (&&), "or" (||)
* Comparisions operators: "<", ">", "<=", ">=", "==", "!=" (for mpc_t types comparisions)
* Parenthesizing of expressions is supported
* Unknown function are recopied in the resulting code, arguments are translated 
if they are expressions (ex: function(a, c+d, pow(x,y)) => function(a, temp1, temp2) where 
temp1 and temp2 are mpc_t variables that contains the result of there respective operation)
* Printfs in the pragma are supported (note: maximum arguments number of a function is 10)

Also, C2MP is compiling as many pragmas as needed in one source file.

**Optimizations**

Besides, C2MP is doing the following optimization on the code:
* Removing common subexpressions
* Removing useless temporary variables
* Removing loop invariants
* Reusing temporary variables (if not used anymore)

## Examples

Check the tests/ repository for examples.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
