#include "DL212.h"
#include "string.h" 
#include "stdlib.h"
#include "stdio.h"
#include "Datalogger_Protocols.h"
#include "arithmetic_expression.h"
#include "hal.h"
#include "hw.h"
#include "delay.h"
#include "stm32l1xx_it.h"
#include "ads1248.h"

struct _NUM sNUM; 
struct _TABLE_PARA sTABLE_PARA[4]; 
float Var[PUBLIC_NUM_MAX+DIM_NUM_MAX]={0}; 
float Var_Avg[PUBLIC_NUM_MAX+DIM_NUM_MAX]; 
float Var_Max[PUBLIC_NUM_MAX+DIM_NUM_MAX]; 
float Var_Min[PUBLIC_NUM_MAX+DIM_NUM_MAX]; 
float Var_MaxMin[PUBLIC_NUM_MAX+DIM_NUM_MAX]; 

 
const unsigned char CPN[100]={ 
0,1,0,6,0,6,0,3,3,6,//0-9
0,0,0,0,0,0,0,0,0,0,//10-19
0,0,0,0,0,0,0,0,0,0,//20-29
2,2,2,2,0,0,0,0,0,0,//30-39
0,0,0,0,0,0,0,0,0,0,//40-49
4,3,3,3,3,3,3,0,0,0,//50-59
0,0,0,0,0,0,0,0,0,0,//60-69
2,2,1,3,2,3,3,0,1,0,//70-79
0,0,0,0,0,0,0,0,0,0,//80-89
0,0,0,0,0,0,0,0,0,0,//90-99	
};

void (*Function[100])(double *para)={	
0,Battery,VGet,PulseCount,ExciteV,SDI12,0,0,0,0,//0-9
0,0,0,0,0,0,0,0,0,0,//10-19
0,0,0,0,0,0,0,0,0,0,//20-29
0,0,0,0,0,0,0,0,0,0,//30-39
0,0,0,0,0,0,0,0,0,0,//40-49
0,0,0,0,0,0,0,0,0,0,//50-59
0,0,0,0,0,0,0,0,0,0,//60-69
SetFlagHigh,SetFlagLow,0,0,0,0,0,0,0,0,//70-79
0,0,0,0,0,0,0,0,0,0//80-89
};

const char *TestProgram=
"DL212\0Public\0Table_1\0Table_2\0;\
Bat[001]\0Pulse[002]\0Volt[004]\0;\
i[002]\0;\
Bat_SMP[001]\0Pulse_SMP[002]\0Volt_SMP[004]\0;\
Bat_AVG[001]\0Pulse_AVG[002]\0Volt_AVG[004]\0;\
Scan1,3,1\0\
c73,0,10,1\0\
{\0\
c1,1\0\
&1=2^(-1)*1.999*&1\0\
}\0\
c1,1\0\
c75,&1,2,<=\0\
{\0\
c1,1\0\
}\0\
c77\0\
{\0\
&1=12.1\0\
}\0\
c75,&2,2,=\0\
{\0\
&2=-1*&2\0\
}\0\
c77\0\
{\0\
&2=&2^2.2\0\
}\0\
;\
@\
c78,1,c5,2,2,1,0,0,100\0\
c78,2,c3,4,4,1,1,0,0\0\
$";
/*Table Definition*/
const char *TableDef_Name[TABLE_NUM_MAX]; 
unsigned int TableDef_Interval[TABLE_NUM_MAX]; 
const char *TableDef_Var_Def[TABLE_NUM_MAX][PUBLIC_NUM_MAX]; 
unsigned char TableDef_Var_Array_Len[TABLE_NUM_MAX][PUBLIC_NUM_MAX]; 
/*Dim*/ 
const char *DimDef_Name[DIM_NUM_MAX]; 
struct _DIM_NUM sDIM_NUM; 
/*task*/
const char *Task_Cmd[TASK_NUM_MAX][TASK_CMD_NUM_MAX]; 
unsigned int Task_Interval[TASK_NUM_MAX];
/*sub*/
const char *Sub_Cmd[SUB_NUM_MAX];  


void DataSave(unsigned int time_1970){
	unsigned char i,j;
	
  for(i=0;i<sNUM.table;i++){  
  	if(0 == (time_1970+sTable_Info[i].time_intoint/1000)%(sTable_Info[i].time_interval/1000)){
			for(j=0;j<6;j++){
			  if(sTABLE_PARA[i].dest[j]){
				
				}
			}
	  }
  }
}
 
