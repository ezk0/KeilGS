/**
	***************************************************************************
  * @file    	main.c 														
  * @author  	Janaina Kaufmann #2326875, Sahin Tekes	#2288847			
  *        	  	HAW-Hamburg													
  *          	Labor für technische Informatik								
  *          	Berliner Tor  7												
  *          	D-20099 Hamburg												
  * @version 	V1.0														
  * @date    	17.05.2018													
  * @brief   	Main program body of the LinkedList
  ***************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "linkedlist.h"


#include "TI_Lib.h"

/**
  * @brief  Main program
  * @param  None
  */
int main(void)
{	
	
	// ### Erprobungssequenz und Nachweise ###
	
	Init_TI_Board();
	
	char* pBew[] = {"Mittel", "Hoch", "Ausgezeichnet"};
	
	/* 1) Realisieren Sie die Eingabe von 8 Listenelementen
	 * mit geeigneten Parametern, wobei die Initialisierungen der Bewertung
	 * mit „Mittel“ und die der Summe der Klausurpunkte mit 0 erfolgen.
	 */
	
	// AddStudent(int p, char* name, int matrNr, tag ,monat ,jahr ,bewPkt , pBew)
	AddStudent(1, "Rogers, Steve G.", 23098, 8, 7, 2002, 0, pBew[0]);
	PrintStudents();
	AddStudent(1, "Stark, Tony", 29348, 2, 1, 2012, 0, pBew[0]);
	PrintStudents();
	AddStudent(0, "Odinson, Thor", 33094, 1, 3, 2003, 0, pBew[0]);
	PrintStudents();
	AddStudent(0, "Banner, Bruce", 20008, 12, 2, 2006, 0, pBew[0]);
	PrintStudents();
	AddStudent(1, "Romanoff, Natasha", 23000, 27, 5, 2007, 0, pBew[0]);
	PrintStudents();
	AddStudent(1, "Barton, Clint", 19098, 30, 9, 2014, 0, pBew[0]);
	PrintStudents();
	AddStudent(1, "Sohn von Laufey aus Jotunheim, Loki", 33033, 22, 10, 2009, 0, pBew[0]);
	PrintStudents();
	AddStudent(1, "Fury, Nick", 13548, 11, 4, 2010, 0, pBew[0]);
	PrintStudents();
	
	/*	2) RErgänzen Sie die Klausurpunkte für vier
	 *	Listenelemente mit Werten um 1800.
	 */
	
	// AddPkt(int matrNr, punkte)
	AddPkt(33094, 1800);
	PrintStudents();
	AddPkt(20008, 1800);
	PrintStudents();
	AddPkt(23098, 1800);
	PrintStudents();
	AddPkt(33033, 1800);
	PrintStudents();
	
	/* 3) Bringen Sie den Bewertungskommentar ein.
	 */
	setzeBew();
	PrintStudents();
	
	/* 4) Löschen Sie die Listenelemente, die keine 
	 * Klausurpunkte über 100 haben.
	 */
	delUnder100();
	PrintStudents();	
	
  return 0;

}
// EOF
