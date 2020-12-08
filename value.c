#include <stdlib.h>
#include <stdio.h>
#include "nodes.h"
#include "C.tab.h"
#include "value.h"

extern char* named(int);

VALUE* create_value(void) {
    VALUE* value = malloc(sizeof(VALUE));
    value->type = -1;
    return value;
}

void print_value(VALUE* val) {
    if(val == NULL) {
        printf("NULL value");
        return;
    }
    if(val->type == CONSTANT) {
        printf("%d", val->v.integer);
    } else if(val->type == STRING_LITERAL) {
        printf("%s", val->v.string);
    } else if(val->type == IDENTIFIER) {
        printf("%s", val->v.string);
    } else {
        printf("Closure \"%s\"", val->v.f->name->lexeme);
        //printf("%d not implemented yet", (val->type));
        return;
    }
}
