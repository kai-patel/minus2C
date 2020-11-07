#include "value.h"
#include "frame.h"

#ifndef INTERPRET_H
#define INTERPRET_H
VALUE* interpret(NODE*, FRAME*);

static VALUE* interpret_punct(NODE*, FRAME*);

static VALUE* interpret_add(NODE*, FRAME*);

static VALUE* interpret_sub(NODE*, FRAME*);

static VALUE* interpret_equality(NODE*, FRAME*);

static VALUE* interpret_leaf(NODE*, FRAME*);

static VALUE* interpret_mul(NODE*, FRAME*);

static VALUE* interpret_div(NODE*, FRAME*);

static VALUE* interpret_mod(NODE*, FRAME*);

static VALUE* interpret_greater(NODE*, FRAME*);

static VALUE* interpret_lesser(NODE*, FRAME*);

static VALUE* interpret_if(NODE*, FRAME*);

static VALUE* interpret_assign(NODE*, FRAME*);
#endif
