#include "grammar_objects.h"

union Object String1 = { .String.type = String, .String.value = "grammar" };
union Object Identifier2 = { .Identifier.type = Identifier, .Identifier.value = "-" };
union Object String3 = { .String.type = String, .String.value = "r" };
union Object Identifier4 = { .Identifier.type = Identifier, .Identifier.value = "root" };
union Object Assignment5 = { .Assignment.type = Assignment, .Assignment.variableName = &String3, .Assignment.ruleIdentifier = &Identifier4 };
union Object Sequence6 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Identifier2, .Binary.rightExpression = &Assignment5 };
union Object Identifier7 = { .Identifier.type = Identifier, .Identifier.value = "declaration" };
union Object String8 = { .String.type = String, .String.value = "d" };
union Object Identifier9 = { .Identifier.type = Identifier, .Identifier.value = "definition" };
union Object Assignment10 = { .Assignment.type = Assignment, .Assignment.variableName = &String8, .Assignment.ruleIdentifier = &Identifier9 };

#define $$ ctx->returnValue
#define yytext ctx->input
#define r ctx->vars->data[0]
#define d ctx->vars->data[1]

void Action11_function(Context *ctx) {
	 addRuleDefinitionToGrammar(r, d); 
}

#undef r
#undef d
#undef $$
#undef yytext

union Object Action11 = { .Action.type = Action, .Action.value = " addRuleDefinitionToGrammar(r, d); ", .Action.function = Action11_function };
union Object Sequence12 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Assignment10, .Binary.rightExpression = &Action11 };
union Object Alternation13 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &Identifier7, .Binary.rightExpression = &Sequence12 };
union Object Plus14 = { .Unary.type = Unary, .Unary.op = Plus, .Unary.expression = &Alternation13 };
union Object Sequence15 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence6, .Binary.rightExpression = &Plus14 };
union Object Identifier16 = { .Identifier.type = Identifier, .Identifier.value = "trailer" };
union Object Optional17 = { .Unary.type = Unary, .Unary.op = Optional, .Unary.expression = &Identifier16 };
union Object Sequence18 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence15, .Binary.rightExpression = &Optional17 };
union Object Identifier19 = { .Identifier.type = Identifier, .Identifier.value = "end-of-file" };
union Object Sequence20 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence18, .Binary.rightExpression = &Identifier19 };

#define $$ ctx->returnValue
#define yytext ctx->input
#define r ctx->vars->data[0]
#define d ctx->vars->data[1]

void Action21_function(Context *ctx) {
	 printTree(r); /*writeTree(r);*/ 
}

#undef r
#undef d
#undef $$
#undef yytext

union Object Action21 = { .Action.type = Action, .Action.value = " printTree(r); /*writeTree(r);*/ ", .Action.function = Action21_function };
union Object Sequence22 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence20, .Binary.rightExpression = &Action21 };
union Object Definition23 = { .Definition.type = Definition, .Definition.name = &String1, .Definition.rule = &Sequence22 };

union Object String24 = { .String.type = String, .String.value = "root" };

#define $$ ctx->returnValue
#define yytext ctx->input

void Action25_function(Context *ctx) {
	 $$ = newGrammar(); 
}

#undef $$
#undef yytext

union Object Action25 = { .Action.type = Action, .Action.value = " $$ = newGrammar(); ", .Action.function = Action25_function };
union Object Definition26 = { .Definition.type = Definition, .Definition.name = &String24, .Definition.rule = &Action25 };

union Object String27 = { .String.type = String, .String.value = "declaration" };
union Object String28 = { .String.type = String, .String.value = "%{" };
union Object Begin29 = { .Begin.type = Begin };
union Object Sequence30 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String28, .Binary.rightExpression = &Begin29 };
union Object String31 = { .String.type = String, .String.value = "%}" };
union Object Not32 = { .Unary.type = Unary, .Unary.op = Not, .Unary.expression = &String31 };
union Object Dot33 = { .Dot.type = Dot };
union Object Sequence34 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Not32, .Binary.rightExpression = &Dot33 };
union Object Star35 = { .Unary.type = Unary, .Unary.op = Star, .Unary.expression = &Sequence34 };
union Object Sequence36 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence30, .Binary.rightExpression = &Star35 };
union Object End37 = { .End.type = End };
union Object Sequence38 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence36, .Binary.rightExpression = &End37 };
union Object Identifier39 = { .Identifier.type = Identifier, .Identifier.value = "RPERCENT" };
union Object Sequence40 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence38, .Binary.rightExpression = &Identifier39 };
union Object Definition41 = { .Definition.type = Definition, .Definition.name = &String27, .Definition.rule = &Sequence40 };

union Object String42 = { .String.type = String, .String.value = "trailer" };
union Object String43 = { .String.type = String, .String.value = "%%" };
union Object Begin44 = { .Begin.type = Begin };
union Object Sequence45 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String43, .Binary.rightExpression = &Begin44 };
union Object Dot46 = { .Dot.type = Dot };
union Object Star47 = { .Unary.type = Unary, .Unary.op = Star, .Unary.expression = &Dot46 };
union Object Sequence48 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence45, .Binary.rightExpression = &Star47 };
union Object End49 = { .End.type = End };
union Object Sequence50 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence48, .Binary.rightExpression = &End49 };
union Object Definition51 = { .Definition.type = Definition, .Definition.name = &String42, .Definition.rule = &Sequence50 };

union Object String52 = { .String.type = String, .String.value = "definition" };
union Object String53 = { .String.type = String, .String.value = "i" };
union Object Identifier54 = { .Identifier.type = Identifier, .Identifier.value = "identifier" };
union Object Assignment55 = { .Assignment.type = Assignment, .Assignment.variableName = &String53, .Assignment.ruleIdentifier = &Identifier54 };
union Object Identifier56 = { .Identifier.type = Identifier, .Identifier.value = "EQUAL" };
union Object Sequence57 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Assignment55, .Binary.rightExpression = &Identifier56 };
union Object String58 = { .String.type = String, .String.value = "e" };
union Object Identifier59 = { .Identifier.type = Identifier, .Identifier.value = "expression" };
union Object Assignment60 = { .Assignment.type = Assignment, .Assignment.variableName = &String58, .Assignment.ruleIdentifier = &Identifier59 };
union Object Sequence61 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence57, .Binary.rightExpression = &Assignment60 };
union Object Identifier62 = { .Identifier.type = Identifier, .Identifier.value = "SEMICOLON" };
union Object Optional63 = { .Unary.type = Unary, .Unary.op = Optional, .Unary.expression = &Identifier62 };
union Object Sequence64 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence61, .Binary.rightExpression = &Optional63 };

#define $$ ctx->returnValue
#define yytext ctx->input
#define i ctx->vars->data[0]
#define e ctx->vars->data[1]

void Action65_function(Context *ctx) {
	 $$ = newDefinition(i, e); 
}

