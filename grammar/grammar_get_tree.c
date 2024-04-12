/// Actions

#include "grammar_objects.h"


#define $$ ctx->returnValue
#define yytext ctx->input
#define r ctx->vars->data[0]
#define d ctx->vars->data[1]

void Action11_function(Context *ctx) {
	 getMethod(r, "addRuleDefinition")(r, d); 
}

#undef r
#undef d
#undef $$
#undef yytext


#define $$ ctx->returnValue
#define yytext ctx->input
#define r ctx->vars->data[0]
#define d ctx->vars->data[1]

void Action21_function(Context *ctx) {
	 printTree(r); writeTree(r); 
}

#undef r
#undef d
#undef $$
#undef yytext


#define $$ ctx->returnValue
#define yytext ctx->input

void Action25_function(Context *ctx) {
	 $$ = newGrammar(); 
}

#undef $$
#undef yytext


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


#define $$ ctx->returnValue
#define yytext ctx->input
#define s1 ctx->vars->data[0]
#define s2 ctx->vars->data[1]

void Action77_function(Context *ctx) {
	 s1 = newAlternation(s1, s2); 
}

#undef s1
#undef s2
#undef $$
#undef yytext


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


#define $$ ctx->returnValue
#define yytext ctx->input
#define e ctx->vars->data[0]
#define f ctx->vars->data[1]

void Action91_function(Context *ctx) {
	 e = newSequence(e, f); 
}

#undef e
#undef f
#undef $$
#undef yytext


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


#define $$ ctx->returnValue
#define yytext ctx->input
#define p ctx->vars->data[0]

void Action107_function(Context *ctx) {
	 $$ = p; 
}

#undef p
#undef $$
#undef yytext


#define $$ ctx->returnValue
#define yytext ctx->input
#define s ctx->vars->data[0]

void Action119_function(Context *ctx) {
	 $$ = newAnd(s); 
}

#undef s
#undef $$
#undef yytext


#define $$ ctx->returnValue
#define yytext ctx->input
#define s ctx->vars->data[0]
#define s ctx->vars->data[1]

void Action126_function(Context *ctx) {
	 $$ = newNot(s); 
}

#undef s
#undef s
#undef $$
#undef yytext


#define $$ ctx->returnValue
#define yytext ctx->input
#define s ctx->vars->data[0]
#define s ctx->vars->data[1]
#define s ctx->vars->data[2]

void Action132_function(Context *ctx) {
	 $$ = s; 
}

#undef s
#undef s
#undef s
#undef $$
#undef yytext


#define $$ ctx->returnValue
#define yytext ctx->input
#define p ctx->vars->data[0]

void Action142_function(Context *ctx) {
	 p = newOptional(p); 
}

#undef p
#undef $$
#undef yytext


#define $$ ctx->returnValue
#define yytext ctx->input
#define p ctx->vars->data[0]

void Action145_function(Context *ctx) {
	 p = newStar(p); 
}

#undef p
#undef $$
#undef yytext


#define $$ ctx->returnValue
#define yytext ctx->input
#define p ctx->vars->data[0]

void Action149_function(Context *ctx) {
	 p = newPlus(p); 
}

#undef p
#undef $$
#undef yytext


#define $$ ctx->returnValue
#define yytext ctx->input
#define p ctx->vars->data[0]

void Action154_function(Context *ctx) {
	 $$ = p; 
}

#undef p
#undef $$
#undef yytext


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


#define $$ ctx->returnValue
#define yytext ctx->input

void Action233_function(Context *ctx) {
	 $$ = newString(yytext); 
}

#undef $$
#undef yytext


#define $$ ctx->returnValue
#define yytext ctx->input

void Action247_function(Context *ctx) {
	 $$ = newIdentifier(yytext); 
}

#undef $$
#undef yytext


#define $$ ctx->returnValue
#define yytext ctx->input

void Action266_function(Context *ctx) {
	 $$ = newString(yytext); 
}

#undef $$
#undef yytext


#define $$ ctx->returnValue
#define yytext ctx->input

void Action283_function(Context *ctx) {
	 $$ = newString(yytext); 
}

#undef $$
#undef yytext


