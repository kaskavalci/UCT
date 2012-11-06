//*******************************************************************
// Project: University Course Timetabling Yeditepe CSE
//
// File:       graphcse.cpp
// Date:       24/12/2010
// Author:     Ahmet Ulak
// Supervisor: Assoc. Prof. Dr. Emin Erkan Korkmaz
//             Computer Engineering Department
//             [+] ARTIficial Intelligence Laboratory
//             Yeditepe University
//             Istanbul, TURKEY
//
// Editted by: Ahmet Ulak
//             
// Description: This is the main file that controls the flow of 
//              the Graph Coloring program and manages input-output. 
//
// Last Modification: 24/12/2010
// ------------------------------------------------------------------
// Copyright (C) 2010 Yeditepe University
//                    Computer Engineering Department
//                    [+] ARTIficial Intelligence Laboratory
//                    http://cse.yeditepe.edu.tr/ARTI
//                    All rights reserved.
//*******************************************************************



INPUT FILES

	inputcse.txt:	Inputs related to courses,lecturers.
	inputparam.txt:	Parametric inputs.
	prereq.txt:	Prerequisite courses.

*******************   inputcse.txt *****************

	In this file there are two types of lines
1)phy101.01.2             p1      1       2
2)C2        phy101.01.2        0       0

Course Name - Lecturer - Semester # - Hours of Course
First line denotes Course name is given by lecturer p1 at semester 1 and course have 2 hours.

C2 (fixed attribute) -  Day - Timeslot
Second line denotes that lecture should be scheduled on day 0 and timeslot 0.

********************* inputparam.txt ***********

Parameters in this file is explained in ahmet_ulak_tez.pdf.
dur parameter defines the duration of the run in seconds. Value of 600 can give reasonable results.

mutaterate,simiter, distance, relax are parameters that are not used in the current version.

********************** prereq.txt **************

A sample line would be
cse101	cse112
This means that the course cse101 is prerequisite of cse112.


*********************  STEPS  TO RUN PROGRAM ****************

1)	First the courses, lecturers and predefined timeslots should be obtained from the faculty and inputcse.txt should be updated.
2)	Second the courses, lecturers and some predefined timeslots for guest lecturers should be obtained and inputcse.txt should be updated.
3)	prereq.txt file should be updated.
4)	inputparam.txt should be updated.
5)	The program should run as follows

	./a.out  5  inputparam.txt

Here instead of number 5 you can provide any seed.

6)	Program should be recompiled as follows in case there is a need,

	g++ graphcse.cpp

*******************     Outputs *************

The program prints the result on screen.
The following is the sample outputs.


********************  Lecturers *******************
OOOOO0000  OOOOO0000  buse       buse       OOOOO0000  OOOOO0000  OOOOO0000  OOOOO0000  OOOOO0000  buse      
OOOOO0000  OOOOO0000  buse       OOOOO0000  OOOOO0000  OOOOO0000  OOOOO0000  OOOOO0000  OOOOO0000  OOOOO0000 
OOOOO0000  OOOOO0000  OOOOO0000  OOOOO0000  OOOOO0000  OOOOO0000  OOOOO0000  buse       buse       OOOOO0000 
OOOOO0000  OOOOO0000  OOOOO0000  OOOOO0000  OOOOO0000  OOOOO0000  OOOOO0000  OOOOO0000  OOOOO0000  OOOOO0000 
OOOOO0000  OOOOO0000  OOOOO0000  OOOOO0000  OOOOO0000  OOOOO0000  OOOOO0000  OOOOO0000  OOOOO0000  OOOOO0000 


********************** Output Required by Faculty      *****************************


