/*
 * RC_Data.c
 *
 *  Created on: 2019年4月5日
 *      Author: zengwangfa
 *      Notes:  遥控器数据解析 Remote Control receiving Data
 */
#include "RC_Data.h"
#include "led.h"
#include "PropellerControl.h"
#define MAX_DATA_LENS 16  //有效数据包长度【不包含 包头、长度位、校验位】

ReceiveDataType ReceiveData = {
		.THR = 1500,
		.YAW = 1500,
		.ROL = 1500,
	  .PIT = 1500
};

ControlDataType Control = {
										.Power = 0
};
uint32 Frame_Conut = 0;//数据包 帧计数
uint8 Frame_EndFlag = 0; //接收数据包结束标志 
uint8 RC_Control_Data[30] = {0};
uint8 Receive_Data_OK = 0;
uint8 Control_RxCheck = 0;	  //尾校验字
uint8 Control_RxCount = 0;	  //接收计数
/**
  * @brief  Remote_Control_Data_Analysis(控制数据解析)
  * @param  控制字符数据 uint8 Data
  * @retval None
  * @notice 从第四个字节开始为控制字符
  */
void Remote_Control_Data_Analysis(uint8 Data) //控制数据解析
{

		static uint8 i = 0;	
	
		RC_Control_Data[Control_RxCount++] = Data;	//将收到的数据存入缓冲区中
		if(Control_RxCount <= (MAX_DATA_LENS+8)){ //定义数据长度未包括包头和包长3个字节,+4)          
				if(RC_Control_Data[0] == 0xAA){ //接收到包头0xAA
						if(Control_RxCount > 3){
								if(RC_Control_Data[1] == 0x55){ //接收到包头0x55
										if(RC_Control_Data[2] > MAX_DATA_LENS+4){Receive_Data_OK = 0;Control_RxCount = 0;return;} //接收超过协议字符数
											
										if(Control_RxCount >= RC_Control_Data[2]+4){ //接收到数据包长度位，开始判断什么时候开始计算校验
												for(i = 0;i <= (Control_RxCount-2);i++){ //累加和校验
														Control_RxCheck += RC_Control_Data[i];
												}
												if(Control_RxCheck == RC_Control_Data[Control_RxCount-1]){
														Receive_Data_OK = 1; //接收数据包成功
														Frame_EndFlag = 1;
														Frame_Conut ++;
												}
												else {Receive_Data_OK = 0;}
												 
												Control_RxCheck = 0; //接收完成清零
												Control_RxCount = 0;	
										}
								}
								else {Receive_Data_OK = 0;Control_RxCount = 0;Receive_Data_OK = 0;Control_Clear();return;} //接收不成功清零
						}//
				}
				else {Receive_Data_OK = 0;Control_RxCount = 0;Receive_Data_OK = 0;Control_Clear();return;} //接收不成功清零
		}
		else {Receive_Data_OK = 0;Control_RxCount = 0;Receive_Data_OK = 0;Control_Clear();return;} //接收不成功清零
		
		if(1 == Receive_Data_OK){
			
				Control.Depth_Lock     = RC_Control_Data[3]; //姿态控制
				Control.Direction_Lock = RC_Control_Data[4];
				Control.Move					 = RC_Control_Data[5]; //前后
				Control.Translation		 = RC_Control_Data[6]; //左右平移
				Control.Vertical 			 = RC_Control_Data[7]; //垂直
				Control.Rotate 				 = RC_Control_Data[8]; //旋转
				
				Control.Power 				 = RC_Control_Data[9];  //动力控制 两倍[0~500]
				Control.Light 				 = RC_Control_Data[10]; //灯光控制
				
				Control.Focus 				 = RC_Control_Data[11]; //变焦摄像头控制
				
				Control.Yuntai 				 = RC_Control_Data[12]; //云台控制
				Control.Arm						 = RC_Control_Data[13]; //机械臂控制

		}
		else{

				Control_Clear();
		}

}

void Control_Clear(void)
{
		Control.Depth_Lock     = 0x00; //姿态控制
		Control.Direction_Lock = 0x00;
		Control.Move					 = 0x00;
		Control.Translation		 = 0x00;
		Control.Vertical 			 = 0x00;
		Control.Rotate 				 = 0x00;
		
		//Control.Power 				 = 0x00;  //动力控制
		Control.Light 				 = 0x00; //灯光控制
		
		Control.Focus 				 = 0x00; //变焦摄像头控制
		
		Control.Yuntai 				 = 0x00; //云台控制
		Control.Arm						 = 0x00; //机械臂控制
}

			



	
	
	
	
