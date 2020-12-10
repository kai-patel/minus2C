#include "nodes.h"
#include "frame.h"
#include "value.h"

#ifndef GENTAC_H
#define GENTAC_H
enum ops {ADD, SUB, MUL, DIV, MOD, CALL_ENUM, RET, STORE, GREATER, LESSER, LABEL, FUNC, END_FUNC, IF_ENUM, GOTO, SET};

typedef struct tac TAC;

typedef struct proc {
    TOKEN* name;
    int arity;
} PROC;

typedef struct block {
    int nvars;
} BLOCK;

typedef struct call {
    TOKEN* name;
    int arity;
} CALL;

typedef struct tac {
    enum ops op;
    union { BLOCK block; CALL call; PROC proc; TAC* test; } args;
    TOKEN* src1, *src2, *dst;
    TAC* next;
} TAC;

typedef struct block BLOCK;

typedef struct call CALL;

typedef struct bb {
    TAC* leader;
} BB;

TAC* gen_tac(NODE*, FRAME*);

extern TOKEN* get_reg(char);

TOKEN* get_label();

static void add_tac(TAC*);

static TAC* create_tac();

static void print_reg(TOKEN*);

static void print_token(TOKEN*);

static TAC* leaf_to_token(NODE*, FRAME*);

static TAC* gen_tac_return(NODE*, FRAME*);

extern void print_tac(TAC*);

static TAC* gen_tac_punct(NODE*, FRAME*);

static TAC* gen_tac_add(NODE*, FRAME*);

static TAC* gen_tac_sub(NODE*, FRAME*);

static TAC* gen_tac_equality(NODE*, FRAME*);

static TAC* gen_tac_leaf(NODE*, FRAME*);

static TAC* gen_tac_mul(NODE*, FRAME*);

static TAC* gen_tac_div(NODE*, FRAME*);

static TAC* gen_tac_mod(NODE*, FRAME*);

static TAC* gen_tac_greater(NODE*, FRAME*);

static TAC* gen_tac_lesser(NODE*, FRAME*);

static TAC* gen_tac_if(NODE*, FRAME*);

static TAC* gen_tac_call(TOKEN*, NODE*, FRAME*);

static TAC* gen_tac_apply(NODE*, FRAME*);

extern TAC* run_gen_tac(NODE*, FRAME*);

extern void print_tac_program(TAC*);

extern void cleanup_gen_tac(TAC*);
#endif
