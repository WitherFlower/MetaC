## Dynamically extensible parser machine

1. build a parser + generator that converts grammar G into a data structure that describes the language: G -> dsG

2. build an interpreter for it => a parser: interp(dsG) -> generator(parser(G'), ds) -> dsG' (at runtime)
   > corollary: if G == G' then you can regenerate the static dsG dynamically by running it on its own input G

3. build an API for modifying the grammar data structure

4. demonstrate a small meta language modifying the original compiled syntax

* <u>Appendix</u> : build a C grammar and demonstrate it being extended dynamically by a meta language

## C meta processor: source->source transformation directed by meta program

1. C parser

2. objects to represent the parse tree (ideally all objects for tree, grammar, etc., are within the same object system)

3. language to manipulate the parser and parse tree

4. bridge between meta language and statically compiled parts

5. bridge between meta language and objects representing the parse tree

6. regenerate a transformed output program from the modified parse tree

## Documentation

The parser generator (with lots of reading in the manual page):

https://www.piumarta.com/software/peg/

Some reading:

https://bford.info/pub/lang/peg.pdf

Some more reading, if you are brave:

https://www.piumarta.com/freeco11/freeco11-piumarta-oecm.pdf