# 2024-04-01

- [x] Switch to Neovim
- [x] Build the tree for grammar representation
- [x] Print the tree

Questions : 

Destroy ?
Begin End chevrons <>
why not enum Types type in the object declarations ?
better names for the unary operators ?
shouldn't assignment and declaration be binary operators ?
why represent a sequence as a binary ?
order of sequence problem ?

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

