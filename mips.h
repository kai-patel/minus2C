#include "gentac.h"

#ifndef MIPS_H
#define MIPS_H
extern void compile(TAC*, char*);

static char* sprint_reg(TOKEN*);

static int is_reg(TOKEN*);

static void handle(TAC*);

static void handle_assign(TAC*);

static void handle_maths(TAC*);

static void handle_add(TAC*);

static void handle_sub(TAC*);

static void handle_mul(TAC*);

static void handle_div(TAC*);

static void handle_mod(TAC*);

static void handle_call(TAC*);

static void handle_ret(TAC*);

static void handle_greater(TAC*);

static void handle_lesser(TAC*);
#endif
