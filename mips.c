#include "mips.h"
#include "gentac.h"

#include <stdio.h>

extern FRAME* frame_create();

extern void print_tac(TAC*);

static FILE* file;

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

char* get_cpu_reg() {
    return "$1";
}

void handle_assign(TAC* tac) {
    return;
}

void handle_add(TAC* tac) {
    int arg1 = tac->src1->value;
    int arg2 = tac->src2->value;

    char* reg1 = get_cpu_reg();
    char* reg2 = get_cpu_reg();

    fprintf(file, "li %s %d\n", reg1, arg1);
    fprintf(file, "li %s %d\n", reg2, arg2);
    fprintf(file, "add %s %s %s\n", get_cpu_reg(), reg1, reg2);
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