#undef i
#undef e
#undef $$
#undef yytext

union Object Action65 = { .Action.type = Action, .Action.value = " $$ = newDefinition(i, e); ", .Action.function = Action65_function };
union Object Sequence66 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence64, .Binary.rightExpression = &Action65 };
union Object Definition67 = { .Definition.type = Definition, .Definition.name = &String52, .Definition.rule = &Sequence66 };

union Object String68 = { .String.type = String, .String.value = "expression" };
union Object String69 = { .String.type = String, .String.value = "s1" };
union Object Identifier70 = { .Identifier.type = Identifier, .Identifier.value = "sequence" };
union Object Assignment71 = { .Assignment.type = Assignment, .Assignment.variableName = &String69, .Assignment.ruleIdentifier = &Identifier70 };
union Object Identifier72 = { .Identifier.type = Identifier, .Identifier.value = "BAR" };
union Object String73 = { .String.type = String, .String.value = "s2" };
union Object Identifier74 = { .Identifier.type = Identifier, .Identifier.value = "sequence" };
union Object Assignment75 = { .Assignment.type = Assignment, .Assignment.variableName = &String73, .Assignment.ruleIdentifier = &Identifier74 };
union Object Sequence76 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Identifier72, .Binary.rightExpression = &Assignment75 };

#define $$ ctx->returnValue
#define yytext ctx->input
#define s1 ctx->vars->data[0]
#define s2 ctx->vars->data[1]

void Action77_function(Context *ctx) {
	 s1 = newBinary(Alternation, s1, s2); 
}

#undef s1
#undef s2
#undef $$
#undef yytext

union Object Action77 = { .Action.type = Action, .Action.value = " s1 = newBinary(Alternation, s1, s2); ", .Action.function = Action77_function };
union Object Sequence78 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence76, .Binary.rightExpression = &Action77 };
union Object Star79 = { .Unary.type = Unary, .Unary.op = Star, .Unary.expression = &Sequence78 };
union Object Sequence80 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Assignment71, .Binary.rightExpression = &Star79 };

#define $$ ctx->returnValue
#define yytext ctx->input
#define s1 ctx->vars->data[0]
#define s2 ctx->vars->data[1]

void Action81_function(Context *ctx) {
	 $$ = s1; 
}

#undef s1
#undef s2
#undef $$
#undef yytext

union Object Action81 = { .Action.type = Action, .Action.value = " $$ = s1; ", .Action.function = Action81_function };
union Object Sequence82 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence80, .Binary.rightExpression = &Action81 };
union Object Definition83 = { .Definition.type = Definition, .Definition.name = &String68, .Definition.rule = &Sequence82 };

union Object String84 = { .String.type = String, .String.value = "sequence" };
union Object String85 = { .String.type = String, .String.value = "e" };
union Object Identifier86 = { .Identifier.type = Identifier, .Identifier.value = "error" };
union Object Assignment87 = { .Assignment.type = Assignment, .Assignment.variableName = &String85, .Assignment.ruleIdentifier = &Identifier86 };
union Object String88 = { .String.type = String, .String.value = "f" };
union Object Identifier89 = { .Identifier.type = Identifier, .Identifier.value = "error" };
union Object Assignment90 = { .Assignment.type = Assignment, .Assignment.variableName = &String88, .Assignment.ruleIdentifier = &Identifier89 };

#define $$ ctx->returnValue
#define yytext ctx->input
#define e ctx->vars->data[0]
#define f ctx->vars->data[1]

void Action91_function(Context *ctx) {
	 e = newBinary(Sequence, e, f); 
}

#undef e
#undef f
#undef $$
#undef yytext

union Object Action91 = { .Action.type = Action, .Action.value = " e = newBinary(Sequence, e, f); ", .Action.function = Action91_function };
union Object Sequence92 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Assignment90, .Binary.rightExpression = &Action91 };
union Object Star93 = { .Unary.type = Unary, .Unary.op = Star, .Unary.expression = &Sequence92 };
union Object Sequence94 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Assignment87, .Binary.rightExpression = &Star93 };

#define $$ ctx->returnValue
#define yytext ctx->input
#define e ctx->vars->data[0]
#define f ctx->vars->data[1]

void Action95_function(Context *ctx) {
	 $$ = e; 
}

#undef e
#undef f
#undef $$
#undef yytext

union Object Action95 = { .Action.type = Action, .Action.value = " $$ = e; ", .Action.function = Action95_function };
union Object Sequence96 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence94, .Binary.rightExpression = &Action95 };
union Object Definition97 = { .Definition.type = Definition, .Definition.name = &String84, .Definition.rule = &Sequence96 };

union Object String98 = { .String.type = String, .String.value = "error" };
union Object String99 = { .String.type = String, .String.value = "p" };
union Object Identifier100 = { .Identifier.type = Identifier, .Identifier.value = "prefix" };
union Object Assignment101 = { .Assignment.type = Assignment, .Assignment.variableName = &String99, .Assignment.ruleIdentifier = &Identifier100 };
union Object Identifier102 = { .Identifier.type = Identifier, .Identifier.value = "TILDE" };
union Object Identifier103 = { .Identifier.type = Identifier, .Identifier.value = "action" };
union Object Sequence104 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Identifier102, .Binary.rightExpression = &Identifier103 };
union Object Optional105 = { .Unary.type = Unary, .Unary.op = Optional, .Unary.expression = &Sequence104 };
union Object Sequence106 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Assignment101, .Binary.rightExpression = &Optional105 };

#define $$ ctx->returnValue
#define yytext ctx->input
#define p ctx->vars->data[0]

void Action107_function(Context *ctx) {
	 $$ = p; 
}

#undef p
#undef $$
#undef yytext

union Object Action107 = { .Action.type = Action, .Action.value = " $$ = p; ", .Action.function = Action107_function };
union Object Sequence108 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence106, .Binary.rightExpression = &Action107 };
union Object Definition109 = { .Definition.type = Definition, .Definition.name = &String98, .Definition.rule = &Sequence108 };

union Object String110 = { .String.type = String, .String.value = "prefix" };
union Object Identifier111 = { .Identifier.type = Identifier, .Identifier.value = "AND" };
union Object Identifier112 = { .Identifier.type = Identifier, .Identifier.value = "action" };
union Object Sequence113 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Identifier111, .Binary.rightExpression = &Identifier112 };
union Object Identifier114 = { .Identifier.type = Identifier, .Identifier.value = "AND" };
union Object String115 = { .String.type = String, .String.value = "s" };
union Object Identifier116 = { .Identifier.type = Identifier, .Identifier.value = "suffix" };
union Object Assignment117 = { .Assignment.type = Assignment, .Assignment.variableName = &String115, .Assignment.ruleIdentifier = &Identifier116 };
union Object Sequence118 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Identifier114, .Binary.rightExpression = &Assignment117 };

