#ifndef __DL212_H
#define __DL212_H

extern const char *TestProgram;
extern const char *Test_Cmd;
extern const char *Test_DataTable;
extern const char *Test_If;

extern unsigned int C1_Dest,C2_Dest,P_SW_Dest,F_Mea_Dest;
extern unsigned int C1_Time,C2_Time,P_SW_Time,F_Mea_Time; 
extern unsigned char C1_Option,C2_Option,P_SW_Option,F_Mea_Option;

#define TASK_NUM_MAX  4
#define TASK_CMD_NUM_MAX 256
#define SUB_NUM_MAX  9

#define TABLE_NUM_MAX 5
#define PUBLIC_NUM_MAX 256
#define DIM_NUM_MAX 64

struct _NUM{
  unsigned char table;
	unsigned char task;
	unsigned char task_cmd_num[TASK_NUM_MAX];
	unsigned char sub; 
	unsigned char array[5];
	unsigned char var[5]; 
};
extern struct _NUM sNUM;

struct _DIM_NUM{
  unsigned short int array;
  unsigned short int array_len[DIM_NUM_MAX];
  unsigned short int var;
};
extern struct _DIM_NUM sDIM_NUM;

struct _TABLE_PARA{
  unsigned short int dest[6];
	unsigned short int reps[6];
};
extern struct _TABLE_PARA sTABLE_PARA[];

struct _PULSE_COUNT{
  unsigned int SE[2];
	unsigned int C1[2];
	unsigned int C2[2];
	unsigned int P_SW[2];
	
};
extern struct _PULSE_COUNT sPULSE_COUNT;

char Program_Execute(unsigned int time_1970); 
char Command_Parse(char *cmd);
char Lexical_Analyzer(char *program);
char CMD_Pointer_Init(void);
   
unsigned int Func_DataTable(char *cmd); 
unsigned int Func_CallSub(char *cmd);

void Battery(double *para); 
void VGet(double *para);  
void PulseCount(double *para); 
void ExciteV(double *para);
void SDI12(double *para); 

void SetFlagHigh(double *para);
void SetFlagLow(double *para); 
 
float SquareRootFloat(float number);
 
extern unsigned char Program_Parse(char *program);
extern const char *DimDef_Name[DIM_NUM_MAX];
extern unsigned short int DimDef_Array_Len[DIM_NUM_MAX];
extern float Var[];

#endif
