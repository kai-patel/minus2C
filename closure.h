#ifndef CLOSURE_H
#define CLOSURE_H

#include "frame.h"
#include "nodes.h"

typedef struct closure {
    FRAME* frame;
    NODE* code;
} CLOSURE;
#endif
