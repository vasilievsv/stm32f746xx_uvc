#include "usbd_uvc_if.h"


//address of the register
volatile unsigned int *DWT_CYCCNT   = (volatile unsigned int *)0xE0001004;     

//address of the register
volatile unsigned int *DWT_CONTROL  = (volatile unsigned int *)0xE0001000;     

//address of the register
volatile unsigned int *DWT_LAR      = (volatile unsigned int *)0xE0001FB0;     

//address of the register
volatile unsigned int *SCB_DEMCR    = (volatile unsigned int *)0xE000EDFC;


typedef struct 
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
}pixel888_t;




pixel888_t pic[UVC_VIDEO_WIDTH][UVC_VIDEO_HEIGHT];

void Camera_On(void);
void Camera_Off(void);
uint8_t *Camera_GetFrame(uint32_t *pFrameLength);
void Camera_FreeFrame(uint8_t *frame);


USBD_UVC_CameraTypeDef USBD_UVC_Camera =
{
	Camera_On,
	Camera_Off,
	Camera_GetFrame,
	Camera_FreeFrame
};


void Camera_On(void)
{
	*DWT_LAR = 0xC5ACCE55; // unlock (CM7)
	*SCB_DEMCR |= 0x01000000;
	*DWT_CYCCNT = 0; // reset the counter
	*DWT_CONTROL |= 1 ; // enable the counter	
}

void Camera_Off(void)
{

}

void Camera_FreeFrame(uint8_t *frame)
{

}


uint8_t reg,data,write,read;

void Camera_Loop(void)
{

}


uint8_t *Camera_GetFrame(uint32_t *pFrameLength)
{
	uint32_t _frame_time = 0;
	uint32_t _tmp = *DWT_CYCCNT;

	pFrameLength[0] = UVC_VIDEO_WIDTH*UVC_VIDEO_HEIGHT*3;
	
	
	for(int i = 0; i<UVC_VIDEO_WIDTH;i++)
	{
			for(int j = 0; j<UVC_VIDEO_HEIGHT;j++)
			{
				pic[i][j].r = 0xFF;
				pic[i][j].g = 0xFF;
				pic[i][j].b = 0xFF;
			}
	}
	
	_frame_time = *DWT_CYCCNT-_tmp;

	return (uint8_t *)&pic;
}

