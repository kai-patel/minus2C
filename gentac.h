#include "nodes.h"
#include "frame.h"
#include "value.h"

#ifndef GENTAC_H
#define GENTAC_H
enum ops {ADD, SUB, MUL, DIV, MOD, CALL_ENUM, RET, STORE, GREATER, LESSER};

typedef struct tac TAC;

typedef struct tac {
    enum ops op;
    TOKEN* src1, *src2, *dst;
    TAC* next;
} TAC;

typedef struct block BLOCK;

typedef struct call CALL;

typedef struct bb BB;

TAC* gen_tac(NODE*, FRAME*);

static TOKEN* get_reg(char);

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

extern TAC* run_gen_tac(NODE*, FRAME*);

extern void print_tac_program(TAC*);
#endif
