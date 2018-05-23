/**
	***************************************************************************
  * @file    	linkedlist.c 														
  * @author  	Janaina Kaufmann #2326875, Sahin Tekes	#2288847			
  *        	  	HAW-Hamburg													
  *          	Labor für technische Informatik								
  *          	Berliner Tor  7												
  *          	D-20099 Hamburg												
  * @version 	V1.0														
  * @date    	17.05.2018													
  * @brief   	LinkedList implementation
  ***************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "linkedlist.h"

struct Student *LIST = NULL;

// Studenten Daten aufnehmen
void
AddStudent (int p, char *Name, int MatrNr, int Etag, int Emon, int Ejahr, int B_P, char *pBew) {
	printf("Adding... MatrNr: %d...\n", MatrNr);
	struct Student * NewStudent = NULL;
	
	NewStudent = (struct Student *) (malloc(sizeof(struct Student)));
	if (NewStudent == NULL) {
		errno = ENOMEM;
		perror("Fail to allocate memory!!");
		return;
	}
	
    // Array überschreiben nur mit strcpy(dest, source)
	strcpy(NewStudent->Name, Name);
	NewStudent->MatrNr = MatrNr;
	NewStudent->Einschreibung.Tag = Etag;
	NewStudent->Einschreibung.Monat = Emon;
	NewStudent->Einschreibung.Jahr = Ejahr;
	NewStudent->Bew_Pkt = B_P;
	NewStudent->pBew = pBew;
	
	if (p == 0 || LIST == NULL) {
		NewStudent->next = LIST;
		LIST = NewStudent;
	
	} else if (p == 1){
		WHILENEXT(LIST, curr) {
			curr = curr->next;
		}
		curr->next = NewStudent;
		NewStudent->next = NULL;
	}
	
}

// lösche Student-Element mit entsprechender Matrikelnummer
void
DelStudent(int MatrNr) {
	printf("Deleting MatrNr: %d...\n", MatrNr);
	struct Student *del = NULL;
	
	if (LIST == NULL) {
		errno = ERANGE;
		perror("List is empty!!");
		return;
	} else if (LIST->MatrNr == MatrNr){
		del = LIST;
		LIST = LIST->next;
		free(del);
		return;
	}
	
	WHILENEXT(LIST, test) {
		if (test->next->MatrNr == MatrNr) {
			break;
		}
		test = test->next;
	}

	if (test->next == NULL) {
		errno = EINVAL;
		perror("Nicht Vorhanden!!");
		return;
	}

	del = test->next;
	test->next = test->next->next;
	
	free(del);
}

// Drucke Listing aller Studenten-Elemente
void
PrintStudents() {
	printf("Printing...\n");
	WHILE(LIST, curr){
		printf("| Name: %s MatrNr.: %d\
			  \n| Einschreibung: %d/%d/%d\
			  \n| Bew_Pkt.: %d pBew.: %s\n\n" , curr->Name, curr->MatrNr\
																				, curr->Einschreibung.Tag, curr->Einschreibung.Monat, curr->Einschreibung.Jahr\
																				, curr->Bew_Pkt, curr->pBew);
		curr = curr->next;
	}
	printf("END\n\n");
}

// füge einem Studenten-Element eine Punktzahl hinzu
void
AddPkt(int MatrNr, int BewPkt) {
	printf("Adding Pkt MatrNr: %d...\n", MatrNr);
	WHILE(LIST, test) {
		if (test->MatrNr == MatrNr) {
			break;
		}
		test = test->next;
	}

	test->Bew_Pkt += BewPkt;
}


// ermittle 'besten' Studenten
int
BewStudent(char *pBew) {
	printf("BewStudent...\n");
	struct Student * bester = LIST;
	
//	if (strcmp(pBew), "Mittel") {
//	
//	}

	WHILE(LIST, curr) {
		if (curr->Bew_Pkt > 1800) {
			curr->pBew = pBew;
		}
		if (curr->Bew_Pkt > bester->Bew_Pkt) {
			bester = curr;
		}
		curr = curr->next;
	}
	
	if (bester->Bew_Pkt > 1800){
		return bester->MatrNr;
	} else {
		return -1;
	}
}

// setze Bewertungskommentare für alle Studenten
void
setzeBew () {
	printf("setzeBew...\n");
	if (LIST == NULL) {
		return;
	}
	
	WHILE(LIST, test) {
		if (test->Bew_Pkt > 1800) {
			test->pBew = "Ausgezeichnet";
		} else if (test->Bew_Pkt < 100) {
			test->pBew = "Mittel";
		} else {
			test->pBew = "Hoch";
		}
		test = test->next;
	}
}

// lösche alle Studenten mit einer Punktzahl weniger als 100
void
delUnder100 () {
	printf("delUnder100...\n");
	if (LIST == NULL) {
		return;
	}
	
	WHILE(LIST, test) {
		if (test->Bew_Pkt < 100) {
			DelStudent(test->MatrNr);
		}
		test = test->next;
	}
}