mb           cse101.01.2   cse101       1            F12-W5       
erkan        cse112.01.2   cse112       1            F67-R4       
onur         cse211.01.2   cse211       1            R12-W4       
mb           cse221.01.2   cse221       1            M67-RX       
esin         cse232.01.2   cse232       1            W67-T3       
gurhan       cse252.01.2   cse252       1            F89          
onur         cse311.01.2   cse311       1            F67-W5       
esin         cse323.01.2   cse323       1            M89-WX       
mehmed       cse321.01.2   cse321       1            R34          
dion         cse343.01.2   cse343       1            T12-T5       
sebnem       cse331.01.2   cse331       1            M67-T3       
erkan        cse354.01.2   cse354       1            M89-T5       
mehmed       cse421.01.2   cse421       1            R89          
birol        cse344.01.2   cse344       1            F67          
tacha        cse471.01.2   cse471       1            F89-FX       
mehmet       cse348.01.2   cse348       1            F89          
tacha        cse315.01.2   cse315       1            W12-T4       
esin         cse483.01.2   cse483       1            M12-W5       
birol        cse349.01.2   cse349       1            F34-M4       
erkan        cse462.01.2   cse462       1            T34-RX       
gurhan       cse427.01.2   cse427       1            R67-F5       
dion         cse487.01.2   cse487       1            M89-F3       
onur         cse211.02.2   cse211       2            W12-FX       
bilgin       cse101.L1.2   cse101       1            F89          
buse         cse112.L1.2   cse112       1            M34          
buse         cse112.L2.2   cse112       2            W89          
buse         cse112.L2.1   cse112       2            T3           
okan         cse211.L1.2   cse211       1            M34          
okan         cse211.L2.2   cse211       2            F67          
okan         cse211.L2.1   cse211       2            F5           
nazli        cse221.L1.2   cse221       1            M67          
nazli        cse221.L2.2   cse221       2            R67          
murat        cse232.L1.2   cse232       1            R89          
kursat       cse252.L1.2   cse252       1            W89          
kursat       cse252.L2.2   cse252       2            F67          
okan         cse311.L1.1   cse311       1            T5           
atasoy       cse321.L1.2   cse321       1            F89          
atasoy       cse321.L2.2   cse321       2            W34          
ahmet        cse343.L1.2   cse343       1            R67          
ahmet        cse343.L2.2   cse343       2            T67          
murat        cse331.L1.2   cse331       1            T67          
murat        cse331.L2.2   cse331       2            F67          
sebnem       cse492.L1.1   cse492       1            W3           
kursat       cse354.L1.1   cse354       1            M3           
atasoy       cse421.L1.2   cse421       1            T89          
atasoy       cse421.L2.2   cse421       2            M34          
korhan       cse344.L1.2   cse344       1            R67          
mine         cse471.L1.2   cse471       1            W67          
ahmet        cse348.L1.2   cse348       1            M67          

************************  TIMETABLE ************************

SEM  1  
       MON            TUE            WED            THR            FRI     
9  : phy101.01.2	eee211.01.2	mat221.01.2	chm101.03.2	cse101.01.2	
10 : phy101.01.2	eee211.01.2	mat221.01.2	chm101.03.2	cse101.01.2	
11 : phy101.03.2	mat151.01.2	phy102.01.2	mat241.01.2	xxxxxxxxxxx	
12 : phy101.03.2	mat151.01.2	phy102.01.2	mat241.01.2	xxxxxxxxxxx	
13 : xxxxxxxxxxx	xxxxxxxxxxx	cse101.01.1	cor107.01.1	xxxxxxxxxxx	
14 : chm101.01.2	xxxxxxxxxxx	ess162.01.2	xxxxxxxxxxx	xxxxxxxxxxx	
15 : chm101.01.2	xxxxxxxxxxx	ess162.01.2	xxxxxxxxxxx	xxxxxxxxxxx	
16 : hum101.01.2	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	exm100.01.2	
17 : hum101.01.2	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	exm100.01.2	
18 : xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	

SEM  2  
       MON            TUE            WED            THR            FRI     