float For_Cycle_Para[3]={0};
char Program_Execute(unsigned int time_1970){
  unsigned int i=0,j=0,k=0,f=0;
  char *cmd,if_result=0;
	
	for(i=0;i<sNUM.task;i++){ 
  	//if(0 == time_1970%(Task_Interval[i]/1000)){ 
		  for(j=1;j<sNUM.task_cmd_num[i];j++){//pass掉Scan
		    cmd = (char*)Task_Cmd[i][j];  
				if('&' == *(cmd+i)){//算术表达式 
				  DL212_Exp_Calculate((double*)Var,(char*)(cmd)); 
				}
				else if('c' == *cmd){ 
				  if_result = Command_Parse(cmd); 
					if(0 != For_Cycle_Para[2]){//cmd是for循环指令
						for(f=For_Cycle_Para[0];f<For_Cycle_Para[1];f+=For_Cycle_Para[2]){
						  k=0,cmd = (char*)Task_Cmd[i][j+(++k)]; 
	  					if('{' == *cmd){ 
		  				  while('}' != *cmd){ 
			  			    cmd = (char*)Task_Cmd[i][j+(++k)]; 
									if('&' == *(cmd+i)){ 
										DL212_Exp_Calculate((double*)Var,(char*)(cmd)); 
									} 
									else if('c' == *cmd){ 
									  Command_Parse(cmd); 
									} 
				  		  } 
					  	} 
						} 
						j += k; 
						For_Cycle_Para[0]=For_Cycle_Para[1]=For_Cycle_Para[2]=0; 
					} 
					if(1 == if_result){//cmd是if指令,结果为真
					  if_result = 0;
					  cmd = (char*)Task_Cmd[i][++j]; 
	  				if('{' == *cmd){ 
		  			  while('}' != *cmd){ 
			  		    cmd = (char*)Task_Cmd[i][++j]; 
								if('&' == *(cmd+i)){ 
									DL212_Exp_Calculate((double*)Var,(char*)(cmd)); 
								} 
								else if('c' == *cmd){ 
								  Command_Parse(cmd); 
								} 
				  	  }  
				  	}
						cmd = (char*)Task_Cmd[i][++j]; 
						if(0 == strcmp(cmd,"c77")){//如果有else,跳过else{}的指令
							cmd = (char*)Task_Cmd[i][++j]; 
				      if('{' == *cmd){ 
		  		      while('}' != *cmd){ 
			 		        cmd = (char*)Task_Cmd[i][++j];  
							  } 
			  	    }						
			    	}
				 	} 				
					else if(2 == if_result){//if结果为假
					  if_result = 0;
					  cmd = (char*)Task_Cmd[i][++j]; 
	  				if('{' == *cmd){ 
		  			  while('}' != *cmd){ 
			  		    cmd = (char*)Task_Cmd[i][++j]; 
				  	  }  
				  	}
						cmd = (char*)Task_Cmd[i][++j]; 
						if(0 == strcmp(cmd,"c77")){//如果有else,执行else{}的指令
							cmd = (char*)Task_Cmd[i][++j]; 
				      if('{' == *cmd){ 
		  		      while('}' != *cmd){ 
			 		        cmd = (char*)Task_Cmd[i][++j];  
									if('&' == *(cmd+i)){ 
									  DL212_Exp_Calculate((double*)Var,(char*)(cmd)); 
								  } 
								  else if('c' == *cmd){ 
								    Command_Parse(cmd); 
							  	} 
							  } 
			  	    }						
			    	}
					}		
				}
        else{
				  while(1);
				}
			} 
	  //} 
  } 
	
	return 0; 
} 
 
