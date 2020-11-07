#include "frame.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct binding {
    TOKEN* name;
    VALUE* val;
    BINDING* next;
} BINDING;

typedef struct frame {
    BINDING* bindings;
    FRAME* next;
} FRAME;

VALUE* frame_check(TOKEN* x, FRAME* frame) {
    while(frame != NULL) {
        BINDING* bindings = frame->bindings;
        while(bindings != NULL) {
            if(bindings->name == x) {
                return bindings->val;
            }
            bindings = bindings->next;
        }
        frame = frame->next;
    }
    //fprintf(stderr, "Unbound variable!\n");
    return NULL;
}

VALUE* frame_assign(TOKEN* x, FRAME* frame, VALUE* value) {
    while(frame != NULL) {
        BINDING* bindings = frame->bindings;
        while(bindings != NULL) {
            if(bindings->name == x) {
                bindings->val = value;
                return value;
            }
            bindings = bindings->next;
        }
        frame = frame->next;
    }
    fprintf(stderr, "Could not assign variable!\n");
}

VALUE* frame_declaration(TOKEN* x, FRAME* frame) {
    BINDING* bindings = frame->bindings ;
    BINDING* new = malloc(sizeof(BINDING));
        if(new != 0) { // check allocation worked
            new->name = x;
            new->val = (VALUE*) 0; // temporary
            new->next = bindings ;
            frame->bindings = new ;
            return (VALUE*) 0; // temporary
        }
    fprintf(stderr, "binding allocation failed\n");
}

FRAME* frame_create(void) {
    FRAME* frame = malloc(sizeof(FRAME));
    frame->bindings = NULL;
    frame->next = NULL;
}
