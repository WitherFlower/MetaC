#include <unistd.h>

#include "grammar_objects.h"
#include "grammar_get_tree.h"
#include "vec.h"

DECLARE_VECTOR(StrVec, char);

int main() {
    oop grammar = getGrammar();

    StrVec *input = StrVec_new();
    
    char buf[1];
    while (read(0, buf, 1) > 0) {
        StrVec_push(input, buf[0]);
    }
    StrVec_push(input, '\0');
    
    evaluateTree(grammar, input->elements);
    return 0;
}
