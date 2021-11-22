#ifndef _VALUE
#define _VALUE

typedef enum {
    INT_TYPE, DOUBLE_TYPE, STR_TYPE, CONS_TYPE, NULL_TYPE, PTR_TYPE,
    OPEN_TYPE, CLOSE_TYPE, BOOL_TYPE, SYMBOL_TYPE, ERROR_TYPE, VOID_TYPE,
    CLOSURE_TYPE, PRIMITIVE_TYPE, UNSPECIFIED_TYPE,

    // Types below are only for bonus work (feel free to comment them out)
    OPENBRACKET_TYPE, CLOSEBRACKET_TYPE, DOT_TYPE, SINGLEQUOTE_TYPE
} valueType;

struct Value {
    valueType type;
    union {
        int i;
        double d;
        char *s;
        void *p;
        struct ConsCell {
            struct Value *car;
            struct Value *cdr;
        } c;
        struct Closure {
            struct Value *paramNames;
            struct Value *functionCode;
            struct Frame *frame;
        } cl;
        struct Value *(*pf)(struct Value *);
    };
};

typedef struct Value Value;

struct Frame {
    struct Frame *parent;
    Value *bindings;
};

typedef struct Frame Frame;

#endif
