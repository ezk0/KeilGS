#ifndef WINKEL_H
#define	WINKEL_H

typedef
enum{
	A_P = 0,
	B_P = 2,
	C_P = 3,
	D_P = 1
} State_Type;


void a_phase(void);
void b_phase(void);
void c_phase(void);
void d_phase(void);

void printLEDsteps(void);

extern void encode (void);

#endif /* WINKEL_H */
