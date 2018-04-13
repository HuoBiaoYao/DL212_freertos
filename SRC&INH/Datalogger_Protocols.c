#include "Datalogger_Protocols.h"
#include "DL212.h"
#include "string.h" 

struct _PROTOCOL_BUF sPBuf;
struct _DATALOGGER_TIME sDatalogger_Time;
struct _DATALOGGER_INFO sDL212_Info={"DL212","1.0.alpha_20171114","1.0_20171114"};
struct _DATALOGGER_STATE sDL212_State;
struct _PROGRAM_INFO sProgram_Info;
struct _TABLE_INFO sTable_Info[TABLE_NUM_MAX];
 
struct _PROTOCOL_FUNCTION sProtocol_Func={
  Packet_Respond,
	//send 
	//set and save time
};
 
  
char Packet_Respond(void){
  unsigned int i=0,time=0,len=0,begrecnbr=0,endrecnbr=0,offset=0; 
	unsigned short int crc=0,table_crc=0,program_crc; 
	unsigned char msg_type=0,t=0,isoffset=0;
	
	if(SERSYNCBYTE==sPBuf.rx_buf[0] && SERSYNCBYTE==sPBuf.rx_buf[sPBuf.rx_len-1] && sPBuf.rx_len>9){
		ReadPb(sPBuf.rx_buf,sPBuf.rx_len);
		memcpy(&crc,&sPBuf.rx_buf[sPBuf.rx_len-3],2);
		if(crc != CRC16(sPBuf.rx_buf,sPBuf.rx_len)){
		  return 0xFF; 
		} 
		sPBuf.tx_buf[0]=SERSYNCBYTE,sPBuf.tx_buf[1]=sPBuf.tx_buf[2]=sPBuf.tx_buf[3]=sPBuf.tx_buf[4]=sPBuf.tx_buf[5]=sPBuf.tx_buf[6]=0x88;
    sPBuf.tx_buf[8] = 0x88;	
		sPBuf.tx_buf[9] = 0x00;
		msg_type = sPBuf.rx_buf[7];  
	  switch(msg_type){ 
		  case CONNECT_CMD:
			  sPBuf.tx_buf[7]=CONNECT_RSP, sPBuf.tx_buf[10]=0x01; 
			  sPBuf.tx_len = 11;
			break; 
			case DISCONNECT_CMD: 
			  sPBuf.tx_buf[7] = DISCONNECT_RSP ; 
			  sPBuf.tx_len = 10; 
			break;
			case CLOCK_CMD:
				sPBuf.tx_buf[7] = DISCONNECT_RSP ; 
				memcpy(&time,&sPBuf.rx_buf[9],4);
				if(0 != time){ 
				  sProtocol_Func.set_datalogger_time(time);
				} 
				memcpy(&sPBuf.tx_buf[10],&sDatalogger_Time.time_1970,4);
			  sPBuf.tx_len = 14;
			break;
			case GETINFO_CMD:
				sPBuf.tx_buf[7] = GETINFO_RSP; 
        strcpy((char*)&sPBuf.tx_buf[10],(char*)&sDL212_Info.type); 
			  len = len+strlen((char*)sDL212_Info.type)+1;
			  strcpy((char*)&sPBuf.tx_buf[10],(char*)&sDL212_Info.os_version); 
			  len = len+strlen((char*)sDL212_Info.os_version)+1;
			  strcpy((char*)&sPBuf.tx_buf[10],(char*)&sDL212_Info.protocol_version); 
			  len = len+strlen((char*)sDL212_Info.protocol_version)+1;
			  sPBuf.tx_len = 10+len;
			break;
			case GETSTATE_CMD:
				sPBuf.tx_buf[7] = GETSTATE_RSP;
			  memcpy(&sPBuf.tx_buf[10],(char*)&sDL212_State,sizeof(sDL212_State));
			  sPBuf.tx_len = 10+sizeof(sDL212_State);
			break;
			case GETDATA_CMD:
				sPBuf.tx_buf[7] = GETDATA_RSP;
			  t = sPBuf.rx_buf[9];
		  	memcpy(&table_crc,&sPBuf.rx_buf[10],2);
			  if(table_crc != sTable_Info[t].crc){
				  sPBuf.tx_buf[9] = 0x01;
					sPBuf.tx_len = 10;
				}
				else{
					sPBuf.tx_buf[10] = t;
				  memcpy(&begrecnbr,&sPBuf.rx_buf[12],4); 
			    memcpy(&endrecnbr,&sPBuf.rx_buf[16],4); 
		  	  sPBuf.tx_buf[11] = sTable_Info[t].record_len>PACK_LEN_MAX ? 0x01:0x00;
					i = PACK_LEN_MAX/sTable_Info[t].record_len;
			    //目前先不考虑有碎片的情况
					if(0xFFFFFFFF == begrecnbr){
						memcpy(&sPBuf.tx_buf[12],&begrecnbr,4);
						memcpy(&sPBuf.tx_buf[16],&endrecnbr,4);
					  sProtocol_Func.get_record(&sPBuf.tx_buf[20],sTable_Info[t].record_max,1);
						sPBuf.tx_len = 20+sTable_Info[t].record_len;
					}
					else{
					  begrecnbr = begrecnbr>sTable_Info[t].record_min?begrecnbr:sTable_Info[t].record_min; 
				    endrecnbr = endrecnbr<sTable_Info[t].record_max?endrecnbr:sTable_Info[t].record_max; 
						i = i<=(endrecnbr-begrecnbr) ? i:(endrecnbr-begrecnbr);
						endrecnbr = begrecnbr+i;
						memcpy(&sPBuf.tx_buf[12],&begrecnbr,4);
						memcpy(&sPBuf.tx_buf[16],&endrecnbr,4);
						sProtocol_Func.get_record(&sPBuf.tx_buf[20],begrecnbr,i); 
						sPBuf.tx_len = 20+i*sTable_Info[t].record_len;
					} 
				}
			break;
			case ERASEDATA_CMD:
				sPBuf.tx_buf[7] = ERASEDATA_RSP;
				sProtocol_Func.erase_data();
			  sPBuf.tx_len = 10;
			break;
			case DELETEPROGRAM_CMD:
				sPBuf.tx_buf[7] = DELETEPROGRAM_RSP;
				sProtocol_Func.delete_program();
			  sPBuf.tx_len = 10;
			break;
			case DOWNLOADPROGRAM_CMD:
				sPBuf.tx_buf[7] = DOWNLOADPROGRAM_RSP;
			  strcpy((char*)sProgram_Info.name,(char*)&sPBuf.rx_buf[9]);
			  len = strlen((char*)sProgram_Info.name)+1;
			  memcpy(&sProgram_Info.lenth,&sPBuf.rx_buf[9+len],4) , len+=4;
			  memcpy(&sProgram_Info.crc,&sPBuf.rx_buf[9+len],2) , len+=2;
			  memcpy(&offset,&sPBuf.rx_buf[9+len],4) , len+=4; 
			  if(0 == offset){
				  sProgram_Info.offset = 0;
				}
			  if(offset != sProgram_Info.offset){
				  sPBuf.tx_buf[9] = 0x01; 
					sPBuf.tx_len = 10;
				}
				else{
				  sProtocol_Func.download_program(&sPBuf.rx_buf[9+len],offset,sPBuf.rx_len-len-12);
			    sProgram_Info.offset = offset+sPBuf.rx_len-len-12;
					memcpy(&sPBuf.tx_buf[10],&sProgram_Info.offset,4);
			    if(sProgram_Info.offset == sProgram_Info.lenth){
				    program_crc = sProtocol_Func.program_crc_calculate(sProgram_Info.lenth);
					  if(sProgram_Info.crc != program_crc){
					    sPBuf.tx_buf[9] = 0x02;  
					  }
						else{
						  sPBuf.tx_buf[9] = 0x03;  
						}
						sPBuf.tx_len = 14; 
				  }
					else if(sProgram_Info.offset > sProgram_Info.lenth){
					  sPBuf.tx_buf[9] = 0x01; 
					  sPBuf.tx_len = 10;
					}
				} 
			break;
			case GETPROGRAMINFO_CMD:
				sPBuf.tx_buf[7] = GETPROGRAMINFO_RSP;
		    strcpy((char*)&sPBuf.tx_buf[10],(char*)sProgram_Info.name); 
			  len = len+strlen((char*)sProgram_Info.name)+1;
			  memcpy(&sPBuf.tx_buf[10+len],&sProgram_Info.lenth,4);
			  memcpy(&sPBuf.tx_buf[14+len],&sProgram_Info.crc,2);
			  memcpy(&sPBuf.tx_buf[16+len],&sProgram_Info.time,4);
			  sPBuf.tx_len = 20+len;
			break;
			case GETTABLEDEFINITION_CMD:
		    sPBuf.tx_buf[7] = GETTABLEDEFINITION_RSP;
			  t = sPBuf.rx_buf[9];
			  memcpy(&table_crc,&sPBuf.rx_buf[10],2); 
			  if(table_crc == sTable_Info[t].crc){
				  sPBuf.tx_buf[10] = t; 
					i=0;
					/*while(*((unsigned int*)psTable_Def[t]+i+0)!=0 || 
						    *((unsigned int*)psTable_Def[t]+i+1)!=0 ||
								*((unsigned int*)psTable_Def[t]+i+2)!=0 ||
								*((unsigned int*)psTable_Def[t]+i+3)!=0 ||
								*((unsigned int*)psTable_Def[t]+i+4)!=0){
					  i++;
					} 
          memcpy((char*)&sPBuf.tx_buf[11],(char*)psTable_Def[t],i+5); */
					sPBuf.tx_len = 20+i+5;
				}
				else{
				  sPBuf.tx_buf[9] = 0x01;    
					sPBuf.tx_len = 10;
				}
			break;
			case SETPUBLIC_CMD:
				;
			break;
			default:
				;
			break;
		} 
		crc = CRC16(sPBuf.tx_buf+1,sPBuf.tx_len-1);
		memcpy(&sPBuf.tx_buf+sPBuf.tx_len,&crc,2) , sPBuf.tx_len+=2;
		SendPb(sPBuf.tx_buf,sPBuf.tx_len);
		sProtocol_Func.packet_send(sPBuf.tx_len) , sPBuf.tx_len = 0;
	}
	else{ 
    sPBuf.rx_buf[0]=0 , sPBuf.rx_len=0;
  }

  return 0;
} 

