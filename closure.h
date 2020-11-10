#include "frame.h"
#include "nodes.h"

#ifndef CLOSURE_H
#define CLOSURE_H
typedef struct closure {
    FRAME* frame;
    NODE* code;
} CLOSURE;
#endif
