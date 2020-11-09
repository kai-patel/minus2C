#include "mips.h"
#include "gentac.h"

#include <stdio.h>
#include <stdlib.h>

extern FRAME* frame_create();

extern void print_tac(TAC*);
extern TOKEN* get_reg(char);

static FILE* file;

char* sprint_reg(TOKEN* tok) {
    char* reg = malloc(3*sizeof(char));
    if(tok->type == 't') sprintf(reg, "t%d", tok->value);
    if(tok->type == 'a') sprintf(reg, "a%d", tok->value);
    reg[2] = '\0';
    return reg;
}

void handle(TAC* tac) {
    switch(tac->op) {
        case ADD:
            handle_add(tac);
            break;
        case SUB:
            handle_sub(tac);
            break;
        case MUL:
            handle_mul(tac);
            break;
        case DIV:
            handle_div(tac);
            break;
        case MOD:
            handle_mod(tac);
            break;
        case CALL_ENUM:
            handle_call(tac);
            break;
        case RET:
            handle_ret(tac);
            break;
        case STORE:
            handle_assign(tac);
            break;
        case GREATER:
            handle_greater(tac);
            break;
        case LESSER:
            handle_lesser(tac);
            break;
        default:
            puts("Could not handle TAC!");
            break;
    }
}

void handle_assign(TAC* tac) {
    return;
}

void handle_add(TAC* tac) {
    int arg1 = tac->src1->value;
    int arg2 = tac->src2->value;

    char* reg1 = sprint_reg(get_reg('t'));
    char* reg2 = sprint_reg(get_reg('t'));

    fprintf(file, "li %s %d\n", reg1, arg1);
    fprintf(file, "li %s %d\n", reg2, arg2);
    fprintf(file, "add %s %s %s\n", sprint_reg(tac->dst), reg1, reg2);
    return;
}

void handle_sub(TAC* tac) {
    return;
}

void handle_mul(TAC* tac) {
    return;
}

void handle_div(TAC* tac) {
    return;
}

void handle_mod(TAC* tac) {
    return;
}

void handle_call(TAC* tac) {
    return;
}

void handle_ret(TAC* tac) {
    return;
}

void handle_greater(TAC* tac) {
    return;
}

void handle_lesser(TAC* tac) {
    return;
}

void compile(TAC* program, char* filename) {
    file = fopen(filename, "w");
    TAC* current = program;
    while(current != NULL) {
        handle(current);
        current = current->next;
    }
    puts("Compiled");
    fclose(file);
}


