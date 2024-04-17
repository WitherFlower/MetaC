#include <stdio.h>
#include <unistd.h>

#include "grammar_objects.h"
#include "grammar_get_tree.h"
#include "vec.h"

DECLARE_VECTOR(StrVec, char)

int main() {
    declareTypes();
    oop grammar = getGrammar();

    StrVec *input = StrVec_new();
    
    char buf;

    while ((buf = getchar()) > 0) {
        StrVec_push(input, buf);
    }
    StrVec_push(input, '\0');
    
    evaluateTree(grammar, input->elements);
    return 0;
}
