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

## What am I doing

You have a C (.leg) program that can parse a grammar "G" into a tree representation "TG".  Let's call this C program "PC".  The problem is that the parser in PC is a C program and so it cannot be extended at run time.  So...
 
Run PC on its own grammar "GG" (a grammar for grammars) to make a tree TGG.  TGG is a data structure describing GG which is a grammar describing how to parse grammars and produce a corresponding tree.
 
Now write out TGG in the form of C data declarations (structs, unions, arrays, pointer variables, functions for the actions, etc.) in such a way that the C compiler will turn them back into the identical tree.  Let's call this C code CTGG.  You can include the CTGG  declarations (along with a header file describing the types it uses) in any C program to obtain the tree TGG without having to actually parse GG again.  It is a form of GG "pickled" as the data structure TGG that describes it, in this case using the C language hence CTGG.
 
Now write an interpreter for CTGG, the tree you just picked.  When you run the tree TGG as a program it should parse some input text describing a grammar G' and create a tree TG' for it.  If you run TGG as a program on the grammar GG then you should recreate an identical copy of TGG.  You should even be able to write that out again as identical CTGG data declarations.
 
What you can potentially do now is to include CTGG and its interpreter in any C program to enable that program to parse grammars written in GG.  With a little extra syntax in GG you can modify the tree CTGG at run time to change its own syntax rules.  In particular, if you have a grammar H written in GG for a language L then you can run PC or TGG on it to create a tree TH and corresponding C declarations CTH which you can include in any C program to enable it to parse programs written in L.  If you union H with GG (including the additional rules to extend trees produced from grammars written in G) then TH understands how to modify itself: you make your parser TH, and therefore the language L, extensible at run time by the L program you are currently parsing.  Our ultimate mission is to have L be the C language, to make a parser TC.  TC is a C parser that can modify the syntax of C that it understands at parse time: the C source that TC parses can include meta code written in GG that modifies the "live" tree TC that is being interpreted as the active, running parser.