//lenth为整个数组长度，包括头尾的两个0xAE
unsigned int SendPb(unsigned char *buf,unsigned int lenth){
  unsigned int i,j,len;

  len = lenth; 
 	*(buf+0) = SERSYNCBYTE;
  for(i=1;i<len-1;i++){
    if(SERSYNCBYTE == *(buf+i)){
      *(buf+i) = QUOTEBYTE;
      len++;
      for(j=len-1;j>i;j--){// 
        *(buf+j) = *(buf+j-1);
      }
			*(buf+i+1) = RBYTE_0;
			i++; 
    }
    else if(0xAC == *(buf+i)){
      len++;
      for(j=len-1;j>i;j--){// 
        *(buf+j) = *(buf+j-1);
      }
			*(buf+i+1) = RBYTE_1;
			i++; 
	  }    
  }
	
	return len;
} 
//将收到数据中被分解的特殊字节还原(AC CE -> AE , AC CC -> AC) 
unsigned int ReadPb(unsigned char *buf,unsigned int lenth){
  unsigned int i,j;
     
  for(i=1;i<lenth-1;i++){
    if(QUOTEBYTE == *(buf+i)){    
      if(RBYTE_0 == *(buf+i+1)){    
        *(buf+i) = SERSYNCBYTE;        
      }    
      else if(RBYTE_1 == *(buf+i+1)){
		  }    
      else{                    
        return 0xFFFFFFFF;    
      }    
	    lenth--;    
	    for(j=1;j<lenth-1;j++){     
	      *(buf+i+j) = *(buf+i+j+1);    
	    }     
    }    
  }      
	
  return lenth; 
}

unsigned short int CRC16(unsigned char * buf,unsigned short int len){
  unsigned short int i,crc=0xffff;
	
  while(len--!=0){
    crc ^= *buf++;
    for(i=0;i<8;i++){
      if(crc&1){
        crc >>= 1;
        crc ^= 0xa001;
      }
      else{  
        crc >>= 1;
			}
    }
  }
	
  return(crc);
}


