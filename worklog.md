# 2024-04-23

- [x] Debugged the tree printing, GC was causing memory corruption
- [ ] Parse actions in their most basic form
- [ ] Writing Rewrite
- [ ] Evaluator

# 2024-04-22

- [x] Replace underlying object system with 先生's prototype system example
- [x] Convert tree printing code to use prototypes

# 2024-04-19

- [x] Understand Piumarta先生's prototype example

# 2024-04-18

- [x] Write a draft of the interpreted language's grammar
- Plan for the next few days :
    - Prototype system
    - Grammar for the interpreted language
    - Store actions as subtrees
    - Make those executable (look at the example for that)

# 2024-04-17

- [x] Convince Piumarta先生 to use Kotlin's string templating
- [x] Generic pointer experimentation
- [x] Use getchar() for faster file reading

# 2024-04-16

- [x] Meeting with Piumarta先生
    Takeaways :
        - Maybe Makefile isn't that bad
        - Change file reading to use getchar() (faster and literally simpler)
        - Convert object system to using prototypes
        - Draft of the meta scripting language

# 2024-04-15

- [x] Repair build stuff
- [x] Test writing tree
- [x] Refactor tree evaluation to use object system

# 2024-04-12

- [x] Refactor printing tree to use object system
- [x] Refactor writing tree to use object system

# 2024-04-11

- [x] WSL setup
- [x] Dictionary type
- [x] Object system

# 2024-04-10

- [x] ~~Makefile~~ Mason for everything
        - [x] Merci Nam

- [x] Redo tests (kinda)

# 2024-04-09

- [x] Debugging
- [x] Regex madness

# 2024-04-08

- [x] Tree Evaluation
        - [x] Store actions with # define inside of CTGG
                requires thinking about he structure of the context
        - [x] Test by feeding the grammar into itself
                - Needs debugging

# 2024-04-05

- [x] Extended List type to support binary search and insertion by pointer value
        With tests !!!
- [x] Symbol list
- [x] Stop using identifiers for definition and assignment
- [x] Rewrite file generation to use static data structures

# 2024-04-04

- [x] Meeting with Piumarta sensei, established direction
- List of symbols
    - intern function that transforms string into symbol (pointer)
    - Sorted array of symbols to store them and search when theres a new one
        - look into memmove()
    - make unit tests
- Parallel list storing the trees for every rule (I should already have this one)
- Make the generated file using static data declarations
- Action stuff with # define

# 2024-04-03

- [x] Fix prefix unaries

- [x] Figure out how to evaluate the tree
      Thought of a solution, need to confirm direction with Piumarta sensei tomorrow.

Questions : 

Destroy ?
Begin End chevrons <>
why not enum Types type in the object declarations ?
better names for the unary operators ?
shouldn't assignment and declaration be binary operators ?
why represent a sequence as a binary ?
order of sequence problem ?
is it correct that all variables are scoped to their definition ?
    aka you can't a new scope for identifiers inside a definition

# 2024-04-02

- [x] Write out TGG in the form of C data declarations
    (structs, unions, arrays, pointer variables, functions for the actions, etc.)
    In such a way that the C compiler will turn them back into the identical tree.

# 2024-04-01

- [x] Switch to Neovim
- [x] Build the tree for grammar representation
- [x] Print the tree

# 2024-03-29

- [x] Create Binary/Unary/Leaf types with inner enums
        Only Binary was needed, not sure about Leaf for now
- [x] Split the constructors into their own functions to have type checking / compile-time decisions
- [x] Integer type so that the evaluator only manipulates objects
- [x] Describe the data structure for a grammar
        Unary * + ? ! &
        Leaf . string characterClass Action ruleIdentifier
        Binary Sequence, Alternation
        Assignment
        ruleDeclaration
- [x] Write the constructors for the data structures

# 2024-03-28

- [x] Tree builder for expressions
        Construction de l'arbre dans la grammaire
- [x] Print tree with tabs
- [x] Bottom-up Evaluator

# 2024-03-27

- [x] Turn declaration into an expression

- [~] Tree builder for expressions
        node type for literal
        Binary operator thing with enums
        node type for var names

# 2024-03-26

Travail à faire pour se familiariser avec peg :
- [x] Multiplication
- [x] Division
- [x] variables en une lettre avec affectation, simplement utiliser un tableau de 26 éléments.

- [x] Parenthèses

# 2024-03-25

Réunion avec M. Piumarta, explication du sujet