char Command_Parse(char *cmd){ 
  unsigned int i=0,j=0,k=0,c=0,iftime_tintoint=0,iftime_interval=0,iftime_units=0;  
	float para[12];
  
	i++;//pass掉开头的符号
	c = atoi(cmd+i); 	
	while(',' != *(cmd+i)){ 
		i++; 
	} 
	i++; 
	switch(c){ 
		case 50://DataTable 
		  Func_DataTable(cmd+i);  
		break; 
		case 72://CallSub 
			Func_CallSub(cmd+i); 
		break; 
		case 73://For 
			j = 0; 
		  while(j < CPN[c]){ 
			  For_Cycle_Para[j] = atoi(cmd+i); 
			  while(',' != *(cmd+i)){ 
			  	i++; 
			  } 
		  	i++,j++; 
		  } 
		break;	
		case 74://Loop 	
		  ;	
		break;	
		case 75://If 	
			j = 0; 	
		  while(j < 3){	
		  	switch(*(cmd+i)){	
				  case ',':	
		  			i++;	
		  		break;	
					case '&':	
			  		i++,k=atoi(cmd+i); 	
				  	if(k){	
					  	para[j++] = *(float*)(Var+k-1);	
					  }	
					  while(',' != *(cmd+i)){ 	
						  i++;	
					  } 		
					break;					
					case'=':	
						if(para[0] == para[1]){
						  return 1;
						}
						else{
						  return 2;
						} 					  	
					break;	
					case'>':	
						i++;	
				  	if(',' == *(cmd+i)){	 
							if(para[0] > para[1]){
						    return 1;
					  	}
						  else{
						    return 2;
						  } 
					  }	
					  else{	
						  if(para[0] >= para[1]){
						    return 1;
					  	}
						  else{
						    return 2;
						  } 
				  	}	
				  break;	
					case'<':	
						i++;	
					  if(',' == *(cmd+i)){	
						  if(para[0] < para[1]){
						    return 1;
					  	}
						  else{
						    return 2;
						  }
					  }	
					  else if('=' == *(cmd+i)){	
						  if(para[0] <= para[1]){
						    return 1;
					  	}
						  else{
						    return 2;
						  }
					  }	
					  else if('>' == *(cmd+i)){	
						  if(para[0] != para[1]){
						    return 1;
					  	}
						  else{
						    return 2;
						  }
					  }
            else{
						  while(1);
						}						
					break;	
					default:	
						para[j++] = atof(cmd+i); 	
  					while(',' != *(cmd+i)){ 	
							i++;	
						} 		
					break;	
				} 	
			}		
		break;	
		case 76://IfTime 	
		  //if(0 == (time_1970+sTable_Info[i].time_intoint/1000)%(sTable_Info[i].time_interval/1000)){;	
		  
		break;  	
		default: 	
			j = 0;	
  		while('\0' != *(cmd+i)){ 	
				para[j++] = atof(cmd+i);   
				while(','!=*(cmd+i) && '\0'!=*(cmd+i)){ 					
					i++; 	
				}
				if(',' == *(cmd+i)){
				  i++;				
				}
			} 	
			Function[c]((double*)para); 	
	  break; 	
	} 	
  
	return 0; 
} 

unsigned int Func_CallSub(char *cmd){
	unsigned int i=0,sub=0;
	const char *p;
	
	if(',' == *(cmd+i)){
	  i++;
	}
	sub = atoi(cmd+i);
	p = Sub_Cmd[sub-1];
	p += 6;//pass掉c78,1,
	Command_Parse((char*)p);
	
  return 0;
}
  
const char *Test_DataTable=",1,0,1,2,c52,1,5,c53,1,4\0";
unsigned int Func_DataTable(char *cmd){
  unsigned char t,i=0,j=0,c;
	unsigned int datatable_cmd[4];
	
	while(j < 4){
	  if(',' == *(cmd+i)){
      i++;
    } 
    else{ 
			datatable_cmd[j++] = atoi(cmd+i); 
      while(',' != *(cmd+i)){ 
        i++;
      } 
    }   
	}
	t = datatable_cmd[0];
	sNUM.table = t>sNUM.table?t:sNUM.table;
	if(0 == t){
	  while(1);
	}
	t--;//上位机表号从1开始
	sTable_Info[t].time_intoint = datatable_cmd[1];
	sTable_Info[t].time_interval = datatable_cmd[2];
	sTable_Info[t].time_unit = datatable_cmd[3];
	while('\0' != *(cmd+i)){
    if(',' == *(cmd+i)){
      i++;
    } 
    else if('c' == *(cmd+i)){ 
      i++; 
			c = atoi(cmd+i);
			if(c==51 || c==52 || c==53 || c==54 || c==55 || c==56){
			  while(',' != *(cmd+i)){
				  i++;
				}
				i++;
			  sTABLE_PARA[t].dest[c-51] = atoi(cmd+i);
				while(',' != *(cmd+i)){
				  i++;
				}
				i++;
				sTABLE_PARA[t].reps[c-51] = atoi(cmd+i);
				while(',' != *(cmd+i)){ 
				  if('\0' == *(cmd+i)){ 
					  return i; 
					} 
					i++; 
				} 
				i++; 
      } 
    } 
  } 
	
	return i;
}

 
 
