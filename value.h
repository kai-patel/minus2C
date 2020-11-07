#ifndef VALUE_H
#define VALUE_H 1
typedef struct value {
    int type;
    union {
        int integer;
        int boolean;
        char* string;
    } v;
} VALUE;

VALUE* create_value(void);

void print_value(VALUE*);
#endif
