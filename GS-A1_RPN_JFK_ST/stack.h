#ifndef STACK_H
#define	STACK_H

#define MAXSIZE 		8

extern int stackTop(void);
extern int pop(void);
extern int clearStack(void);
extern int push (void);
extern void copyStack (int * cStack, int * tStack);
extern void swapPosition (void);
extern void duplicate (void);

#endif	/* STACK_H */