char Lexical_Analyzer(char *program){ 
  unsigned int i=0,j=0,ptr=0; 
	const char *data_logger_type; 
	/*if(sProgram_Info.crc != sProtocol_Func.program_crc_calculate(sProgram_Info.lenth)){
	  return 1;
	} */
	CMD_Pointer_Init();
	while(*(program+i)!='$' && i<0xFFFF){
		i++;
	}
	if(i == 0xFFFF){
	  return 1;
	}
  sProgram_Info.lenth = i;  
	data_logger_type = program; 
	ptr += strlen(data_logger_type)+1;
	if(strcmp(data_logger_type,(char*)sDL212_Info.type)){
	  return 1;//wrong datalogger tpe 
	} 
	i=0,j=0;
	//Table name
  while(*(program+ptr)!=';' && i<TABLE_NUM_MAX){
	  TableDef_Name[i] = program+ptr; 
		ptr += strlen(TableDef_Name[i])+1;
		i++;
	}  
	sNUM.table=i , ptr++ , i=0,j=0;
	//Public 
  while(*(program+ptr)!=';' && i<PUBLIC_NUM_MAX){
	  TableDef_Var_Def[0][i] = program+ptr;  
	  ptr += strlen(TableDef_Var_Def[0][i])+1;
		TableDef_Var_Array_Len[0][i] = atoi(program+ptr-4);
		sNUM.var[0] += TableDef_Var_Array_Len[0][i];
		i++;
	} 
	sNUM.array[0] = i;
	ptr++ , i=0,j=0;
	//Dim
	while(*(program+ptr)!=';' && i<DIM_NUM_MAX){
	  DimDef_Name[i] = program+ptr; 
	  ptr += strlen(DimDef_Name[i])+1;
		sDIM_NUM.array_len[i] = atoi(program+ptr-4);
		sDIM_NUM.var += sDIM_NUM.array_len[i];
		i++;
	} 
	sDIM_NUM.array=i , ptr++,i=0,j=0; 
	//Table var
	for(i=1;i<sNUM.table;i++){ 
	  while(*(program+ptr)!=';' && j<PUBLIC_NUM_MAX){
	    TableDef_Var_Def[i][j] = program+ptr; 
	    ptr += strlen(TableDef_Var_Def[i][j])+1;
			TableDef_Var_Array_Len[i][j] = atoi(program+ptr-4);
			sNUM.var[i] += TableDef_Var_Array_Len[i][j];
		  j++;
	  }
		sNUM.array[i]=j , ptr++,j=0; 
	}  
	i=0,j=0;
	//Task command
	while(*(program+ptr)!='@' && *(program+ptr)!='$'){ 
    while(*(program+ptr)!=';' && j<TASK_CMD_NUM_MAX){ 
		  Task_Cmd[i][j] = program+ptr;  
			ptr += strlen(Task_Cmd[i][j])+1;
			j++;
			sNUM.task_cmd_num[i]++;
		} 
		sNUM.task++;
		Task_Interval[i] = atoi(Task_Cmd[i][0]+5); 
		ptr++,i++,j=0;
	}
	i=0;
	//Subroutine command
	if(*(program+ptr) == '@'){ 
		ptr++;
	  while(*(program+ptr)!='$' && i<SUB_NUM_MAX){ 
		  Sub_Cmd[i] = program+ptr; 
			ptr += strlen(Sub_Cmd[i]);   
			sNUM.sub++; 
		  ptr++,i++; 
	  } 
	} 
	
  return 0; 
}  

char CMD_Pointer_Init(void){
  unsigned int i,j;
	
	for(i=0;i<TABLE_NUM_MAX;i++){ 
    TableDef_Name[i] = 0; 
	}
	for(i=0;i<TABLE_NUM_MAX;i++){
	  for(j=0;j<PUBLIC_NUM_MAX;j++){
		  TableDef_Var_Def[i][j] = 0;
		}
	} 
	for(i=0;i<DIM_NUM_MAX;i++){ 
    DimDef_Name[i] = 0; 
	}
	for(i=0;i<TASK_NUM_MAX;i++){
	  for(j=0;j<TASK_CMD_NUM_MAX;j++){
		  Task_Cmd[i][j] = 0;
		}
	}
	for(i=0;i<SUB_NUM_MAX;i++){ 
    Sub_Cmd[i] = 0; 
	}
	
	return 0;
}


