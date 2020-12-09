#include "mips.h"
#include "gentac.h"

#include <stdio.h>
#include <stdlib.h>

extern FRAME* frame_create();

extern void print_tac(TAC*);
extern TOKEN* get_reg(char);

static FILE* file;
static TAC* current;

char* sprint_reg(TOKEN* tok) {
    char* reg = malloc(3*sizeof(char));
    if(tok->type == 't') sprintf(reg, "t%d", tok->value);
    if(tok->type == 'a') sprintf(reg, "a%d", tok->value);
    reg[2] = '\0';
    return reg;
}

int is_reg(TOKEN* t) {
    if(t->type == 't' || t->type == 'a') {
        return 1;
    }
    return 0;
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
        case IF_ENUM:
            handle_if(tac);
            break;
        case LABEL:
            handle_label(tac);
            break;
        case GOTO:
            handle_goto(tac);
            break;
        case SET:
            break;
        default:
            printf("%d: Could not handle TAC!\n", tac->op);
            break;
    }
}

void handle_assign(TAC* tac) {
    TOKEN* reg = tac->dst;
    int arg = tac->src1->value;
    fprintf(file, "\t# Handling variable storage\n");
    fprintf(file, "\tli $%s, %d\n", sprint_reg(reg), arg);
    return;
}

void handle_add(TAC* tac) {
    fprintf(file, "\t# Handling addition\n");
    int arg1 = tac->src1->value;
    int arg2 = tac->src2->value;

    char* reg1 = sprint_reg(get_reg('t'));
    char* reg2 = sprint_reg(get_reg('t'));

    if(is_reg(tac->src1)) {
        fprintf(file, "\tmove $%s, $%s\n", reg1, sprint_reg(tac->src1));
    } else {
        fprintf(file, "\tli $%s, %d\n", reg1, arg1);
    }

    if(is_reg(tac->src2)) {
        fprintf(file, "\tmove $%s, $%s\n", reg2, sprint_reg(tac->src2));
    } else {
        fprintf(file, "\tli $%s, %d\n", reg2, arg2);
    }

    fprintf(file, "\tadd $%s, $%s, $%s\n", sprint_reg(tac->dst), reg1, reg2);
    return;
}

void handle_sub(TAC* tac) {
    fprintf(file, "\t# Handling subtraction\n");
    int arg1 = tac->src1->value;
    int arg2 = tac->src2->value;

    char* reg1 = sprint_reg(get_reg('t'));
    char* reg2 = sprint_reg(get_reg('t'));

    if(is_reg(tac->src1)) {
        fprintf(file, "\tmove $%s, $%s\n", reg1, sprint_reg(tac->src1));
    } else {
        fprintf(file, "\tli $%s, %d\n", reg1, arg1);
    }

    if(is_reg(tac->src2)) {
        fprintf(file, "\tmove $%s, $%s\n", reg2, sprint_reg(tac->src2));
    } else {
        fprintf(file, "\tli $%s, %d\n", reg2, arg2);
    }

    fprintf(file, "\tsub $%s, $%s, $%s\n", sprint_reg(tac->dst), reg1, reg2);
    return;
}

void handle_mul(TAC* tac) {
    fprintf(file, "\t# Handling multiplication\n");
    int arg1 = tac->src1->value;
    int arg2 = tac->src2->value;

    char* reg1 = sprint_reg(get_reg('t'));
    char* reg2 = sprint_reg(get_reg('t'));

    if(is_reg(tac->src1)) {
        fprintf(file, "\tmove $%s, $%s\n", reg1, sprint_reg(tac->src1));
    } else {
        fprintf(file, "\tli $%s, %d\n", reg1, arg1);
    }

    if(is_reg(tac->src2)) {
        fprintf(file, "\tmove $%s, $%s\n", reg2, sprint_reg(tac->src2));
    } else {
        fprintf(file, "\tli $%s, %d\n", reg2, arg2);
    }
    fprintf(file, "\tmul $%s, $%s, $%s\n", sprint_reg(tac->dst), reg1, reg2);
    return;
}

