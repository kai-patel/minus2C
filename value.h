#include "frame.h"

#ifndef VALUE_H
#define VALUE_H

typedef struct frame FRAME;

typedef struct closure {
    FRAME* frame;
    NODE* code;
    TOKEN* name;
    NODE* formals;
} CLOSURE;

typedef struct value {
    int type;
    int retval; //is it a value created by interpret_return?
    union {
        int integer;
        int boolean;
        char* string;
        CLOSURE* f;
    } v;
} VALUE;


VALUE* create_value(void);

void print_value(VALUE*);
#endif