#define $$ ctx->returnValue
#define yytext ctx->input
#define s ctx->vars->data[0]

void Action119_function(Context *ctx) {
	 $$ = newUnary(And, s); 
}

#undef s
#undef $$
#undef yytext

union Object Action119 = { .Action.type = Action, .Action.value = " $$ = newUnary(And, s); ", .Action.function = Action119_function };
union Object Sequence120 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence118, .Binary.rightExpression = &Action119 };
union Object Identifier121 = { .Identifier.type = Identifier, .Identifier.value = "NOT" };
union Object String122 = { .String.type = String, .String.value = "s" };
union Object Identifier123 = { .Identifier.type = Identifier, .Identifier.value = "suffix" };
union Object Assignment124 = { .Assignment.type = Assignment, .Assignment.variableName = &String122, .Assignment.ruleIdentifier = &Identifier123 };
union Object Sequence125 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Identifier121, .Binary.rightExpression = &Assignment124 };

#define $$ ctx->returnValue
#define yytext ctx->input
#define s ctx->vars->data[0]

void Action126_function(Context *ctx) {
	 $$ = newUnary(Not, s); 
}

#undef s
#undef $$
#undef yytext

union Object Action126 = { .Action.type = Action, .Action.value = " $$ = newUnary(Not, s); ", .Action.function = Action126_function };
union Object Sequence127 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence125, .Binary.rightExpression = &Action126 };
union Object Alternation128 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &Sequence120, .Binary.rightExpression = &Sequence127 };
union Object String129 = { .String.type = String, .String.value = "s" };
union Object Identifier130 = { .Identifier.type = Identifier, .Identifier.value = "suffix" };
union Object Assignment131 = { .Assignment.type = Assignment, .Assignment.variableName = &String129, .Assignment.ruleIdentifier = &Identifier130 };

#define $$ ctx->returnValue
#define yytext ctx->input
#define s ctx->vars->data[0]

void Action132_function(Context *ctx) {
	 $$ = s; 
}

#undef s
#undef $$
#undef yytext

union Object Action132 = { .Action.type = Action, .Action.value = " $$ = s; ", .Action.function = Action132_function };
union Object Sequence133 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Assignment131, .Binary.rightExpression = &Action132 };
union Object Alternation134 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &Alternation128, .Binary.rightExpression = &Sequence133 };
union Object Alternation135 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &Sequence113, .Binary.rightExpression = &Alternation134 };
union Object Definition136 = { .Definition.type = Definition, .Definition.name = &String110, .Definition.rule = &Alternation135 };

union Object String137 = { .String.type = String, .String.value = "suffix" };
union Object String138 = { .String.type = String, .String.value = "p" };
union Object Identifier139 = { .Identifier.type = Identifier, .Identifier.value = "primary" };
union Object Assignment140 = { .Assignment.type = Assignment, .Assignment.variableName = &String138, .Assignment.ruleIdentifier = &Identifier139 };
union Object Identifier141 = { .Identifier.type = Identifier, .Identifier.value = "QUERY" };

#define $$ ctx->returnValue
#define yytext ctx->input
#define p ctx->vars->data[0]

void Action142_function(Context *ctx) {
	 p = newUnary(Optional, p); 
}

#undef p
#undef $$
#undef yytext

union Object Action142 = { .Action.type = Action, .Action.value = " p = newUnary(Optional, p); ", .Action.function = Action142_function };
union Object Sequence143 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Identifier141, .Binary.rightExpression = &Action142 };
union Object Identifier144 = { .Identifier.type = Identifier, .Identifier.value = "STAR" };

#define $$ ctx->returnValue
#define yytext ctx->input
#define p ctx->vars->data[0]

void Action145_function(Context *ctx) {
	 p = newUnary(Star, p); 
}

#undef p
#undef $$
#undef yytext

union Object Action145 = { .Action.type = Action, .Action.value = " p = newUnary(Star, p); ", .Action.function = Action145_function };
union Object Sequence146 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Identifier144, .Binary.rightExpression = &Action145 };
union Object Alternation147 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &Sequence143, .Binary.rightExpression = &Sequence146 };
union Object Identifier148 = { .Identifier.type = Identifier, .Identifier.value = "PLUS" };

#define $$ ctx->returnValue
#define yytext ctx->input
#define p ctx->vars->data[0]

void Action149_function(Context *ctx) {
	 p = newUnary(Plus, p); 
}

#undef p
#undef $$
#undef yytext

union Object Action149 = { .Action.type = Action, .Action.value = " p = newUnary(Plus, p); ", .Action.function = Action149_function };
union Object Sequence150 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Identifier148, .Binary.rightExpression = &Action149 };
union Object Alternation151 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &Alternation147, .Binary.rightExpression = &Sequence150 };
union Object Optional152 = { .Unary.type = Unary, .Unary.op = Optional, .Unary.expression = &Alternation151 };
union Object Sequence153 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Assignment140, .Binary.rightExpression = &Optional152 };

#define $$ ctx->returnValue
#define yytext ctx->input
#define p ctx->vars->data[0]

void Action154_function(Context *ctx) {
	 $$ = p; 
}

#undef p
#undef $$
#undef yytext

union Object Action154 = { .Action.type = Action, .Action.value = " $$ = p; ", .Action.function = Action154_function };
union Object Sequence155 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence153, .Binary.rightExpression = &Action154 };
union Object Definition156 = { .Definition.type = Definition, .Definition.name = &String137, .Definition.rule = &Sequence155 };

union Object String157 = { .String.type = String, .String.value = "primary" };
union Object String158 = { .String.type = String, .String.value = "i1" };
union Object Identifier159 = { .Identifier.type = Identifier, .Identifier.value = "identifier" };
union Object Assignment160 = { .Assignment.type = Assignment, .Assignment.variableName = &String158, .Assignment.ruleIdentifier = &Identifier159 };
union Object Identifier161 = { .Identifier.type = Identifier, .Identifier.value = "COLON" };
union Object Sequence162 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Assignment160, .Binary.rightExpression = &Identifier161 };
union Object String163 = { .String.type = String, .String.value = "i2" };
union Object Identifier164 = { .Identifier.type = Identifier, .Identifier.value = "ruleCallIdent" };
union Object Assignment165 = { .Assignment.type = Assignment, .Assignment.variableName = &String163, .Assignment.ruleIdentifier = &Identifier164 };
union Object Sequence166 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence162, .Binary.rightExpression = &Assignment165 };
union Object Identifier167 = { .Identifier.type = Identifier, .Identifier.value = "EQUAL" };
union Object Not168 = { .Unary.type = Unary, .Unary.op = Not, .Unary.expression = &Identifier167 };
union Object Sequence169 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence166, .Binary.rightExpression = &Not168 };

#define $$ ctx->returnValue
#define yytext ctx->input
#define i1 ctx->vars->data[0]
#define i2 ctx->vars->data[1]

