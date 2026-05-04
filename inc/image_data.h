/*
 * image_data.h
 *
 * Created on: Jan 17, 2026
 * Author: ROHAN KUMAR
 */

#ifndef INC_IMAGE_DATA_H_
#define INC_IMAGE_DATA_H_

#include <stdint.h>

/* Image resolution */
#define IMAGE_WIDTH   320
#define IMAGE_HEIGHT  240

/* Total image size in bytes */
#define IMAGE_SIZE    9600

/* External declaration (NO DATA HERE) */
extern const uint8_t image_data[IMAGE_SIZE];

#endif /* INC_IMAGE_DATA_H_ */