#define $$ ctx->returnValue
#define yytext ctx->input

void Action303_function(Context *ctx) {
	 $$ = newCharacterClass(yytext); 
}

#undef $$
#undef yytext


#define $$ ctx->returnValue
#define yytext ctx->input

void Action353_function(Context *ctx) {
	 $$ = newAction(yytext, NULL); 
}

#undef $$
#undef yytext


/// Declarations

oop getGrammar() {
oop grammar = newGrammar();
oop String1 = newString("grammar");
oop Identifier2 = newIdentifier("-");
oop String3 = newString("r");
oop Identifier4 = newIdentifier("root");
oop Assignment5 = newAssignment(String3, Identifier4);
oop Sequence6 = newSequence(Identifier2, Assignment5);
oop Identifier7 = newIdentifier("declaration");
oop String8 = newString("d");
oop Identifier9 = newIdentifier("definition");
oop Assignment10 = newAssignment(String8, Identifier9);
oop Action11 = newAction(" getMethod(r, \"addRuleDefinition\")(r, d); ", Action11_function);
oop Sequence12 = newSequence(Assignment10, Action11);
oop Alternation13 = newAlternation(Identifier7, Sequence12);
oop Plus14 = newPlus(Alternation13);
oop Sequence15 = newSequence(Sequence6, Plus14);
oop Identifier16 = newIdentifier("trailer");
oop Optional17 = newOptional(Identifier16);
oop Sequence18 = newSequence(Sequence15, Optional17);
oop Identifier19 = newIdentifier("end-of-file");
oop Sequence20 = newSequence(Sequence18, Identifier19);
oop Action21 = newAction(" printTree(r); writeTree(r); ", Action21_function);
oop Sequence22 = newSequence(Sequence20, Action21);
oop Definition23 = newDefinition(String1, Sequence22);

push((List*)getProperty(grammar, "definitions"), Definition23);

oop String24 = newString("root");
oop Action25 = newAction(" $$ = newGrammar(); ", Action25_function);
oop Definition26 = newDefinition(String24, Action25);

push((List*)getProperty(grammar, "definitions"), Definition26);

oop String27 = newString("declaration");
oop String28 = newString("%{");
oop Begin29 = newBegin();oop Sequence30 = newSequence(String28, Begin29);
oop String31 = newString("%}");
oop Not32 = newNot(String31);
oop Dot33 = newDot();oop Sequence34 = newSequence(Not32, Dot33);
oop Star35 = newStar(Sequence34);
oop Sequence36 = newSequence(Sequence30, Star35);
oop End37 = newEnd();oop Sequence38 = newSequence(Sequence36, End37);
oop Identifier39 = newIdentifier("RPERCENT");
oop Sequence40 = newSequence(Sequence38, Identifier39);
oop Definition41 = newDefinition(String27, Sequence40);

push((List*)getProperty(grammar, "definitions"), Definition41);

oop String42 = newString("trailer");
oop String43 = newString("%%");
oop Begin44 = newBegin();oop Sequence45 = newSequence(String43, Begin44);
oop Dot46 = newDot();oop Star47 = newStar(Dot46);
oop Sequence48 = newSequence(Sequence45, Star47);
oop End49 = newEnd();oop Sequence50 = newSequence(Sequence48, End49);
oop Definition51 = newDefinition(String42, Sequence50);

push((List*)getProperty(grammar, "definitions"), Definition51);

oop String52 = newString("definition");
oop String53 = newString("i");
oop Identifier54 = newIdentifier("identifier");
oop Assignment55 = newAssignment(String53, Identifier54);
oop Identifier56 = newIdentifier("EQUAL");
oop Sequence57 = newSequence(Assignment55, Identifier56);
oop String58 = newString("e");
oop Identifier59 = newIdentifier("expression");
oop Assignment60 = newAssignment(String58, Identifier59);
oop Sequence61 = newSequence(Sequence57, Assignment60);
oop Identifier62 = newIdentifier("SEMICOLON");
oop Optional63 = newOptional(Identifier62);
oop Sequence64 = newSequence(Sequence61, Optional63);
oop Action65 = newAction(" $$ = newDefinition(i, e); ", Action65_function);
oop Sequence66 = newSequence(Sequence64, Action65);
oop Definition67 = newDefinition(String52, Sequence66);

push((List*)getProperty(grammar, "definitions"), Definition67);

oop String68 = newString("expression");
oop String69 = newString("s1");
oop Identifier70 = newIdentifier("sequence");
oop Assignment71 = newAssignment(String69, Identifier70);
oop Identifier72 = newIdentifier("BAR");
oop String73 = newString("s2");
oop Identifier74 = newIdentifier("sequence");
oop Assignment75 = newAssignment(String73, Identifier74);
oop Sequence76 = newSequence(Identifier72, Assignment75);
oop Action77 = newAction(" s1 = newAlternation(s1, s2); ", Action77_function);
oop Sequence78 = newSequence(Sequence76, Action77);
oop Star79 = newStar(Sequence78);
oop Sequence80 = newSequence(Assignment71, Star79);
oop Action81 = newAction(" $$ = s1; ", Action81_function);
oop Sequence82 = newSequence(Sequence80, Action81);
oop Definition83 = newDefinition(String68, Sequence82);

push((List*)getProperty(grammar, "definitions"), Definition83);

oop String84 = newString("sequence");
oop String85 = newString("e");
oop Identifier86 = newIdentifier("error");
oop Assignment87 = newAssignment(String85, Identifier86);
oop String88 = newString("f");
oop Identifier89 = newIdentifier("error");
oop Assignment90 = newAssignment(String88, Identifier89);
oop Action91 = newAction(" e = newSequence(e, f); ", Action91_function);
oop Sequence92 = newSequence(Assignment90, Action91);
oop Star93 = newStar(Sequence92);
oop Sequence94 = newSequence(Assignment87, Star93);
oop Action95 = newAction(" $$ = e; ", Action95_function);
oop Sequence96 = newSequence(Sequence94, Action95);
oop Definition97 = newDefinition(String84, Sequence96);

push((List*)getProperty(grammar, "definitions"), Definition97);

oop String98 = newString("error");
oop String99 = newString("p");
oop Identifier100 = newIdentifier("prefix");
oop Assignment101 = newAssignment(String99, Identifier100);
oop Identifier102 = newIdentifier("TILDE");
oop Identifier103 = newIdentifier("action");
oop Sequence104 = newSequence(Identifier102, Identifier103);
oop Optional105 = newOptional(Sequence104);
oop Sequence106 = newSequence(Assignment101, Optional105);
oop Action107 = newAction(" $$ = p; ", Action107_function);
oop Sequence108 = newSequence(Sequence106, Action107);
oop Definition109 = newDefinition(String98, Sequence108);

push((List*)getProperty(grammar, "definitions"), Definition109);

oop String110 = newString("prefix");
oop Identifier111 = newIdentifier("AND");
oop Identifier112 = newIdentifier("action");
oop Sequence113 = newSequence(Identifier111, Identifier112);
oop Identifier114 = newIdentifier("AND");
oop String115 = newString("s");
oop Identifier116 = newIdentifier("suffix");
oop Assignment117 = newAssignment(String115, Identifier116);
oop Sequence118 = newSequence(Identifier114, Assignment117);
oop Action119 = newAction(" $$ = newAnd(s); ", Action119_function);
oop Sequence120 = newSequence(Sequence118, Action119);
oop Identifier121 = newIdentifier("NOT");
oop String122 = newString("s");
oop Identifier123 = newIdentifier("suffix");
oop Assignment124 = newAssignment(String122, Identifier123);
oop Sequence125 = newSequence(Identifier121, Assignment124);
oop Action126 = newAction(" $$ = newNot(s); ", Action126_function);
oop Sequence127 = newSequence(Sequence125, Action126);
oop Alternation128 = newAlternation(Sequence120, Sequence127);
oop String129 = newString("s");
oop Identifier130 = newIdentifier("suffix");
oop Assignment131 = newAssignment(String129, Identifier130);
oop Action132 = newAction(" $$ = s; ", Action132_function);
oop Sequence133 = newSequence(Assignment131, Action132);
oop Alternation134 = newAlternation(Alternation128, Sequence133);
oop Alternation135 = newAlternation(Sequence113, Alternation134);
oop Definition136 = newDefinition(String110, Alternation135);

push((List*)getProperty(grammar, "definitions"), Definition136);

oop String137 = newString("suffix");
oop String138 = newString("p");
oop Identifier139 = newIdentifier("primary");
oop Assignment140 = newAssignment(String138, Identifier139);
oop Identifier141 = newIdentifier("QUERY");
oop Action142 = newAction(" p = newOptional(p); ", Action142_function);
oop Sequence143 = newSequence(Identifier141, Action142);
oop Identifier144 = newIdentifier("STAR");
oop Action145 = newAction(" p = newStar(p); ", Action145_function);
oop Sequence146 = newSequence(Identifier144, Action145);
oop Alternation147 = newAlternation(Sequence143, Sequence146);
oop Identifier148 = newIdentifier("PLUS");
oop Action149 = newAction(" p = newPlus(p); ", Action149_function);
oop Sequence150 = newSequence(Identifier148, Action149);
oop Alternation151 = newAlternation(Alternation147, Sequence150);
oop Optional152 = newOptional(Alternation151);
oop Sequence153 = newSequence(Assignment140, Optional152);
oop Action154 = newAction(" $$ = p; ", Action154_function);
oop Sequence155 = newSequence(Sequence153, Action154);
oop Definition156 = newDefinition(String137, Sequence155);

push((List*)getProperty(grammar, "definitions"), Definition156);

oop String157 = newString("primary");
oop String158 = newString("i1");
oop Identifier159 = newIdentifier("identifier");
oop Assignment160 = newAssignment(String158, Identifier159);
oop Identifier161 = newIdentifier("COLON");
oop Sequence162 = newSequence(Assignment160, Identifier161);
oop String163 = newString("i2");
oop Identifier164 = newIdentifier("ruleCallIdent");
oop Assignment165 = newAssignment(String163, Identifier164);
oop Sequence166 = newSequence(Sequence162, Assignment165);
oop Identifier167 = newIdentifier("EQUAL");
oop Not168 = newNot(Identifier167);
oop Sequence169 = newSequence(Sequence166, Not168);
oop Action170 = newAction(" $$ = newAssignment(i1, i2); ", Action170_function);
oop Sequence171 = newSequence(Sequence169, Action170);
oop String172 = newString("i");
oop Identifier173 = newIdentifier("ruleCallIdent");
oop Assignment174 = newAssignment(String172, Identifier173);
oop Identifier175 = newIdentifier("EQUAL");
oop Not176 = newNot(Identifier175);
oop Sequence177 = newSequence(Assignment174, Not176);
oop Action178 = newAction(" $$ = i; ", Action178_function);
oop Sequence179 = newSequence(Sequence177, Action178);
oop Alternation180 = newAlternation(Sequence171, Sequence179);
oop Identifier181 = newIdentifier("OPEN");
oop String182 = newString("e");
oop Identifier183 = newIdentifier("expression");
oop Assignment184 = newAssignment(String182, Identifier183);
oop Sequence185 = newSequence(Identifier181, Assignment184);
oop Identifier186 = newIdentifier("CLOSE");
oop Sequence187 = newSequence(Sequence185, Identifier186);
oop Action188 = newAction(" $$ = e; ", Action188_function);
oop Sequence189 = newSequence(Sequence187, Action188);
oop Alternation190 = newAlternation(Alternation180, Sequence189);
oop String191 = newString("l");
oop Identifier192 = newIdentifier("literal");
oop Assignment193 = newAssignment(String191, Identifier192);
oop Action194 = newAction(" $$ = l; ", Action194_function);
oop Sequence195 = newSequence(Assignment193, Action194);
oop Alternation196 = newAlternation(Alternation190, Sequence195);
oop String197 = newString("c");
oop Identifier198 = newIdentifier("class");
oop Assignment199 = newAssignment(String197, Identifier198);
oop Action200 = newAction(" $$ = c; ", Action200_function);
oop Sequence201 = newSequence(Assignment199, Action200);
oop Alternation202 = newAlternation(Alternation196, Sequence201);
oop Identifier203 = newIdentifier("DOT");
oop Action204 = newAction(" $$ = newDot(); ", Action204_function);
oop Sequence205 = newSequence(Identifier203, Action204);
oop Alternation206 = newAlternation(Alternation202, Sequence205);
oop String207 = newString("a");
oop Identifier208 = newIdentifier("action");
oop Assignment209 = newAssignment(String207, Identifier208);
oop Action210 = newAction(" $$ = a; ", Action210_function);
oop Sequence211 = newSequence(Assignment209, Action210);
oop Alternation212 = newAlternation(Alternation206, Sequence211);
oop Identifier213 = newIdentifier("BEGIN");
oop Action214 = newAction(" $$ = newBegin(); ", Action214_function);
oop Sequence215 = newSequence(Identifier213, Action214);
oop Alternation216 = newAlternation(Alternation212, Sequence215);
oop Identifier217 = newIdentifier("END");
oop Action218 = newAction(" $$ = newEnd(); ", Action218_function);
oop Sequence219 = newSequence(Identifier217, Action218);
oop Alternation220 = newAlternation(Alternation216, Sequence219);
oop Definition221 = newDefinition(String157, Alternation220);

push((List*)getProperty(grammar, "definitions"), Definition221);

oop String222 = newString("identifier");
oop Begin223 = newBegin();oop CharacterClass224 = newCharacterClass("-a-zA-Z_");
oop Sequence225 = newSequence(Begin223, CharacterClass224);
oop CharacterClass226 = newCharacterClass("-a-zA-Z_0-9");
oop Star227 = newStar(CharacterClass226);
oop Sequence228 = newSequence(Sequence225, Star227);
oop End229 = newEnd();oop Sequence230 = newSequence(Sequence228, End229);
oop Identifier231 = newIdentifier("-");
oop Sequence232 = newSequence(Sequence230, Identifier231);
oop Action233 = newAction(" $$ = newString(yytext); ", Action233_function);
oop Sequence234 = newSequence(Sequence232, Action233);
oop Definition235 = newDefinition(String222, Sequence234);

push((List*)getProperty(grammar, "definitions"), Definition235);

oop String236 = newString("ruleCallIdent");
oop Begin237 = newBegin();oop CharacterClass238 = newCharacterClass("-a-zA-Z_");
oop Sequence239 = newSequence(Begin237, CharacterClass238);
oop CharacterClass240 = newCharacterClass("-a-zA-Z_0-9");
oop Star241 = newStar(CharacterClass240);
oop Sequence242 = newSequence(Sequence239, Star241);
oop End243 = newEnd();oop Sequence244 = newSequence(Sequence242, End243);
oop Identifier245 = newIdentifier("-");
oop Sequence246 = newSequence(Sequence244, Identifier245);
oop Action247 = newAction(" $$ = newIdentifier(yytext); ", Action247_function);
oop Sequence248 = newSequence(Sequence246, Action247);
oop Definition249 = newDefinition(String236, Sequence248);

push((List*)getProperty(grammar, "definitions"), Definition249);

oop String250 = newString("literal");
oop CharacterClass251 = newCharacterClass("\'");
oop Begin252 = newBegin();oop Sequence253 = newSequence(CharacterClass251, Begin252);
oop CharacterClass254 = newCharacterClass("\'");
oop Not255 = newNot(CharacterClass254);
oop Identifier256 = newIdentifier("char");
oop Sequence257 = newSequence(Not255, Identifier256);
oop Star258 = newStar(Sequence257);
oop Sequence259 = newSequence(Sequence253, Star258);
oop End260 = newEnd();oop Sequence261 = newSequence(Sequence259, End260);
oop CharacterClass262 = newCharacterClass("\'");
oop Sequence263 = newSequence(Sequence261, CharacterClass262);
oop Identifier264 = newIdentifier("-");
oop Sequence265 = newSequence(Sequence263, Identifier264);
oop Action266 = newAction(" $$ = newString(yytext); ", Action266_function);
oop Sequence267 = newSequence(Sequence265, Action266);
oop CharacterClass268 = newCharacterClass("\"");
oop Begin269 = newBegin();oop Sequence270 = newSequence(CharacterClass268, Begin269);
oop CharacterClass271 = newCharacterClass("\"");
oop Not272 = newNot(CharacterClass271);
oop Identifier273 = newIdentifier("char");
oop Sequence274 = newSequence(Not272, Identifier273);
oop Star275 = newStar(Sequence274);
oop Sequence276 = newSequence(Sequence270, Star275);
oop End277 = newEnd();oop Sequence278 = newSequence(Sequence276, End277);
oop CharacterClass279 = newCharacterClass("\"");
oop Sequence280 = newSequence(Sequence278, CharacterClass279);
oop Identifier281 = newIdentifier("-");
oop Sequence282 = newSequence(Sequence280, Identifier281);
oop Action283 = newAction(" $$ = newString(yytext); ", Action283_function);
oop Sequence284 = newSequence(Sequence282, Action283);
oop Alternation285 = newAlternation(Sequence267, Sequence284);
oop Definition286 = newDefinition(String250, Alternation285);

push((List*)getProperty(grammar, "definitions"), Definition286);

oop String287 = newString("class");
oop String288 = newString("[");
oop Begin289 = newBegin();oop Sequence290 = newSequence(String288, Begin289);
oop String291 = newString("]");
oop Not292 = newNot(String291);
oop Identifier293 = newIdentifier("range");
oop Sequence294 = newSequence(Not292, Identifier293);
oop Star295 = newStar(Sequence294);
oop Sequence296 = newSequence(Sequence290, Star295);
oop End297 = newEnd();oop Sequence298 = newSequence(Sequence296, End297);
oop String299 = newString("]");
oop Sequence300 = newSequence(Sequence298, String299);
oop Identifier301 = newIdentifier("-");
oop Sequence302 = newSequence(Sequence300, Identifier301);
oop Action303 = newAction(" $$ = newCharacterClass(yytext); ", Action303_function);
oop Sequence304 = newSequence(Sequence302, Action303);
oop Definition305 = newDefinition(String287, Sequence304);

push((List*)getProperty(grammar, "definitions"), Definition305);

oop String306 = newString("range");
oop Identifier307 = newIdentifier("char");
oop String308 = newString("-");
oop Sequence309 = newSequence(Identifier307, String308);
oop Identifier310 = newIdentifier("char");
oop Sequence311 = newSequence(Sequence309, Identifier310);
oop Identifier312 = newIdentifier("char");
oop Alternation313 = newAlternation(Sequence311, Identifier312);
oop Definition314 = newDefinition(String306, Alternation313);

push((List*)getProperty(grammar, "definitions"), Definition314);

oop String315 = newString("char");
oop String316 = newString("\\\\");
oop CharacterClass317 = newCharacterClass("abefnrtv\'\"\\[\\]\\\\");
oop Sequence318 = newSequence(String316, CharacterClass317);
oop String319 = newString("\\\\");
oop CharacterClass320 = newCharacterClass("0-3");
oop Sequence321 = newSequence(String319, CharacterClass320);
oop CharacterClass322 = newCharacterClass("0-7");
oop Sequence323 = newSequence(Sequence321, CharacterClass322);
oop CharacterClass324 = newCharacterClass("0-7");
oop Sequence325 = newSequence(Sequence323, CharacterClass324);
oop Alternation326 = newAlternation(Sequence318, Sequence325);
oop String327 = newString("\\\\");
oop CharacterClass328 = newCharacterClass("0-7");
oop Sequence329 = newSequence(String327, CharacterClass328);
oop CharacterClass330 = newCharacterClass("0-7");
oop Optional331 = newOptional(CharacterClass330);
oop Sequence332 = newSequence(Sequence329, Optional331);
oop Alternation333 = newAlternation(Alternation326, Sequence332);
oop String334 = newString("\\\\");
oop Not335 = newNot(String334);
oop Dot336 = newDot();oop Sequence337 = newSequence(Not335, Dot336);
oop Alternation338 = newAlternation(Alternation333, Sequence337);
oop Definition339 = newDefinition(String315, Alternation338);

push((List*)getProperty(grammar, "definitions"), Definition339);

oop String340 = newString("action");
oop String341 = newString("{");
oop Begin342 = newBegin();oop Sequence343 = newSequence(String341, Begin342);
oop Identifier344 = newIdentifier("braces");
oop Star345 = newStar(Identifier344);
oop Sequence346 = newSequence(Sequence343, Star345);
oop End347 = newEnd();oop Sequence348 = newSequence(Sequence346, End347);
oop String349 = newString("}");
oop Sequence350 = newSequence(Sequence348, String349);
oop Identifier351 = newIdentifier("-");
oop Sequence352 = newSequence(Sequence350, Identifier351);
oop Action353 = newAction(" $$ = newAction(yytext, NULL); ", Action353_function);
oop Sequence354 = newSequence(Sequence352, Action353);
oop Definition355 = newDefinition(String340, Sequence354);

push((List*)getProperty(grammar, "definitions"), Definition355);

oop String356 = newString("braces");
oop String357 = newString("{");
oop Identifier358 = newIdentifier("braces");
oop Star359 = newStar(Identifier358);
oop Sequence360 = newSequence(String357, Star359);
oop String361 = newString("}");
oop Sequence362 = newSequence(Sequence360, String361);
oop String363 = newString("}");
oop Not364 = newNot(String363);
oop Dot365 = newDot();oop Sequence366 = newSequence(Not364, Dot365);
oop Alternation367 = newAlternation(Sequence362, Sequence366);
oop Definition368 = newDefinition(String356, Alternation367);

push((List*)getProperty(grammar, "definitions"), Definition368);

oop String369 = newString("EQUAL");
oop String370 = newString("=");
oop Identifier371 = newIdentifier("-");
oop Sequence372 = newSequence(String370, Identifier371);
oop Definition373 = newDefinition(String369, Sequence372);

push((List*)getProperty(grammar, "definitions"), Definition373);

oop String374 = newString("COLON");
oop String375 = newString(":");
oop Identifier376 = newIdentifier("-");
oop Sequence377 = newSequence(String375, Identifier376);
oop Definition378 = newDefinition(String374, Sequence377);

push((List*)getProperty(grammar, "definitions"), Definition378);

oop String379 = newString("SEMICOLON");
oop String380 = newString(";");
oop Identifier381 = newIdentifier("-");
oop Sequence382 = newSequence(String380, Identifier381);
oop Definition383 = newDefinition(String379, Sequence382);

push((List*)getProperty(grammar, "definitions"), Definition383);

oop String384 = newString("BAR");
oop String385 = newString("|");
oop Identifier386 = newIdentifier("-");
oop Sequence387 = newSequence(String385, Identifier386);
oop Definition388 = newDefinition(String384, Sequence387);

push((List*)getProperty(grammar, "definitions"), Definition388);

oop String389 = newString("AND");
oop String390 = newString("&");
oop Identifier391 = newIdentifier("-");
oop Sequence392 = newSequence(String390, Identifier391);
oop Definition393 = newDefinition(String389, Sequence392);

push((List*)getProperty(grammar, "definitions"), Definition393);

oop String394 = newString("NOT");
oop String395 = newString("!");
oop Identifier396 = newIdentifier("-");
oop Sequence397 = newSequence(String395, Identifier396);
oop Definition398 = newDefinition(String394, Sequence397);

push((List*)getProperty(grammar, "definitions"), Definition398);

oop String399 = newString("QUERY");
oop String400 = newString("?");
oop Identifier401 = newIdentifier("-");
oop Sequence402 = newSequence(String400, Identifier401);
oop Definition403 = newDefinition(String399, Sequence402);

push((List*)getProperty(grammar, "definitions"), Definition403);

oop String404 = newString("STAR");
oop String405 = newString("*");
oop Identifier406 = newIdentifier("-");
oop Sequence407 = newSequence(String405, Identifier406);
oop Definition408 = newDefinition(String404, Sequence407);

push((List*)getProperty(grammar, "definitions"), Definition408);

oop String409 = newString("PLUS");
oop String410 = newString("+");
oop Identifier411 = newIdentifier("-");
oop Sequence412 = newSequence(String410, Identifier411);
oop Definition413 = newDefinition(String409, Sequence412);

push((List*)getProperty(grammar, "definitions"), Definition413);

oop String414 = newString("OPEN");
oop String415 = newString("(");
oop Identifier416 = newIdentifier("-");
oop Sequence417 = newSequence(String415, Identifier416);
oop Definition418 = newDefinition(String414, Sequence417);

push((List*)getProperty(grammar, "definitions"), Definition418);

oop String419 = newString("CLOSE");
oop String420 = newString(")");
oop Identifier421 = newIdentifier("-");
oop Sequence422 = newSequence(String420, Identifier421);
oop Definition423 = newDefinition(String419, Sequence422);

push((List*)getProperty(grammar, "definitions"), Definition423);

oop String424 = newString("DOT");
oop String425 = newString(".");
oop Identifier426 = newIdentifier("-");
oop Sequence427 = newSequence(String425, Identifier426);
oop Definition428 = newDefinition(String424, Sequence427);

push((List*)getProperty(grammar, "definitions"), Definition428);

oop String429 = newString("BEGIN");
oop String430 = newString("<");
oop Identifier431 = newIdentifier("-");
oop Sequence432 = newSequence(String430, Identifier431);
oop Definition433 = newDefinition(String429, Sequence432);

push((List*)getProperty(grammar, "definitions"), Definition433);

oop String434 = newString("END");
oop String435 = newString(">");
oop Identifier436 = newIdentifier("-");
oop Sequence437 = newSequence(String435, Identifier436);
oop Definition438 = newDefinition(String434, Sequence437);

push((List*)getProperty(grammar, "definitions"), Definition438);

oop String439 = newString("TILDE");
oop String440 = newString("~");
oop Identifier441 = newIdentifier("-");
oop Sequence442 = newSequence(String440, Identifier441);
oop Definition443 = newDefinition(String439, Sequence442);

push((List*)getProperty(grammar, "definitions"), Definition443);

oop String444 = newString("RPERCENT");
oop String445 = newString("%}");
oop Identifier446 = newIdentifier("-");
oop Sequence447 = newSequence(String445, Identifier446);
oop Definition448 = newDefinition(String444, Sequence447);

push((List*)getProperty(grammar, "definitions"), Definition448);

oop String449 = newString("-");
oop Identifier450 = newIdentifier("space");
oop Identifier451 = newIdentifier("comment");
oop Alternation452 = newAlternation(Identifier450, Identifier451);
oop Star453 = newStar(Alternation452);
oop Definition454 = newDefinition(String449, Star453);

push((List*)getProperty(grammar, "definitions"), Definition454);

oop String455 = newString("space");
oop String456 = newString(" ");
oop String457 = newString("\\t");
oop Alternation458 = newAlternation(String456, String457);
oop Identifier459 = newIdentifier("end-of-line");
oop Alternation460 = newAlternation(Alternation458, Identifier459);
oop Definition461 = newDefinition(String455, Alternation460);

push((List*)getProperty(grammar, "definitions"), Definition461);

oop String462 = newString("comment");
oop String463 = newString("#");
oop Identifier464 = newIdentifier("end-of-line");
oop Not465 = newNot(Identifier464);
oop Dot466 = newDot();oop Sequence467 = newSequence(Not465, Dot466);
oop Star468 = newStar(Sequence467);
oop Sequence469 = newSequence(String463, Star468);
oop Identifier470 = newIdentifier("end-of-line");
oop Sequence471 = newSequence(Sequence469, Identifier470);
oop Definition472 = newDefinition(String462, Sequence471);

push((List*)getProperty(grammar, "definitions"), Definition472);

oop String473 = newString("end-of-line");
oop String474 = newString("\\r\\n");
oop String475 = newString("\\n");
oop Alternation476 = newAlternation(String474, String475);
oop String477 = newString("\\r");
oop Alternation478 = newAlternation(Alternation476, String477);
oop Definition479 = newDefinition(String473, Alternation478);

push((List*)getProperty(grammar, "definitions"), Definition479);

oop String480 = newString("end-of-file");
oop Dot481 = newDot();oop Not482 = newNot(Dot481);
oop Definition483 = newDefinition(String480, Not482);

push((List*)getProperty(grammar, "definitions"), Definition483);

	 return grammar;
}