void Action170_function(Context *ctx) {
	 $$ = newAssignment(i1, i2); 
}

#undef i1
#undef i2
#undef $$
#undef yytext

union Object Action170 = { .Action.type = Action, .Action.value = " $$ = newAssignment(i1, i2); ", .Action.function = Action170_function };
union Object Sequence171 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence169, .Binary.rightExpression = &Action170 };
union Object String172 = { .String.type = String, .String.value = "i" };
union Object Identifier173 = { .Identifier.type = Identifier, .Identifier.value = "ruleCallIdent" };
union Object Assignment174 = { .Assignment.type = Assignment, .Assignment.variableName = &String172, .Assignment.ruleIdentifier = &Identifier173 };
union Object Identifier175 = { .Identifier.type = Identifier, .Identifier.value = "EQUAL" };
union Object Not176 = { .Unary.type = Unary, .Unary.op = Not, .Unary.expression = &Identifier175 };
union Object Sequence177 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Assignment174, .Binary.rightExpression = &Not176 };

#define $$ ctx->returnValue
#define yytext ctx->input
#define i1 ctx->vars->data[0]
#define i2 ctx->vars->data[1]
#define i ctx->vars->data[2]

void Action178_function(Context *ctx) {
	 $$ = i; 
}

#undef i1
#undef i2
#undef i
#undef $$
#undef yytext

union Object Action178 = { .Action.type = Action, .Action.value = " $$ = i; ", .Action.function = Action178_function };
union Object Sequence179 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence177, .Binary.rightExpression = &Action178 };
union Object Alternation180 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &Sequence171, .Binary.rightExpression = &Sequence179 };
union Object Identifier181 = { .Identifier.type = Identifier, .Identifier.value = "OPEN" };
union Object String182 = { .String.type = String, .String.value = "e" };
union Object Identifier183 = { .Identifier.type = Identifier, .Identifier.value = "expression" };
union Object Assignment184 = { .Assignment.type = Assignment, .Assignment.variableName = &String182, .Assignment.ruleIdentifier = &Identifier183 };
union Object Sequence185 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Identifier181, .Binary.rightExpression = &Assignment184 };
union Object Identifier186 = { .Identifier.type = Identifier, .Identifier.value = "CLOSE" };
union Object Sequence187 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence185, .Binary.rightExpression = &Identifier186 };

#define $$ ctx->returnValue
#define yytext ctx->input
#define i1 ctx->vars->data[0]
#define i2 ctx->vars->data[1]
#define i ctx->vars->data[2]
#define e ctx->vars->data[3]

void Action188_function(Context *ctx) {
	 $$ = e; 
}

#undef i1
#undef i2
#undef i
#undef e
#undef $$
#undef yytext

union Object Action188 = { .Action.type = Action, .Action.value = " $$ = e; ", .Action.function = Action188_function };
union Object Sequence189 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence187, .Binary.rightExpression = &Action188 };
union Object Alternation190 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &Alternation180, .Binary.rightExpression = &Sequence189 };
union Object String191 = { .String.type = String, .String.value = "l" };
union Object Identifier192 = { .Identifier.type = Identifier, .Identifier.value = "literal" };
union Object Assignment193 = { .Assignment.type = Assignment, .Assignment.variableName = &String191, .Assignment.ruleIdentifier = &Identifier192 };

#define $$ ctx->returnValue
#define yytext ctx->input
#define i1 ctx->vars->data[0]
#define i2 ctx->vars->data[1]
#define i ctx->vars->data[2]
#define e ctx->vars->data[3]
#define l ctx->vars->data[4]

void Action194_function(Context *ctx) {
	 $$ = l; 
}

#undef i1
#undef i2
#undef i
#undef e
#undef l
#undef $$
#undef yytext

union Object Action194 = { .Action.type = Action, .Action.value = " $$ = l; ", .Action.function = Action194_function };
union Object Sequence195 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Assignment193, .Binary.rightExpression = &Action194 };
union Object Alternation196 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &Alternation190, .Binary.rightExpression = &Sequence195 };
union Object String197 = { .String.type = String, .String.value = "c" };
union Object Identifier198 = { .Identifier.type = Identifier, .Identifier.value = "class" };
union Object Assignment199 = { .Assignment.type = Assignment, .Assignment.variableName = &String197, .Assignment.ruleIdentifier = &Identifier198 };

#define $$ ctx->returnValue
#define yytext ctx->input
#define i1 ctx->vars->data[0]
#define i2 ctx->vars->data[1]
#define i ctx->vars->data[2]
#define e ctx->vars->data[3]
#define l ctx->vars->data[4]
#define c ctx->vars->data[5]

void Action200_function(Context *ctx) {
	 $$ = c; 
}

#undef i1
#undef i2
#undef i
#undef e
#undef l
#undef c
#undef $$
#undef yytext

union Object Action200 = { .Action.type = Action, .Action.value = " $$ = c; ", .Action.function = Action200_function };
union Object Sequence201 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Assignment199, .Binary.rightExpression = &Action200 };
union Object Alternation202 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &Alternation196, .Binary.rightExpression = &Sequence201 };
union Object Identifier203 = { .Identifier.type = Identifier, .Identifier.value = "DOT" };

#define $$ ctx->returnValue
#define yytext ctx->input
#define i1 ctx->vars->data[0]
#define i2 ctx->vars->data[1]
#define i ctx->vars->data[2]
#define e ctx->vars->data[3]
#define l ctx->vars->data[4]
#define c ctx->vars->data[5]

void Action204_function(Context *ctx) {
	 $$ = newDot(); 
}

#undef i1
#undef i2
#undef i
#undef e
#undef l
#undef c
#undef $$
#undef yytext

union Object Action204 = { .Action.type = Action, .Action.value = " $$ = newDot(); ", .Action.function = Action204_function };
union Object Sequence205 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Identifier203, .Binary.rightExpression = &Action204 };
union Object Alternation206 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &Alternation202, .Binary.rightExpression = &Sequence205 };
union Object String207 = { .String.type = String, .String.value = "a" };
union Object Identifier208 = { .Identifier.type = Identifier, .Identifier.value = "action" };
union Object Assignment209 = { .Assignment.type = Assignment, .Assignment.variableName = &String207, .Assignment.ruleIdentifier = &Identifier208 };

#define $$ ctx->returnValue
#define yytext ctx->input
#define i1 ctx->vars->data[0]
#define i2 ctx->vars->data[1]
#define i ctx->vars->data[2]
#define e ctx->vars->data[3]
#define l ctx->vars->data[4]
#define c ctx->vars->data[5]
#define a ctx->vars->data[6]

void Action210_function(Context *ctx) {
	 $$ = a; 
}

#undef i1
#undef i2
#undef i
#undef e
#undef l
#undef c
#undef a
#undef $$
#undef yytext

