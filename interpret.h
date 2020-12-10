#ifndef INTERPRET_H
#define INTERPRET_H

#include "nodes.h"
#include "C.tab.h"
#include "value.h"
#include "frame.h"

//TOKEN* TRUE = make_symbol(" TRUE ");
//TOKEN* FALSE = make_symbol(" FALSE ");


extern VALUE* run_interpret(NODE*, FRAME*);

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

static VALUE* lexical_call(VALUE*, NODE*, FRAME*);

static VALUE* interpret_apply(NODE*, FRAME*);

static void call_builtin(NODE*, FRAME*);

static VALUE* interpret_while(NODE*, FRAME*);

extern void cleanup_interpret(NODE*, FRAME*);
#endif
