# Interpreter

## Things that the interpreter can handle

- All mathematical operators (+, -, \*, /, %, ==, !=)
- return
- Variable Assignment
- Variable Declaraton

## Things it doesn't handle

- Conditionals
- Function Declarations
- Function Application

# TAC Generator

## Things that the TAC generator can handle

- All mathematical operators
- return
- "Fake" variable assignment

## Things it doesn't handle

- Conditionals
- Function declarations
- Function applications
- Scope/Blocks
- Optimisation

# Straight-line TODO list

- TAC generation
- Basic MIPS generation
- a proper return
    - print\_int(return arg)
    - then exit()
- TAC generator needs to use frames to store and retrieve variables
- TAC\* linked list
    - gen\_tac should return the first TAC\* as a linked list head so main can traverse the TAC
