#include "value.h"
#include "token.h"

#ifndef FRAME_H
#define FRAME_H
typedef struct binding BINDING;

typedef struct frame FRAME;

extern FRAME* frame_create();

VALUE* frame_check(TOKEN* x, FRAME* frame);
VALUE* frame_assign(TOKEN* x, FRAME* frame, VALUE* value);
VALUE* frame_declaration(TOKEN* x, FRAME* frame);
#endif
