leg -o grammar.c grammar.leg
cc -o grammar grammar.c grammar_objects.c -g
(./grammar < grammar.leg) > thing

cc -o test_get_tree test_get_tree.c grammar_objects.c grammar_get_tree.c
./test_get_tree > thing2

diff thing thing2