union Object Action210 = { .Action.type = Action, .Action.value = " $$ = a; ", .Action.function = Action210_function };
union Object Sequence211 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Assignment209, .Binary.rightExpression = &Action210 };
union Object Alternation212 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &Alternation206, .Binary.rightExpression = &Sequence211 };
union Object Identifier213 = { .Identifier.type = Identifier, .Identifier.value = "BEGIN" };

#define $$ ctx->returnValue
#define yytext ctx->input
#define i1 ctx->vars->data[0]
#define i2 ctx->vars->data[1]
#define i ctx->vars->data[2]
#define e ctx->vars->data[3]
#define l ctx->vars->data[4]
#define c ctx->vars->data[5]
#define a ctx->vars->data[6]

void Action214_function(Context *ctx) {
	 $$ = newBegin(); 
}

#undef i1
#undef i2
#undef i
#undef e
#undef l
#undef c
#undef a
#undef $$
#undef yytext

union Object Action214 = { .Action.type = Action, .Action.value = " $$ = newBegin(); ", .Action.function = Action214_function };
union Object Sequence215 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Identifier213, .Binary.rightExpression = &Action214 };
union Object Alternation216 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &Alternation212, .Binary.rightExpression = &Sequence215 };
union Object Identifier217 = { .Identifier.type = Identifier, .Identifier.value = "END" };

#define $$ ctx->returnValue
#define yytext ctx->input
#define i1 ctx->vars->data[0]
#define i2 ctx->vars->data[1]
#define i ctx->vars->data[2]
#define e ctx->vars->data[3]
#define l ctx->vars->data[4]
#define c ctx->vars->data[5]
#define a ctx->vars->data[6]

void Action218_function(Context *ctx) {
	 $$ = newEnd(); 
}

#undef i1
#undef i2
#undef i
#undef e
#undef l
#undef c
#undef a
#undef $$
#undef yytext

union Object Action218 = { .Action.type = Action, .Action.value = " $$ = newEnd(); ", .Action.function = Action218_function };
union Object Sequence219 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Identifier217, .Binary.rightExpression = &Action218 };
union Object Alternation220 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &Alternation216, .Binary.rightExpression = &Sequence219 };
union Object Definition221 = { .Definition.type = Definition, .Definition.name = &String157, .Definition.rule = &Alternation220 };

union Object String222 = { .String.type = String, .String.value = "identifier" };
union Object Begin223 = { .Begin.type = Begin };
union Object CharacterClass224 = { .CharacterClass.type = CharacterClass, .CharacterClass.value = "-a-zA-Z_" };
union Object Sequence225 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Begin223, .Binary.rightExpression = &CharacterClass224 };
union Object CharacterClass226 = { .CharacterClass.type = CharacterClass, .CharacterClass.value = "-a-zA-Z_0-9" };
union Object Star227 = { .Unary.type = Unary, .Unary.op = Star, .Unary.expression = &CharacterClass226 };
union Object Sequence228 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence225, .Binary.rightExpression = &Star227 };
union Object End229 = { .End.type = End };
union Object Sequence230 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence228, .Binary.rightExpression = &End229 };
union Object Identifier231 = { .Identifier.type = Identifier, .Identifier.value = "-" };
union Object Sequence232 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence230, .Binary.rightExpression = &Identifier231 };

#define $$ ctx->returnValue
#define yytext ctx->input

void Action233_function(Context *ctx) {
	 $$ = newString(yytext); 
}

#undef $$
#undef yytext

union Object Action233 = { .Action.type = Action, .Action.value = " $$ = newString(yytext); ", .Action.function = Action233_function };
union Object Sequence234 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence232, .Binary.rightExpression = &Action233 };
union Object Definition235 = { .Definition.type = Definition, .Definition.name = &String222, .Definition.rule = &Sequence234 };

union Object String236 = { .String.type = String, .String.value = "ruleCallIdent" };
union Object Begin237 = { .Begin.type = Begin };
union Object CharacterClass238 = { .CharacterClass.type = CharacterClass, .CharacterClass.value = "-a-zA-Z_" };
union Object Sequence239 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Begin237, .Binary.rightExpression = &CharacterClass238 };
union Object CharacterClass240 = { .CharacterClass.type = CharacterClass, .CharacterClass.value = "-a-zA-Z_0-9" };
union Object Star241 = { .Unary.type = Unary, .Unary.op = Star, .Unary.expression = &CharacterClass240 };
union Object Sequence242 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence239, .Binary.rightExpression = &Star241 };
union Object End243 = { .End.type = End };
union Object Sequence244 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence242, .Binary.rightExpression = &End243 };
union Object Identifier245 = { .Identifier.type = Identifier, .Identifier.value = "-" };
union Object Sequence246 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence244, .Binary.rightExpression = &Identifier245 };

#define $$ ctx->returnValue
#define yytext ctx->input

void Action247_function(Context *ctx) {
	 $$ = newIdentifier(yytext); 
}

#undef $$
#undef yytext

union Object Action247 = { .Action.type = Action, .Action.value = " $$ = newIdentifier(yytext); ", .Action.function = Action247_function };
union Object Sequence248 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence246, .Binary.rightExpression = &Action247 };
union Object Definition249 = { .Definition.type = Definition, .Definition.name = &String236, .Definition.rule = &Sequence248 };

union Object String250 = { .String.type = String, .String.value = "literal" };
union Object CharacterClass251 = { .CharacterClass.type = CharacterClass, .CharacterClass.value = "\'" };
union Object Begin252 = { .Begin.type = Begin };
union Object Sequence253 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &CharacterClass251, .Binary.rightExpression = &Begin252 };
union Object CharacterClass254 = { .CharacterClass.type = CharacterClass, .CharacterClass.value = "\'" };
union Object Not255 = { .Unary.type = Unary, .Unary.op = Not, .Unary.expression = &CharacterClass254 };
union Object Identifier256 = { .Identifier.type = Identifier, .Identifier.value = "char" };
union Object Sequence257 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Not255, .Binary.rightExpression = &Identifier256 };
union Object Star258 = { .Unary.type = Unary, .Unary.op = Star, .Unary.expression = &Sequence257 };
union Object Sequence259 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence253, .Binary.rightExpression = &Star258 };
union Object End260 = { .End.type = End };
union Object Sequence261 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence259, .Binary.rightExpression = &End260 };
union Object CharacterClass262 = { .CharacterClass.type = CharacterClass, .CharacterClass.value = "\'" };
union Object Sequence263 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence261, .Binary.rightExpression = &CharacterClass262 };
union Object Identifier264 = { .Identifier.type = Identifier, .Identifier.value = "-" };
union Object Sequence265 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence263, .Binary.rightExpression = &Identifier264 };

#define $$ ctx->returnValue
#define yytext ctx->input

void Action266_function(Context *ctx) {
	 $$ = newString(yytext); 
}

#undef $$
#undef yytext