float Battery(double *para){ 
	float adc=0; 
	
  ADS1248SetChannel(2,0);
	ADS1248SetChannel(3,1); 
  adc = ADS1248RDATARead();
	adc = adc*3/0x7FFFFF;
	adc = adc*251/51;
	
	return adc;
}
  
void VGet(double *para){ 
  unsigned int dest,reps,chan,time,i; 
	
	dest = (unsigned int)(*para); 
	reps = (unsigned int)(*(para+1)); 
	chan = (unsigned int)(*(para+2)); 
	time = (unsigned int)(*(para+3)); 
	delay_ms(time); 
  for(i=0;i<reps;i++){ 
    Var[dest+i] = psSE_FUNC->read(chan+i); 
	} 
}
 
unsigned int  C1_Dest=0  ,C2_Dest=0  ,P_SW_Dest=0  ,F_Mea_Dest=0;
unsigned int  C1_Time=0  ,C2_Time=0  ,P_SW_Time=0  ,F_Mea_Time=0; 
unsigned char C1_Option=0,C2_Option=0,P_SW_Option=0,F_Mea_Option=0;
void PulseCount(double *para){ 
  unsigned int dest,pchan,config,option,time_ms,i,count; 
	
	/*dest = (unsigned int)(*para); 
	pchan = (unsigned int)(*(para+1)); 
	config = (unsigned int)(*(para+2)); 
	option = (unsigned int)(*(para+3)); 
	time_ms = (unsigned int)(*(para+4));*/ 
	dest = 1; pchan = 4; config = 1; option = 1; time_ms = 2000; 
	switch(pchan){
	  case 1://C1
			C1_Dest=dest,C1_Time=time_ms*2,C1_Option=(unsigned char)option;//C1上升下降沿都检测，所以time_ms*2用于计算
			TIM7_Init(time_ms); 
		break;
		case 2://C2
			C2_Dest=dest,C2_Time=time_ms,C2_Option=(unsigned char)option;
			TIM4_Init(time_ms); 
		break;
		case 3://P_SW
			P_SW_Dest=dest,P_SW_Time=time_ms*2,P_SW_Option=(unsigned char)option;//P_SW上升下降沿都检测，所以time_ms*2用于计算
			TIM6_Init(time_ms); 
		break;
		case 4://F_Mea
			F_Mea_Dest=dest,F_Mea_Time=time_ms*2,F_Mea_Option=(unsigned char)option;//F_Mea上升下降沿都检测，所以time_ms*2用于计算
		  psSE_FUNC->chan_open(1); 
		  TIM10_Init(time_ms); 
		break;
		case 5://F_Mea
			F_Mea_Dest=dest,F_Mea_Time=time_ms*2,F_Mea_Option=(unsigned char)option;
		  psSE_FUNC->chan_open(2); 
		  TIM10_Init(time_ms); 
		break;
		case 6://F_Mea
			F_Mea_Dest=dest,F_Mea_Time=time_ms*2,F_Mea_Option=(unsigned char)option;
		  psSE_FUNC->chan_open(3); 
		  TIM10_Init(time_ms); 
		break;
		case 7://F_Mea
			F_Mea_Dest=dest,F_Mea_Time=time_ms*2,F_Mea_Option=(unsigned char)option;
		  psSE_FUNC->chan_open(4); 
		  TIM10_Init(time_ms); 
		break;
		default:
		break;
	}  
} 
 
void ExciteV(double *para){ 
 
} 

void SDI12(double *para){
  unsigned int dest,sdiport,sdiaddress,sdicommand;
  
	
}
   
void SetFlagHigh(double *para){

}

void SetFlagLow(double *para){

}

 
 
float SquareRootFloat(float number){
	float x,y,*pt1;
  unsigned int i;
	int *pt2;  
  const float f = 1.5;  
	
  x = number * 0.5;
  y = number;
  pt2 = (int *)&y;
  i = *pt2;
  i  = 0x5f3759df - ( i >> 1 );//0x5f375a86可能更好，没测试过
  pt1=(float *)&i;
  y = *pt1;
  y  = y * ( f - ( x * y * y ) );
  y  = y * ( f - ( x * y * y ) );
  y = 1/y;

  return  y;
}