9  : phy101.02.2	eee211.02.2	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	
10 : phy101.02.2	eee211.02.2	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	
11 : cse112.L1.2	cse112.L2.1	phy102.02.2	mat241.02.2	cor103.01.2	
12 : cse112.L1.2	xxxxxxxxxxx	phy102.02.2	mat241.02.2	cor103.01.2	
13 : xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	cor108.01.1	cor109.01.1	
14 : xxxxxxxxxxx	xxxxxxxxxxx	ess162.02.2	cor100.01.2	cse112.01.2	
15 : xxxxxxxxxxx	xxxxxxxxxxx	ess162.02.2	cor100.01.2	cse112.01.2	
16 : hum101.02.2	tkl201.01.2	tkl202.01.2	xxxxxxxxxxx	exm101.01.2	
17 : hum101.02.2	tkl201.01.2	tkl202.01.2	xxxxxxxxxxx	exm101.01.2	
18 : cse112.L1.1	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	

SEM  3  
       MON            TUE            WED            THR            FRI     
9  : xxxxxxxxxxx	eee211.03.2	mat221.02.2	cse211.01.2	xxxxxxxxxxx	
10 : xxxxxxxxxxx	eee211.03.2	mat221.02.2	cse211.01.2	xxxxxxxxxxx	
11 : phy101.04.2	mat151.02.2	xxxxxxxxxxx	mat241.03.2	xxxxxxxxxxx	
12 : phy101.04.2	mat151.02.2	cse211.01.1	mat241.03.2	xxxxxxxxxxx	
13 : cor111.01.1	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	cse211.L2.1	
14 : cse221.01.2	mat154.01.2	xxxxxxxxxxx	cse221.L2.2	cse211.L2.2	
15 : cse221.01.2	mat154.01.2	xxxxxxxxxxx	cse221.L2.2	cse211.L2.2	
16 : hum101.03.2	tkl201.02.2	xxxxxxxxxxx	xxxxxxxxxxx	exm102.01.2	
17 : hum101.03.2	tkl201.02.2	xxxxxxxxxxx	xxxxxxxxxxx	exm102.01.2	
18 : xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	cse221.01.1	cse211.02.1	

SEM  4  
       MON            TUE            WED            THR            FRI     
9  : xxxxxxxxxxx	xxxxxxxxxxx	mat221.03.2	xxxxxxxxxxx	xxxxxxxxxxx	
10 : xxxxxxxxxxx	xxxxxxxxxxx	mat221.03.2	xxxxxxxxxxx	xxxxxxxxxxx	
11 : phy101.05.2	cse232.01.1	xxxxxxxxxxx	mat241.04.2	xxxxxxxxxxx	
12 : phy101.05.2	xxxxxxxxxxx	xxxxxxxxxxx	mat241.04.2	xxxxxxxxxxx	
13 : xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	
14 : xxxxxxxxxxx	mat154.02.2	ess162.03.2	xxxxxxxxxxx	cse252.L2.2	
15 : xxxxxxxxxxx	mat154.02.2	ess162.03.2	xxxxxxxxxxx	cse252.L2.2	
16 : hum101.04.2	tkl201.03.2	cse252.L1.2	cor101.01.2	exm103.01.2	
17 : hum101.04.2	tkl201.03.2	cse252.L1.2	cor101.01.2	exm103.01.2	
18 : xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	

SEM  5  
       MON            TUE            WED            THR            FRI     
9  : xxxxxxxxxxx	cse343.01.2	mat221.04.2	xxxxxxxxxxx	xxxxxxxxxxx	
10 : xxxxxxxxxxx	cse343.01.2	mat221.04.2	xxxxxxxxxxx	xxxxxxxxxxx	
11 : xxxxxxxxxxx	xxxxxxxxxxx	cse321.L2.2	cse321.01.2	cor104.01.2	
12 : xxxxxxxxxxx	xxxxxxxxxxx	cse321.L2.2	cse321.01.2	cor104.01.2	
13 : xxxxxxxxxxx	cse343.01.1	cse311.01.1	xxxxxxxxxxx	xxxxxxxxxxx	
14 : xxxxxxxxxxx	cse343.L2.2	xxxxxxxxxxx	cse343.L1.2	cse311.01.2	
15 : xxxxxxxxxxx	cse343.L2.2	xxxxxxxxxxx	cse343.L1.2	cse311.01.2	
16 : hum101.05.2	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	cse321.L1.2	
17 : hum101.05.2	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	cse321.L1.2	
18 : xxxxxxxxxxx	xxxxxxxxxxx	cse323.01.1	xxxxxxxxxxx	xxxxxxxxxxx	

