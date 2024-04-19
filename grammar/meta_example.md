```
@{

    anonymousFunctionRule = @peg.expression {
        t:abstract-declarator "(" - p:parameter-list ")" - "=>" - b:block 
        {
            name = newTempName(); // name = __function782309842379847

            @c.external-declaration("$t $name ( $p ) $b");

            $$ = newIdentifier(name);
        }
    };

    grammar.rules["expression"].prepend(anonymousFunctionRule);

}
```

-------- Scripting language grammar --------

# First entry point (or escape point) from C :
# Opening a block and modifying the grammar

metaBlock = '@{' - metaInstruction* '}' -

metaInstruction = 
                ( metaVarAssignment
                | metaExpression
                ) SC -

metaVarAssignment = identifier EQUAL metaExpression -

metaExpression =
               ( '@' namespaceIdentifier '.' ruleIdentifier LBRACE < .* > RBRACE
               | evaluable ( '.' evaluable )*
               ) -

# Let's assume we don't have operators for now
evaluable = identifier
          | identifier '(' argumentList? ')'
          | identifier '[' stringLiteral ']'

argumentList =
             ( evaluable
             | evaluable ( ',' evaluable )+
             ) -

# Second entry point, from the C code in actions :

metaFunctionCall = "@" metaExpression '(' argumentList ')'

--------------------------------------------

Execution :

- Parse C code normally
- Meta block :
    - Interpret scripting language inside the block

- What to do with actions ?
    - Are we turning every action into scripting language instead of C code ?
        This means changing how actions are executed at parse time, likely requiring to store them as their own tree
        that needs to be executable instead of the current "writing functions to generated C file" approach



            // translationUnit.append(functionDefinition(t, name, p, b));

            `c.external-declaration @,t @,name (@,p) @,b
