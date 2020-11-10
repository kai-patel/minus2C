
#ifndef FRAME_H
#define FRAME_H

#include "nodes.h"
#include "value.h"
#include "token.h"

typedef struct binding BINDING;

typedef struct frame {
    BINDING* bindings;
    struct frame* next;
} FRAME;

extern FRAME* frame_create();

VALUE* frame_check(TOKEN* x, FRAME* frame);
VALUE* frame_assign(TOKEN* x, FRAME* frame, VALUE* value);
VALUE* frame_declaration(TOKEN* x, FRAME* frame);
FRAME* frame_extend(FRAME*, NODE*, NODE*);
BINDING* make_binding(NODE*, VALUE*, BINDING*);
#endif