SEM  6  
       MON            TUE            WED            THR            FRI     
9  : xxxxxxxxxxx	xxxxxxxxxxx	mat221.05.2	xxxxxxxxxxx	xxxxxxxxxxx	
10 : xxxxxxxxxxx	xxxxxxxxxxx	mat221.05.2	xxxxxxxxxxx	xxxxxxxxxxx	
11 : cse354.L1.1	cse331.01.1	xxxxxxxxxxx	xxxxxxxxxxx	cor105.01.2	
12 : cse421.L2.2	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	cor105.01.2	
13 : xxxxxxxxxxx	cse354.01.1	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	
14 : cse331.01.2	cse331.L1.2	xxxxxxxxxxx	xxxxxxxxxxx	cse331.L2.2	
15 : cse331.01.2	cse331.L1.2	xxxxxxxxxxx	xxxxxxxxxxx	cse331.L2.2	
16 : hum101.06.2	cse421.L1.2	xxxxxxxxxxx	cse421.01.2	cse348.01.2	
17 : hum101.06.2	cse421.L1.2	xxxxxxxxxxx	cse421.01.2	cse348.01.2	
18 : xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	

SEM  7  
       MON            TUE            WED            THR            FRI     
9  : xxxxxxxxxxx	xxxxxxxxxxx	cse315.01.2	xxxxxxxxxxx	xxxxxxxxxxx	
10 : xxxxxxxxxxx	xxxxxxxxxxx	cse315.01.2	xxxxxxxxxxx	xxxxxxxxxxx	
11 : xxxxxxxxxxx	xxxxxxxxxxx	cse492.L1.1	mat241.05.2	cse349.01.2	
12 : cse349.01.1	cse315.01.1	xxxxxxxxxxx	mat241.05.2	cse349.01.2	
13 : xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	
14 : chm101.02.2	xxxxxxxxxxx	cse471.L1.2	cse344.L1.2	cse344.01.2	
15 : chm101.02.2	xxxxxxxxxxx	cse471.L1.2	cse344.L1.2	cse344.01.2	
16 : xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	cse471.01.2	
17 : xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	cse471.01.2	
18 : xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	cse471.01.1	

SEM  8  
       MON            TUE            WED            THR            FRI     
9  : cse483.01.2	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	cor102.01.2	
10 : cse483.01.2	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	cor102.01.2	
11 : xxxxxxxxxxx	cse462.01.2	phy102.03.2	xxxxxxxxxxx	cor106.01.2	
12 : xxxxxxxxxxx	cse462.01.2	phy102.03.2	xxxxxxxxxxx	cor106.01.2	
13 : xxxxxxxxxxx	xxxxxxxxxxx	cse483.01.1	xxxxxxxxxxx	cse427.01.1	
14 : xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	cse427.01.2	xxxxxxxxxxx	
15 : xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	cse427.01.2	xxxxxxxxxxx	
16 : cse487.01.2	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	
17 : cse487.01.2	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	
18 : xxxxxxxxxxx	xxxxxxxxxxx	xxxxxxxxxxx	cse462.01.1	xxxxxxxxxxx	


*****************************   HARD & SOFT CONFLICTS *********************************

