#ifndef	_DIGITIZER_API_H
#define	_DIGITIZER_API_H

#include "digitizer_comm.h"
#include "scan.h"

/*
Input:
Value1: adc value (ANT-1)
Value2: max adc value (ANT)
Value3: adc value (ANT+1)
Value3: max adc value (ANT Num)
PenScale :  PEN Scale
Output:
Ret: freq Value

*/
unsigned int PosCalcP_Lib(unsigned int value1,unsigned int value2, unsigned int value3, unsigned char value4, unsigned int lpi);


/*
Input:
Value1: adc value (ANT-1)
Value2: max adc value (ANT)
Value3: adc value (ANT+1)
Value3: max adc value (ANT Num)
LPI:   Line per ANT
Output:
ret: X,Y position
*/
//unsigned int PosCalc2_Lib(unsigned int value1,unsigned int value2, unsigned int value3, unsigned char value4, unsigned int lpi);
unsigned int PosCalcCoordinate_3Line_Lib(unsigned int value1,unsigned int value2, unsigned int value3, unsigned int value4, unsigned int value5, unsigned char value6, unsigned int lpi);
unsigned int PosCalcCoordinate_4Line_Lib(unsigned int value1,unsigned int value2, unsigned int value3, unsigned int value4, unsigned int value5, unsigned char value6, unsigned int lpi);
unsigned int PosCalcCoordinate_3And4Line_Lib(unsigned int value1,unsigned int value2, unsigned int value3, unsigned int value4, unsigned int value5, unsigned char value6, unsigned int lpi);

/*
Input:
sourceX: X or Y position
correction: Calibration table
index: Calibration position index
Value3: max adc value (ANT Num)
pixel:  dot pitch
Output:
ret: X,Y position
*/

unsigned int CalibrationXY(unsigned int sourceX,const unsigned int* correction,unsigned short int index,unsigned char pixel);


/*
Input:
sourceX: X or Y position struct
*result: X,Y position struct
output:
Ret:None
*/

void Pos_smooth_Lib(def_pos_calc *source , def_report_pos *result);
void Pos_smooth_new_Lib(def_pos_calc *source , def_report_pos *result,SMOOTH_PARAMETER *Smooth);

/*
Input:
def_antanaX: X max adc Value serven ant struct
def_antanaY: Y max adc Value serven ant struct
Tilt_Parameter: angle parameter
*x_deg:
*y_deg:

result: X,Y position struct
output:
Ret:None
*/

void Tilt_LIB_B(ANTANA_DATA_STRUCT *, ANTANA_DATA_STRUCT  *, unsigned int *, \
                unsigned char , unsigned char , unsigned char , unsigned char , \
                unsigned char , unsigned char , TILT_PARAMETER *,\
                char *, char *,unsigned short *,unsigned short *);


void Tilt_LIB_D(ANTANA_DATA_STRUCT *X_value, ANTANA_DATA_STRUCT  *Y_value, \
							  unsigned char gAntXCnt, unsigned char gAntYCnt, \
							  unsigned char ALL_X_ANTANA, unsigned char ALL_Y_ANTANA, \
							  char *x_deg, char *y_deg);



#endif

