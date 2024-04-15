#include "grammar_objects.h"
#include "grammar_get_tree.h"

int main() {
    declareTypes();
    printTree(getGrammar());
    return 0;
}