Hard  1 Confmat  phy101.04.2 cse211.L1.2 -1 
Hard  1 Confmat  hum101.05.2 cse323.01.2 -1 
Hard  1 Confmat  hum101.06.2 cse354.01.2 -1 
Hard  1 Confmat  mat241.02.2 cse112.01.1 -1 
Hard  3 Hours 11-13    mat241.02.2 cse112.01.1 -1 
Hard  1 Confmat  mat221.02.2 cse211.02.2 -1 
Hard  1 Confmat  ess162.03.2 cse232.01.2 -1 
Hard  1 Confmat  tkl202.01.2 cse112.L2.2 -1 
Hard  1 Confmat  cor101.01.2 cse232.L1.2 -1 
Hard  1 Confmat  cor106.01.2 cse487.01.1 -1 
Hard  3 Hours 11-13    cor106.01.2 cse487.01.1 -1 
Hard  1 Confmat  exm100.01.2 cse101.L1.2 -1 
Hard  1 Confmat  exm103.01.2 cse252.01.2 -1 
Hard  1 Confmat  cor111.01.1 cse211.L1.1 -1 
Hard  1 Confmat  cse221.01.2 cse221.L1.2 -1 
Hard  2 Same Day  cse343.01.1 cse343.01.2 -1 
Hard  1 Confmat  cse343.01.1 cse311.L1.1 -1 
Hard  1 Confmat  cse331.01.2 cse348.L1.2 -1 
Hard  2 Same Day  cse471.01.1 cse471.01.2 -1 
Hard  1 Confmat  cse354.L1.1 cse421.L2.2 -1 
Hard  3 Hours 11-13    cse354.L1.1 cse421.L2.2 -1 
Soft  15 Departmental Meeting      cse427.01.2 cse427.01.2 -1079904756 
Soft  15 Departmental Meeting      cse221.L2.2 cse221.L2.2 -1079904756 
Soft  15 Departmental Meeting      cse343.L1.2 cse343.L1.2 -1079904756 
Soft  15 Departmental Meeting      cse344.L1.2 cse344.L1.2 -1079904756 
Soft  8 Lecturer 4 cons Empty Hour      erkan   -1079904756 
Soft  12 Lecturer More than 2 morning courses       onur   -1079904756 
Soft  8 Lecturer 4 cons Empty Hour      esin   -1079904756 
Soft  8 Lecturer 4 cons Empty Hour      buse   -1079904756 
Soft1  11 L T L conflict         cse211     3 
Soft1  11 L T L conflict         cse211     3 
Soft1  11 L T L conflict         cse221     4 
Soft1  11 L T L conflict         cse321     8 
Soft2  4 Prev Sem Next Sem     cse354.01.1 cse343.01.1 -1079905088 
Soft2  4 Prev Sem Next Sem     cse354.01.2 cse323.01.2 -1079905088 
Soft2  4 Prev Sem Next Sem     cse471.01.2 cse348.01.2 -1079905088 
Soft2  4 Prev Sem Next Sem     cse349.01.1 cse421.L2.2 -1079905088 
Soft2  4 Prev Sem Next Sem     cse462.01.2 cse315.01.1 -1079905088 
Soft2  4 Prev Sem Next Sem     cse487.01.1 cse349.01.2 -1079905088 
Soft2  4 Prev Sem Next Sem   LABS  cse311 cse354 -1079905088 
Soft2  4 Prev Sem Next Sem   LABS  cse344 cse427 -1079905088 
Soft3  14 No Lunch Hour  conflict            2 
Soft3  14 No Lunch Hour  conflict            16 
Soft3  14 No Lunch Hour  conflict            34 
Soft3  14 No Lunch Hour  conflict            58 
Soft3  14 No Lunch Hour  conflict            60 
Soft3  5 Evening Lecture     cse211.02.1   60 
Soft3  5 Evening Lecture     cse221.01.1   60 
Soft3  5 Evening Lecture     cse323.01.1   60 
Soft3  5 Evening Lecture     cse471.01.1   60 
Soft3  5 Evening Lecture     cse462.01.1   60 
Soft3  5 Evening Lecture     cse112.L1.1   60 

