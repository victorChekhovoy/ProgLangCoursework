#ifndef _LOOKUP
#define _LOOKUP

Value *lookUpSymbol(Value *tree, Frame *frame);

Frame *replaceSymbol(Value *lookUp, Value *replace, Frame *frame);

#endif