union Object Action266 = { .Action.type = Action, .Action.value = " $$ = newString(yytext); ", .Action.function = Action266_function };
union Object Sequence267 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence265, .Binary.rightExpression = &Action266 };
union Object CharacterClass268 = { .CharacterClass.type = CharacterClass, .CharacterClass.value = "\"" };
union Object Begin269 = { .Begin.type = Begin };
union Object Sequence270 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &CharacterClass268, .Binary.rightExpression = &Begin269 };
union Object CharacterClass271 = { .CharacterClass.type = CharacterClass, .CharacterClass.value = "\"" };
union Object Not272 = { .Unary.type = Unary, .Unary.op = Not, .Unary.expression = &CharacterClass271 };
union Object Identifier273 = { .Identifier.type = Identifier, .Identifier.value = "char" };
union Object Sequence274 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Not272, .Binary.rightExpression = &Identifier273 };
union Object Star275 = { .Unary.type = Unary, .Unary.op = Star, .Unary.expression = &Sequence274 };
union Object Sequence276 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence270, .Binary.rightExpression = &Star275 };
union Object End277 = { .End.type = End };
union Object Sequence278 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence276, .Binary.rightExpression = &End277 };
union Object CharacterClass279 = { .CharacterClass.type = CharacterClass, .CharacterClass.value = "\"" };
union Object Sequence280 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence278, .Binary.rightExpression = &CharacterClass279 };
union Object Identifier281 = { .Identifier.type = Identifier, .Identifier.value = "-" };
union Object Sequence282 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence280, .Binary.rightExpression = &Identifier281 };

#define $$ ctx->returnValue
#define yytext ctx->input

void Action283_function(Context *ctx) {
	 $$ = newString(yytext); 
}

#undef $$
#undef yytext

union Object Action283 = { .Action.type = Action, .Action.value = " $$ = newString(yytext); ", .Action.function = Action283_function };
union Object Sequence284 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence282, .Binary.rightExpression = &Action283 };
union Object Alternation285 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &Sequence267, .Binary.rightExpression = &Sequence284 };
union Object Definition286 = { .Definition.type = Definition, .Definition.name = &String250, .Definition.rule = &Alternation285 };

union Object String287 = { .String.type = String, .String.value = "class" };
union Object String288 = { .String.type = String, .String.value = "[" };
union Object Begin289 = { .Begin.type = Begin };
union Object Sequence290 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String288, .Binary.rightExpression = &Begin289 };
union Object String291 = { .String.type = String, .String.value = "]" };
union Object Not292 = { .Unary.type = Unary, .Unary.op = Not, .Unary.expression = &String291 };
union Object Identifier293 = { .Identifier.type = Identifier, .Identifier.value = "range" };
union Object Sequence294 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Not292, .Binary.rightExpression = &Identifier293 };
union Object Star295 = { .Unary.type = Unary, .Unary.op = Star, .Unary.expression = &Sequence294 };
union Object Sequence296 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence290, .Binary.rightExpression = &Star295 };
union Object End297 = { .End.type = End };
union Object Sequence298 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence296, .Binary.rightExpression = &End297 };
union Object String299 = { .String.type = String, .String.value = "]" };
union Object Sequence300 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence298, .Binary.rightExpression = &String299 };
union Object Identifier301 = { .Identifier.type = Identifier, .Identifier.value = "-" };
union Object Sequence302 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence300, .Binary.rightExpression = &Identifier301 };

#define $$ ctx->returnValue
#define yytext ctx->input

void Action303_function(Context *ctx) {
	 $$ = newCharacterClass(yytext); 
}

#undef $$
#undef yytext

union Object Action303 = { .Action.type = Action, .Action.value = " $$ = newCharacterClass(yytext); ", .Action.function = Action303_function };
union Object Sequence304 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence302, .Binary.rightExpression = &Action303 };
union Object Definition305 = { .Definition.type = Definition, .Definition.name = &String287, .Definition.rule = &Sequence304 };

union Object String306 = { .String.type = String, .String.value = "range" };
union Object Identifier307 = { .Identifier.type = Identifier, .Identifier.value = "char" };
union Object String308 = { .String.type = String, .String.value = "-" };
union Object Sequence309 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Identifier307, .Binary.rightExpression = &String308 };
union Object Identifier310 = { .Identifier.type = Identifier, .Identifier.value = "char" };
union Object Sequence311 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence309, .Binary.rightExpression = &Identifier310 };
union Object Identifier312 = { .Identifier.type = Identifier, .Identifier.value = "char" };
union Object Alternation313 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &Sequence311, .Binary.rightExpression = &Identifier312 };
union Object Definition314 = { .Definition.type = Definition, .Definition.name = &String306, .Definition.rule = &Alternation313 };

union Object String315 = { .String.type = String, .String.value = "char" };
union Object String316 = { .String.type = String, .String.value = "\\\\" };
union Object CharacterClass317 = { .CharacterClass.type = CharacterClass, .CharacterClass.value = "abefnrtv\'\"\\[\\]\\\\" };
union Object Sequence318 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String316, .Binary.rightExpression = &CharacterClass317 };
union Object String319 = { .String.type = String, .String.value = "\\\\" };
union Object CharacterClass320 = { .CharacterClass.type = CharacterClass, .CharacterClass.value = "0-3" };
union Object Sequence321 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String319, .Binary.rightExpression = &CharacterClass320 };
union Object CharacterClass322 = { .CharacterClass.type = CharacterClass, .CharacterClass.value = "0-7" };
union Object Sequence323 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence321, .Binary.rightExpression = &CharacterClass322 };
union Object CharacterClass324 = { .CharacterClass.type = CharacterClass, .CharacterClass.value = "0-7" };
union Object Sequence325 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence323, .Binary.rightExpression = &CharacterClass324 };
union Object Alternation326 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &Sequence318, .Binary.rightExpression = &Sequence325 };
union Object String327 = { .String.type = String, .String.value = "\\\\" };
union Object CharacterClass328 = { .CharacterClass.type = CharacterClass, .CharacterClass.value = "0-7" };
union Object Sequence329 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String327, .Binary.rightExpression = &CharacterClass328 };
union Object CharacterClass330 = { .CharacterClass.type = CharacterClass, .CharacterClass.value = "0-7" };
union Object Optional331 = { .Unary.type = Unary, .Unary.op = Optional, .Unary.expression = &CharacterClass330 };
union Object Sequence332 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence329, .Binary.rightExpression = &Optional331 };
union Object Alternation333 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &Alternation326, .Binary.rightExpression = &Sequence332 };
union Object String334 = { .String.type = String, .String.value = "\\\\" };
union Object Not335 = { .Unary.type = Unary, .Unary.op = Not, .Unary.expression = &String334 };
union Object Dot336 = { .Dot.type = Dot };
union Object Sequence337 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Not335, .Binary.rightExpression = &Dot336 };
union Object Alternation338 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &Alternation333, .Binary.rightExpression = &Sequence337 };
union Object Definition339 = { .Definition.type = Definition, .Definition.name = &String315, .Definition.rule = &Alternation338 };