void handle_div(TAC* tac) {
    fprintf(file, "\t# Handling division\n");
    int arg1 = tac->src1->value;
    int arg2 = tac->src2->value;

    char* reg1 = sprint_reg(get_reg('t'));
    char* reg2 = sprint_reg(get_reg('t'));

    if(is_reg(tac->src1)) {
        fprintf(file, "\tmove $%s, $%s\n", reg1, sprint_reg(tac->src1));
    } else {
        fprintf(file, "\tli $%s, %d\n", reg1, arg1);
    }

    if(is_reg(tac->src2)) {
        fprintf(file, "\tmove $%s, $%s\n", reg2, sprint_reg(tac->src2));
    } else {
        fprintf(file, "\tli $%s, %d\n", reg2, arg2);
    }

    fprintf(file, "\tdiv $%s, $%s, $%s\n", sprint_reg(tac->dst), reg1, reg2);
    return;
}

void handle_mod(TAC* tac) {
    fprintf(file, "\t# Handling mod\n");
    int arg1 = tac->src1->value;
    int arg2 = tac->src2->value;

    char* reg1 = sprint_reg(get_reg('t'));
    char* reg2 = sprint_reg(get_reg('t'));

    if(is_reg(tac->src1)) {
        fprintf(file, "\tmove $%s, $%s\n", reg1, sprint_reg(tac->src1));
    } else {
        fprintf(file, "\tli $%s, %d\n", reg1, arg1);
    }

    if(is_reg(tac->src2)) {
        fprintf(file, "\tmove $%s, $%s\n", reg2, sprint_reg(tac->src2));
    } else {
        fprintf(file, "\tli $%s, %d\n", reg2, arg2);
    }

    fprintf(file, "\trem $%s, $%s, $%s\n", sprint_reg(tac->dst), reg1, reg2);
    return;
}

void handle_call(TAC* tac) {
    return;
}

void handle_ret(TAC* tac) {
    fprintf(file, "\t# Handling return\n");
    int arg1 = tac->src1->value;
    if(tac->src1->type == 't' || tac->src1->type == 'a') {
        fprintf(file, "\tmove $a0, $%s\n\tli $v0, 1\n\tsyscall\n", sprint_reg(tac->src1));
    } else {
        fprintf(file, "\tli $a0, %d\n", arg1);
    }
    return;
}

void handle_if(TAC* tac) {
    TAC* test = tac->args.test;
    int op = test->op;

    switch(op) {
        case LESSER:
            {
                fprintf(file, "\t# Handling < condition\n");
                char* reg1;
                char* reg2;
                if(is_reg(test->src1)) {
                    reg1 = sprint_reg(test->src1);
                } else {
                    reg1 = sprint_reg(get_reg('t'));
                    fprintf(file, "\tli $%s, %d\n", reg1, test->src1->value);
                }

                fprintf(file, "\tbgt $%s, ", reg1);

                if(is_reg(test->src2)) {
                    fprintf(file, "$%s, ", sprint_reg(test->src2));
                } else {
                    fprintf(file, "%d, ", test->src2->value);
                }
                fprintf(file, "%s\n", tac->src2->lexeme);
            }
            break;
        default:
            break;
    }
    return;
}

void handle_label(TAC* tac) {
    fprintf(file, "%s:\n", tac->src2->lexeme);
}

void handle_goto(TAC* tac) {
    fprintf(file, "\tj %s\n", tac->src2->lexeme);
}

void handle_greater(TAC* tac) {
    return;
}

void handle_lesser(TAC* tac) {
    return;
}

void compile(TAC* program, char* filename) {
    file = fopen(filename, "w");
    fprintf(file, "# Standard program preamble\n");
    fprintf(file, "\t.globl main\n\n\t.text\n\nmain:\n\n");
    current = program;
    while(current != NULL) {
        handle(current);
        current = current->next;
    }
    fprintf(file, "\t# Standard program exit\n");
    fprintf(file, "\tli $v0, 10\n\tsyscall\n");
    puts("Compiled");
    fclose(file);
}


