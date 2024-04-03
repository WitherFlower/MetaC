# 2024-04-03

- [x] Fix prefix unaries

- [ ] Figure out how to evaluate the tree
    - What should we get as a result of execution ?
        grammar g ---> eval G-Tree ---> ???

Questions : 

Destroy ?
Begin End chevrons <>
why not enum Types type in the object declarations ?
better names for the unary operators ?
shouldn't assignment and declaration be binary operators ?
why represent a sequence as a binary ?
order of sequence problem ?
grammar_objects.c:391 : freeing thing question

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