union Object String340 = { .String.type = String, .String.value = "action" };
union Object String341 = { .String.type = String, .String.value = "{" };
union Object Begin342 = { .Begin.type = Begin };
union Object Sequence343 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String341, .Binary.rightExpression = &Begin342 };
union Object Identifier344 = { .Identifier.type = Identifier, .Identifier.value = "braces" };
union Object Star345 = { .Unary.type = Unary, .Unary.op = Star, .Unary.expression = &Identifier344 };
union Object Sequence346 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence343, .Binary.rightExpression = &Star345 };
union Object End347 = { .End.type = End };
union Object Sequence348 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence346, .Binary.rightExpression = &End347 };
union Object String349 = { .String.type = String, .String.value = "}" };
union Object Sequence350 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence348, .Binary.rightExpression = &String349 };
union Object Identifier351 = { .Identifier.type = Identifier, .Identifier.value = "-" };
union Object Sequence352 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence350, .Binary.rightExpression = &Identifier351 };

#define $$ ctx->returnValue
#define yytext ctx->input

void Action353_function(Context *ctx) {
	 $$ = newAction(yytext, NULL); 
}

#undef $$
#undef yytext

union Object Action353 = { .Action.type = Action, .Action.value = " $$ = newAction(yytext, NULL); ", .Action.function = Action353_function };
union Object Sequence354 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence352, .Binary.rightExpression = &Action353 };
union Object Definition355 = { .Definition.type = Definition, .Definition.name = &String340, .Definition.rule = &Sequence354 };

union Object String356 = { .String.type = String, .String.value = "braces" };
union Object String357 = { .String.type = String, .String.value = "{" };
union Object Identifier358 = { .Identifier.type = Identifier, .Identifier.value = "braces" };
union Object Star359 = { .Unary.type = Unary, .Unary.op = Star, .Unary.expression = &Identifier358 };
union Object Sequence360 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String357, .Binary.rightExpression = &Star359 };
union Object String361 = { .String.type = String, .String.value = "}" };
union Object Sequence362 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence360, .Binary.rightExpression = &String361 };
union Object String363 = { .String.type = String, .String.value = "}" };
union Object Not364 = { .Unary.type = Unary, .Unary.op = Not, .Unary.expression = &String363 };
union Object Dot365 = { .Dot.type = Dot };
union Object Sequence366 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Not364, .Binary.rightExpression = &Dot365 };
union Object Alternation367 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &Sequence362, .Binary.rightExpression = &Sequence366 };
union Object Definition368 = { .Definition.type = Definition, .Definition.name = &String356, .Definition.rule = &Alternation367 };

union Object String369 = { .String.type = String, .String.value = "EQUAL" };
union Object String370 = { .String.type = String, .String.value = "=" };
union Object Identifier371 = { .Identifier.type = Identifier, .Identifier.value = "-" };
union Object Sequence372 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String370, .Binary.rightExpression = &Identifier371 };
union Object Definition373 = { .Definition.type = Definition, .Definition.name = &String369, .Definition.rule = &Sequence372 };

union Object String374 = { .String.type = String, .String.value = "COLON" };
union Object String375 = { .String.type = String, .String.value = ":" };
union Object Identifier376 = { .Identifier.type = Identifier, .Identifier.value = "-" };
union Object Sequence377 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String375, .Binary.rightExpression = &Identifier376 };
union Object Definition378 = { .Definition.type = Definition, .Definition.name = &String374, .Definition.rule = &Sequence377 };

union Object String379 = { .String.type = String, .String.value = "SEMICOLON" };
union Object String380 = { .String.type = String, .String.value = ";" };
union Object Identifier381 = { .Identifier.type = Identifier, .Identifier.value = "-" };
union Object Sequence382 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String380, .Binary.rightExpression = &Identifier381 };
union Object Definition383 = { .Definition.type = Definition, .Definition.name = &String379, .Definition.rule = &Sequence382 };

union Object String384 = { .String.type = String, .String.value = "BAR" };
union Object String385 = { .String.type = String, .String.value = "|" };
union Object Identifier386 = { .Identifier.type = Identifier, .Identifier.value = "-" };
union Object Sequence387 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String385, .Binary.rightExpression = &Identifier386 };
union Object Definition388 = { .Definition.type = Definition, .Definition.name = &String384, .Definition.rule = &Sequence387 };

union Object String389 = { .String.type = String, .String.value = "AND" };
union Object String390 = { .String.type = String, .String.value = "&" };
union Object Identifier391 = { .Identifier.type = Identifier, .Identifier.value = "-" };
union Object Sequence392 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String390, .Binary.rightExpression = &Identifier391 };
union Object Definition393 = { .Definition.type = Definition, .Definition.name = &String389, .Definition.rule = &Sequence392 };

union Object String394 = { .String.type = String, .String.value = "NOT" };
union Object String395 = { .String.type = String, .String.value = "!" };
union Object Identifier396 = { .Identifier.type = Identifier, .Identifier.value = "-" };
union Object Sequence397 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String395, .Binary.rightExpression = &Identifier396 };
union Object Definition398 = { .Definition.type = Definition, .Definition.name = &String394, .Definition.rule = &Sequence397 };

union Object String399 = { .String.type = String, .String.value = "QUERY" };
union Object String400 = { .String.type = String, .String.value = "?" };
union Object Identifier401 = { .Identifier.type = Identifier, .Identifier.value = "-" };
union Object Sequence402 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String400, .Binary.rightExpression = &Identifier401 };
union Object Definition403 = { .Definition.type = Definition, .Definition.name = &String399, .Definition.rule = &Sequence402 };

union Object String404 = { .String.type = String, .String.value = "STAR" };
union Object String405 = { .String.type = String, .String.value = "*" };
union Object Identifier406 = { .Identifier.type = Identifier, .Identifier.value = "-" };
union Object Sequence407 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String405, .Binary.rightExpression = &Identifier406 };
union Object Definition408 = { .Definition.type = Definition, .Definition.name = &String404, .Definition.rule = &Sequence407 };

union Object String409 = { .String.type = String, .String.value = "PLUS" };
union Object String410 = { .String.type = String, .String.value = "+" };
union Object Identifier411 = { .Identifier.type = Identifier, .Identifier.value = "-" };
union Object Sequence412 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String410, .Binary.rightExpression = &Identifier411 };
union Object Definition413 = { .Definition.type = Definition, .Definition.name = &String409, .Definition.rule = &Sequence412 };

