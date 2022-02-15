#include "usbd_uvc_if.h"


//address of the register
volatile unsigned int *DWT_CYCCNT   = (volatile unsigned int *)0xE0001004;     

//address of the register
volatile unsigned int *DWT_CONTROL  = (volatile unsigned int *)0xE0001000;     

//address of the register
volatile unsigned int *DWT_LAR      = (volatile unsigned int *)0xE0001FB0;     

//address of the register
volatile unsigned int *SCB_DEMCR    = (volatile unsigned int *)0xE000EDFC;



uint16_t pic[UVC_VIDEO_WIDTH][UVC_VIDEO_HEIGHT];

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
    uint32_t _frame_time = 0;
    uint32_t _tmp = *DWT_CYCCNT;
    static uint16_t color_cnt;

    for(int i = 0; i<UVC_VIDEO_WIDTH;i++)
    {
        for(int j = 0; j<UVC_VIDEO_HEIGHT;j++)
        {
            if(j%2)
            {
                pic[i][j] = 0xFFFF;
            }
        }
    }

    _frame_time = *DWT_CYCCNT-_tmp;
}


uint8_t *Camera_GetFrame(uint32_t *pFrameLength)
{
    pFrameLength[0] = UVC_VIDEO_MAX_FRAME_BUF_SIZE;
		  
    return (uint8_t *)&pic;
}

//https://auto.siili.com/blog/optimizing-rendering-performance-on-stm32-using-dma2d
void Camera_FillRect(uint32_t color, uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    DMA2D_HandleTypeDef hdma2d;
    hdma2d.Instance = DMA2D;
    hdma2d.Init.Mode = DMA2D_R2M;
    hdma2d.Init.ColorMode = DMA2D_OUTPUT_RGB565;
    hdma2d.Init.OutputOffset = 448 - width;
    HAL_DMA2D_Init(&hdma2d);
    HAL_DMA2D_Start(&hdma2d,color, ((uint32_t)&pic)+(x + y * 448) * 2, width, height);
    HAL_DMA2D_PollForTransfer(&hdma2d, 10);
}