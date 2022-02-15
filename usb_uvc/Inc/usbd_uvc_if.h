/*
 * usbd_camera.h
 *
 */

#ifndef INC_USBD_CAMERA_H_
#define INC_USBD_CAMERA_H_

#include "main.h"
#include "usbd_uvc.h"

extern USBD_UVC_CameraTypeDef USBD_UVC_Camera;

typedef struct 
{
	uint8_t r;
	uint8_t g;
	uint8_t b;

}pixel888_t;

extern uint16_t pic[UVC_VIDEO_WIDTH][UVC_VIDEO_HEIGHT];

#define CAMERA_BUFFER_STATE_FREE            1
#define CAMERA_BUFFER_STATE_LOCKED_BY_DMA   2
#define CAMERA_BUFFER_STATE_READY           3
#define CAMERA_BUFFER_STATE_LOCKED_BY_UVC   4


void Camera_Loop(void);
void Camera_FillRect(uint32_t color, uint32_t x, uint32_t y, uint32_t width, uint32_t height);

#endif /* INC_USBD_CAMERA_H_ */