union Object String414 = { .String.type = String, .String.value = "OPEN" };
union Object String415 = { .String.type = String, .String.value = "(" };
union Object Identifier416 = { .Identifier.type = Identifier, .Identifier.value = "-" };
union Object Sequence417 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String415, .Binary.rightExpression = &Identifier416 };
union Object Definition418 = { .Definition.type = Definition, .Definition.name = &String414, .Definition.rule = &Sequence417 };

union Object String419 = { .String.type = String, .String.value = "CLOSE" };
union Object String420 = { .String.type = String, .String.value = ")" };
union Object Identifier421 = { .Identifier.type = Identifier, .Identifier.value = "-" };
union Object Sequence422 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String420, .Binary.rightExpression = &Identifier421 };
union Object Definition423 = { .Definition.type = Definition, .Definition.name = &String419, .Definition.rule = &Sequence422 };

union Object String424 = { .String.type = String, .String.value = "DOT" };
union Object String425 = { .String.type = String, .String.value = "." };
union Object Identifier426 = { .Identifier.type = Identifier, .Identifier.value = "-" };
union Object Sequence427 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String425, .Binary.rightExpression = &Identifier426 };
union Object Definition428 = { .Definition.type = Definition, .Definition.name = &String424, .Definition.rule = &Sequence427 };

union Object String429 = { .String.type = String, .String.value = "BEGIN" };
union Object String430 = { .String.type = String, .String.value = "<" };
union Object Identifier431 = { .Identifier.type = Identifier, .Identifier.value = "-" };
union Object Sequence432 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String430, .Binary.rightExpression = &Identifier431 };
union Object Definition433 = { .Definition.type = Definition, .Definition.name = &String429, .Definition.rule = &Sequence432 };

union Object String434 = { .String.type = String, .String.value = "END" };
union Object String435 = { .String.type = String, .String.value = ">" };
union Object Identifier436 = { .Identifier.type = Identifier, .Identifier.value = "-" };
union Object Sequence437 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String435, .Binary.rightExpression = &Identifier436 };
union Object Definition438 = { .Definition.type = Definition, .Definition.name = &String434, .Definition.rule = &Sequence437 };

union Object String439 = { .String.type = String, .String.value = "TILDE" };
union Object String440 = { .String.type = String, .String.value = "~" };
union Object Identifier441 = { .Identifier.type = Identifier, .Identifier.value = "-" };
union Object Sequence442 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String440, .Binary.rightExpression = &Identifier441 };
union Object Definition443 = { .Definition.type = Definition, .Definition.name = &String439, .Definition.rule = &Sequence442 };

union Object String444 = { .String.type = String, .String.value = "RPERCENT" };
union Object String445 = { .String.type = String, .String.value = "%}" };
union Object Identifier446 = { .Identifier.type = Identifier, .Identifier.value = "-" };
union Object Sequence447 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String445, .Binary.rightExpression = &Identifier446 };
union Object Definition448 = { .Definition.type = Definition, .Definition.name = &String444, .Definition.rule = &Sequence447 };

union Object String449 = { .String.type = String, .String.value = "-" };
union Object Identifier450 = { .Identifier.type = Identifier, .Identifier.value = "space" };
union Object Identifier451 = { .Identifier.type = Identifier, .Identifier.value = "comment" };
union Object Alternation452 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &Identifier450, .Binary.rightExpression = &Identifier451 };
union Object Star453 = { .Unary.type = Unary, .Unary.op = Star, .Unary.expression = &Alternation452 };
union Object Definition454 = { .Definition.type = Definition, .Definition.name = &String449, .Definition.rule = &Star453 };

union Object String455 = { .String.type = String, .String.value = "space" };
union Object String456 = { .String.type = String, .String.value = " " };
union Object String457 = { .String.type = String, .String.value = "\\t" };
union Object Alternation458 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &String456, .Binary.rightExpression = &String457 };
union Object Identifier459 = { .Identifier.type = Identifier, .Identifier.value = "end-of-line" };
union Object Alternation460 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &Alternation458, .Binary.rightExpression = &Identifier459 };
union Object Definition461 = { .Definition.type = Definition, .Definition.name = &String455, .Definition.rule = &Alternation460 };

union Object String462 = { .String.type = String, .String.value = "comment" };
union Object String463 = { .String.type = String, .String.value = "#" };
union Object Identifier464 = { .Identifier.type = Identifier, .Identifier.value = "end-of-line" };
union Object Not465 = { .Unary.type = Unary, .Unary.op = Not, .Unary.expression = &Identifier464 };
union Object Dot466 = { .Dot.type = Dot };
union Object Sequence467 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Not465, .Binary.rightExpression = &Dot466 };
union Object Star468 = { .Unary.type = Unary, .Unary.op = Star, .Unary.expression = &Sequence467 };
union Object Sequence469 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &String463, .Binary.rightExpression = &Star468 };
union Object Identifier470 = { .Identifier.type = Identifier, .Identifier.value = "end-of-line" };
union Object Sequence471 = { .Binary.type = Binary, .Binary.op = Sequence, .Binary.leftExpression = &Sequence469, .Binary.rightExpression = &Identifier470 };
union Object Definition472 = { .Definition.type = Definition, .Definition.name = &String462, .Definition.rule = &Sequence471 };

union Object String473 = { .String.type = String, .String.value = "end-of-line" };
union Object String474 = { .String.type = String, .String.value = "\\r\\n" };
union Object String475 = { .String.type = String, .String.value = "\\n" };
union Object Alternation476 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &String474, .Binary.rightExpression = &String475 };
union Object String477 = { .String.type = String, .String.value = "\\r" };
union Object Alternation478 = { .Binary.type = Binary, .Binary.op = Alternation, .Binary.leftExpression = &Alternation476, .Binary.rightExpression = &String477 };
union Object Definition479 = { .Definition.type = Definition, .Definition.name = &String473, .Definition.rule = &Alternation478 };

union Object String480 = { .String.type = String, .String.value = "end-of-file" };
union Object Dot481 = { .Dot.type = Dot };
union Object Not482 = { .Unary.type = Unary, .Unary.op = Not, .Unary.expression = &Dot481 };
union Object Definition483 = { .Definition.type = Definition, .Definition.name = &String480, .Definition.rule = &Not482 };

List definitions = { .size = 42, .used = 40, .data = { &Definition23, &Definition26, &Definition41, &Definition51, &Definition67, &Definition83, &Definition97, &Definition109, &Definition136, &Definition156, &Definition221, &Definition235, &Definition249, &Definition286, &Definition305, &Definition314, &Definition339, &Definition355, &Definition368, &Definition373, &Definition378, &Definition383, &Definition388, &Definition393, &Definition398, &Definition403, &Definition408, &Definition413, &Definition418, &Definition423, &Definition428, &Definition433, &Definition438, &Definition443, &Definition448, &Definition454, &Definition461, &Definition472, &Definition479, &Definition483, } };

union Object grammar = { .Grammar.type = Grammar, .Grammar.definitions = &definitions };

oop getGrammar() {
	 return &grammar;
}

