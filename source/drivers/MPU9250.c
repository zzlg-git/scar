#include "common.h"
#include "MPU9250.h"

float q[4]; /* 四元数*/
float gyrof[3];  /* X/Y/Z 角速度*/
float accelf[3]; /* X/Y/Z 加速度 */
float yprf[3];   /* yaw/pitch/roll*/
float mag[3];
unsigned char dmpdatas[42];

//DMP设置以及初始化
const unsigned char dmpmemorydata[1929]={
 // bank 0, 256 bytes
    0xFB, 0x00, 0x00, 0x3E, 0x00, 0x0B, 0x00, 0x36, 0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x00,
    0x00, 0x65, 0x00, 0x54, 0xFF, 0xEF, 0x00, 0x00, 0xFA, 0x80, 0x00, 0x0B, 0x12, 0x82, 0x00, 0x01,
    0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x28, 0x00, 0x00, 0xFF, 0xFF, 0x45, 0x81, 0xFF, 0xFF, 0xFA, 0x72, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03, 0xE8, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x7F, 0xFF, 0xFF, 0xFE, 0x80, 0x01,
    0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x3E, 0x03, 0x30, 0x40, 0x00, 0x00, 0x00, 0x02, 0xCA, 0xE3, 0x09, 0x3E, 0x80, 0x00, 0x00,
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00,
    0x41, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x0B, 0x2A, 0x00, 0x00, 0x16, 0x55, 0x00, 0x00, 0x21, 0x82,
    0xFD, 0x87, 0x26, 0x50, 0xFD, 0x80, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x05, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x6F, 0x00, 0x02, 0x65, 0x32, 0x00, 0x00, 0x5E, 0xC0,
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFB, 0x8C, 0x6F, 0x5D, 0xFD, 0x5D, 0x08, 0xD9, 0x00, 0x7C, 0x73, 0x3B, 0x00, 0x6C, 0x12, 0xCC,
    0x32, 0x00, 0x13, 0x9D, 0x32, 0x00, 0xD0, 0xD6, 0x32, 0x00, 0x08, 0x00, 0x40, 0x00, 0x01, 0xF4,
    0xFF, 0xE6, 0x80, 0x79, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD0, 0xD6, 0x00, 0x00, 0x27, 0x10,

    // bank 1, 256 bytes
    0xFB, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xFA, 0x36, 0xFF, 0xBC, 0x30, 0x8E, 0x00, 0x05, 0xFB, 0xF0, 0xFF, 0xD9, 0x5B, 0xC8,
    0xFF, 0xD0, 0x9A, 0xBE, 0x00, 0x00, 0x10, 0xA9, 0xFF, 0xF4, 0x1E, 0xB2, 0x00, 0xCE, 0xBB, 0xF7,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x02, 0x00, 0x02, 0x02, 0x00, 0x00, 0x0C,
    0xFF, 0xC2, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0xCF, 0x80, 0x00, 0x40, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x14,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x03, 0x3F, 0x68, 0xB6, 0x79, 0x35, 0x28, 0xBC, 0xC6, 0x7E, 0xD1, 0x6C,
    0x80, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB2, 0x6A, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x30,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x25, 0x4D, 0x00, 0x2F, 0x70, 0x6D, 0x00, 0x00, 0x05, 0xAE, 0x00, 0x0C, 0x02, 0xD0,

    // bank 2, 256 bytes
    0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x00, 0x54, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x00, 0x54, 0x00, 0x00, 0xFF, 0xEF, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
    0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    // bank 3, 256 bytes
    0xD8, 0xDC, 0xBA, 0xA2, 0xF1, 0xDE, 0xB2, 0xB8, 0xB4, 0xA8, 0x81, 0x91, 0xF7, 0x4A, 0x90, 0x7F,
    0x91, 0x6A, 0xF3, 0xF9, 0xDB, 0xA8, 0xF9, 0xB0, 0xBA, 0xA0, 0x80, 0xF2, 0xCE, 0x81, 0xF3, 0xC2,
    0xF1, 0xC1, 0xF2, 0xC3, 0xF3, 0xCC, 0xA2, 0xB2, 0x80, 0xF1, 0xC6, 0xD8, 0x80, 0xBA, 0xA7, 0xDF,
    0xDF, 0xDF, 0xF2, 0xA7, 0xC3, 0xCB, 0xC5, 0xB6, 0xF0, 0x87, 0xA2, 0x94, 0x24, 0x48, 0x70, 0x3C,
    0x95, 0x40, 0x68, 0x34, 0x58, 0x9B, 0x78, 0xA2, 0xF1, 0x83, 0x92, 0x2D, 0x55, 0x7D, 0xD8, 0xB1,
    0xB4, 0xB8, 0xA1, 0xD0, 0x91, 0x80, 0xF2, 0x70, 0xF3, 0x70, 0xF2, 0x7C, 0x80, 0xA8, 0xF1, 0x01,
    0xB0, 0x98, 0x87, 0xD9, 0x43, 0xD8, 0x86, 0xC9, 0x88, 0xBA, 0xA1, 0xF2, 0x0E, 0xB8, 0x97, 0x80,
    0xF1, 0xA9, 0xDF, 0xDF, 0xDF, 0xAA, 0xDF, 0xDF, 0xDF, 0xF2, 0xAA, 0xC5, 0xCD, 0xC7, 0xA9, 0x0C,
    0xC9, 0x2C, 0x97, 0x97, 0x97, 0x97, 0xF1, 0xA9, 0x89, 0x26, 0x46, 0x66, 0xB0, 0xB4, 0xBA, 0x80,
    0xAC, 0xDE, 0xF2, 0xCA, 0xF1, 0xB2, 0x8C, 0x02, 0xA9, 0xB6, 0x98, 0x00, 0x89, 0x0E, 0x16, 0x1E,
    0xB8, 0xA9, 0xB4, 0x99, 0x2C, 0x54, 0x7C, 0xB0, 0x8A, 0xA8, 0x96, 0x36, 0x56, 0x76, 0xF1, 0xB9,
    0xAF, 0xB4, 0xB0, 0x83, 0xC0, 0xB8, 0xA8, 0x97, 0x11, 0xB1, 0x8F, 0x98, 0xB9, 0xAF, 0xF0, 0x24,
    0x08, 0x44, 0x10, 0x64, 0x18, 0xF1, 0xA3, 0x29, 0x55, 0x7D, 0xAF, 0x83, 0xB5, 0x93, 0xAF, 0xF0,
    0x00, 0x28, 0x50, 0xF1, 0xA3, 0x86, 0x9F, 0x61, 0xA6, 0xDA, 0xDE, 0xDF, 0xD9, 0xFA, 0xA3, 0x86,
    0x96, 0xDB, 0x31, 0xA6, 0xD9, 0xF8, 0xDF, 0xBA, 0xA6, 0x8F, 0xC2, 0xC5, 0xC7, 0xB2, 0x8C, 0xC1,
    0xB8, 0xA2, 0xDF, 0xDF, 0xDF, 0xA3, 0xDF, 0xDF, 0xDF, 0xD8, 0xD8, 0xF1, 0xB8, 0xA8, 0xB2, 0x86,

    // bank 4, 256 bytes
    0xB4, 0x98, 0x0D, 0x35, 0x5D, 0xB8, 0xAA, 0x98, 0xB0, 0x87, 0x2D, 0x35, 0x3D, 0xB2, 0xB6, 0xBA,
    0xAF, 0x8C, 0x96, 0x19, 0x8F, 0x9F, 0xA7, 0x0E, 0x16, 0x1E, 0xB4, 0x9A, 0xB8, 0xAA, 0x87, 0x2C,
    0x54, 0x7C, 0xB9, 0xA3, 0xDE, 0xDF, 0xDF, 0xA3, 0xB1, 0x80, 0xF2, 0xC4, 0xCD, 0xC9, 0xF1, 0xB8,
    0xA9, 0xB4, 0x99, 0x83, 0x0D, 0x35, 0x5D, 0x89, 0xB9, 0xA3, 0x2D, 0x55, 0x7D, 0xB5, 0x93, 0xA3,
    0x0E, 0x16, 0x1E, 0xA9, 0x2C, 0x54, 0x7C, 0xB8, 0xB4, 0xB0, 0xF1, 0x97, 0x83, 0xA8, 0x11, 0x84,
    0xA5, 0x09, 0x98, 0xA3, 0x83, 0xF0, 0xDA, 0x24, 0x08, 0x44, 0x10, 0x64, 0x18, 0xD8, 0xF1, 0xA5,
    0x29, 0x55, 0x7D, 0xA5, 0x85, 0x95, 0x02, 0x1A, 0x2E, 0x3A, 0x56, 0x5A, 0x40, 0x48, 0xF9, 0xF3,
    0xA3, 0xD9, 0xF8, 0xF0, 0x98, 0x83, 0x24, 0x08, 0x44, 0x10, 0x64, 0x18, 0x97, 0x82, 0xA8, 0xF1,
    0x11, 0xF0, 0x98, 0xA2, 0x24, 0x08, 0x44, 0x10, 0x64, 0x18, 0xDA, 0xF3, 0xDE, 0xD8, 0x83, 0xA5,
    0x94, 0x01, 0xD9, 0xA3, 0x02, 0xF1, 0xA2, 0xC3, 0xC5, 0xC7, 0xD8, 0xF1, 0x84, 0x92, 0xA2, 0x4D,
    0xDA, 0x2A, 0xD8, 0x48, 0x69, 0xD9, 0x2A, 0xD8, 0x68, 0x55, 0xDA, 0x32, 0xD8, 0x50, 0x71, 0xD9,
    0x32, 0xD8, 0x70, 0x5D, 0xDA, 0x3A, 0xD8, 0x58, 0x79, 0xD9, 0x3A, 0xD8, 0x78, 0x93, 0xA3, 0x4D,
    0xDA, 0x2A, 0xD8, 0x48, 0x69, 0xD9, 0x2A, 0xD8, 0x68, 0x55, 0xDA, 0x32, 0xD8, 0x50, 0x71, 0xD9,
    0x32, 0xD8, 0x70, 0x5D, 0xDA, 0x3A, 0xD8, 0x58, 0x79, 0xD9, 0x3A, 0xD8, 0x78, 0xA8, 0x8A, 0x9A,
    0xF0, 0x28, 0x50, 0x78, 0x9E, 0xF3, 0x88, 0x18, 0xF1, 0x9F, 0x1D, 0x98, 0xA8, 0xD9, 0x08, 0xD8,
    0xC8, 0x9F, 0x12, 0x9E, 0xF3, 0x15, 0xA8, 0xDA, 0x12, 0x10, 0xD8, 0xF1, 0xAF, 0xC8, 0x97, 0x87,

    // bank 5, 256 bytes
    0x34, 0xB5, 0xB9, 0x94, 0xA4, 0x21, 0xF3, 0xD9, 0x22, 0xD8, 0xF2, 0x2D, 0xF3, 0xD9, 0x2A, 0xD8,
    0xF2, 0x35, 0xF3, 0xD9, 0x32, 0xD8, 0x81, 0xA4, 0x60, 0x60, 0x61, 0xD9, 0x61, 0xD8, 0x6C, 0x68,
    0x69, 0xD9, 0x69, 0xD8, 0x74, 0x70, 0x71, 0xD9, 0x71, 0xD8, 0xB1, 0xA3, 0x84, 0x19, 0x3D, 0x5D,
    0xA3, 0x83, 0x1A, 0x3E, 0x5E, 0x93, 0x10, 0x30, 0x81, 0x10, 0x11, 0xB8, 0xB0, 0xAF, 0x8F, 0x94,
    0xF2, 0xDA, 0x3E, 0xD8, 0xB4, 0x9A, 0xA8, 0x87, 0x29, 0xDA, 0xF8, 0xD8, 0x87, 0x9A, 0x35, 0xDA,
    0xF8, 0xD8, 0x87, 0x9A, 0x3D, 0xDA, 0xF8, 0xD8, 0xB1, 0xB9, 0xA4, 0x98, 0x85, 0x02, 0x2E, 0x56,
    0xA5, 0x81, 0x00, 0x0C, 0x14, 0xA3, 0x97, 0xB0, 0x8A, 0xF1, 0x2D, 0xD9, 0x28, 0xD8, 0x4D, 0xD9,
    0x48, 0xD8, 0x6D, 0xD9, 0x68, 0xD8, 0xB1, 0x84, 0x0D, 0xDA, 0x0E, 0xD8, 0xA3, 0x29, 0x83, 0xDA,
    0x2C, 0x0E, 0xD8, 0xA3, 0x84, 0x49, 0x83, 0xDA, 0x2C, 0x4C, 0x0E, 0xD8, 0xB8, 0xB0, 0xA8, 0x8A,
    0x9A, 0xF5, 0x20, 0xAA, 0xDA, 0xDF, 0xD8, 0xA8, 0x40, 0xAA, 0xD0, 0xDA, 0xDE, 0xD8, 0xA8, 0x60,
    0xAA, 0xDA, 0xD0, 0xDF, 0xD8, 0xF1, 0x97, 0x86, 0xA8, 0x31, 0x9B, 0x06, 0x99, 0x07, 0xAB, 0x97,
    0x28, 0x88, 0x9B, 0xF0, 0x0C, 0x20, 0x14, 0x40, 0xB8, 0xB0, 0xB4, 0xA8, 0x8C, 0x9C, 0xF0, 0x04,
    0x28, 0x51, 0x79, 0x1D, 0x30, 0x14, 0x38, 0xB2, 0x82, 0xAB, 0xD0, 0x98, 0x2C, 0x50, 0x50, 0x78,
    0x78, 0x9B, 0xF1, 0x1A, 0xB0, 0xF0, 0x8A, 0x9C, 0xA8, 0x29, 0x51, 0x79, 0x8B, 0x29, 0x51, 0x79,
    0x8A, 0x24, 0x70, 0x59, 0x8B, 0x20, 0x58, 0x71, 0x8A, 0x44, 0x69, 0x38, 0x8B, 0x39, 0x40, 0x68,
    0x8A, 0x64, 0x48, 0x31, 0x8B, 0x30, 0x49, 0x60, 0xA5, 0x88, 0x20, 0x09, 0x71, 0x58, 0x44, 0x68,

    // bank 6, 256 bytes
    0x11, 0x39, 0x64, 0x49, 0x30, 0x19, 0xF1, 0xAC, 0x00, 0x2C, 0x54, 0x7C, 0xF0, 0x8C, 0xA8, 0x04,
    0x28, 0x50, 0x78, 0xF1, 0x88, 0x97, 0x26, 0xA8, 0x59, 0x98, 0xAC, 0x8C, 0x02, 0x26, 0x46, 0x66,
    0xF0, 0x89, 0x9C, 0xA8, 0x29, 0x51, 0x79, 0x24, 0x70, 0x59, 0x44, 0x69, 0x38, 0x64, 0x48, 0x31,
    0xA9, 0x88, 0x09, 0x20, 0x59, 0x70, 0xAB, 0x11, 0x38, 0x40, 0x69, 0xA8, 0x19, 0x31, 0x48, 0x60,
    0x8C, 0xA8, 0x3C, 0x41, 0x5C, 0x20, 0x7C, 0x00, 0xF1, 0x87, 0x98, 0x19, 0x86, 0xA8, 0x6E, 0x76,
    0x7E, 0xA9, 0x99, 0x88, 0x2D, 0x55, 0x7D, 0x9E, 0xB9, 0xA3, 0x8A, 0x22, 0x8A, 0x6E, 0x8A, 0x56,
    0x8A, 0x5E, 0x9F, 0xB1, 0x83, 0x06, 0x26, 0x46, 0x66, 0x0E, 0x2E, 0x4E, 0x6E, 0x9D, 0xB8, 0xAD,
    0x00, 0x2C, 0x54, 0x7C, 0xF2, 0xB1, 0x8C, 0xB4, 0x99, 0xB9, 0xA3, 0x2D, 0x55, 0x7D, 0x81, 0x91,
    0xAC, 0x38, 0xAD, 0x3A, 0xB5, 0x83, 0x91, 0xAC, 0x2D, 0xD9, 0x28, 0xD8, 0x4D, 0xD9, 0x48, 0xD8,
    0x6D, 0xD9, 0x68, 0xD8, 0x8C, 0x9D, 0xAE, 0x29, 0xD9, 0x04, 0xAE, 0xD8, 0x51, 0xD9, 0x04, 0xAE,
    0xD8, 0x79, 0xD9, 0x04, 0xD8, 0x81, 0xF3, 0x9D, 0xAD, 0x00, 0x8D, 0xAE, 0x19, 0x81, 0xAD, 0xD9,
    0x01, 0xD8, 0xF2, 0xAE, 0xDA, 0x26, 0xD8, 0x8E, 0x91, 0x29, 0x83, 0xA7, 0xD9, 0xAD, 0xAD, 0xAD,
    0xAD, 0xF3, 0x2A, 0xD8, 0xD8, 0xF1, 0xB0, 0xAC, 0x89, 0x91, 0x3E, 0x5E, 0x76, 0xF3, 0xAC, 0x2E,
    0x2E, 0xF1, 0xB1, 0x8C, 0x5A, 0x9C, 0xAC, 0x2C, 0x28, 0x28, 0x28, 0x9C, 0xAC, 0x30, 0x18, 0xA8,
    0x98, 0x81, 0x28, 0x34, 0x3C, 0x97, 0x24, 0xA7, 0x28, 0x34, 0x3C, 0x9C, 0x24, 0xF2, 0xB0, 0x89,
    0xAC, 0x91, 0x2C, 0x4C, 0x6C, 0x8A, 0x9B, 0x2D, 0xD9, 0xD8, 0xD8, 0x51, 0xD9, 0xD8, 0xD8, 0x79,

    // bank 7, 138 bytes (remainder)
    0xD9, 0xD8, 0xD8, 0xF1, 0x9E, 0x88, 0xA3, 0x31, 0xDA, 0xD8, 0xD8, 0x91, 0x2D, 0xD9, 0x28, 0xD8,
    0x4D, 0xD9, 0x48, 0xD8, 0x6D, 0xD9, 0x68, 0xD8, 0xB1, 0x83, 0x93, 0x35, 0x3D, 0x80, 0x25, 0xDA,
    0xD8, 0xD8, 0x85, 0x69, 0xDA, 0xD8, 0xD8, 0xB4, 0x93, 0x81, 0xA3, 0x28, 0x34, 0x3C, 0xF3, 0xAB,
    0x8B, 0xF8, 0xA3, 0x91, 0xB6, 0x09, 0xB4, 0xD9, 0xAB, 0xDE, 0xFA, 0xB0, 0x87, 0x9C, 0xB9, 0xA3,
    0xDD, 0xF1, 0xA3, 0xA3, 0xA3, 0xA3, 0x95, 0xF1, 0xA3, 0xA3, 0xA3, 0x9D, 0xF1, 0xA3, 0xA3, 0xA3,
    0xA3, 0xF2, 0xA3, 0xB4, 0x90, 0x80, 0xF2, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3,
    0xA3, 0xB2, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3, 0xB0, 0x87, 0xB5, 0x99, 0xF1, 0xA3, 0xA3, 0xA3,
    0x98, 0xF1, 0xA3, 0xA3, 0xA3, 0xA3, 0x97, 0xA3, 0xA3, 0xA3, 0xA3, 0xF3, 0x9B, 0xA3, 0xA3, 0xDC,
    0xB9, 0xA7, 0xF1, 0x26, 0x26, 0x26, 0xD8, 0xD8, 0xFF
};

const unsigned char dmpcfgupddata[239] = {
//  dmp config 
//  BANK    OFFSET  LENGTH  [DATA]
    0x03,   0x7B,   0x03,   0x4C, 0xCD, 0x6C,         
    0x03,   0xAB,   0x03,   0x36, 0x56, 0x76,         
    0x00,   0x68,   0x04,   0x02, 0xCB, 0x47, 0xA2,   
    0x02,   0x18,   0x04,   0x00, 0x05, 0x8B, 0xC1,   
    0x01,   0x0C,   0x04,   0x00, 0x00, 0x00, 0x00,   
    0x03,   0x7F,   0x06,   0x0C, 0xC9, 0x2C, 0x97, 0x97, 0x97, 
    0x03,   0x89,   0x03,   0x26, 0x46, 0x66,         
    0x00,   0x6C,   0x02,   0x20, 0x00,               
    0x02,   0x40,   0x04,   0x00, 0x00, 0x00, 0x00,   
    0x02,   0x44,   0x04,   0x00, 0x00, 0x00, 0x00,   
    0x02,   0x48,   0x04,   0x00, 0x00, 0x00, 0x00,   
    0x02,   0x4C,   0x04,   0x00, 0x00, 0x00, 0x00,   
    0x02,   0x50,   0x04,   0x00, 0x00, 0x00, 0x00,   
    0x02,   0x54,   0x04,   0x00, 0x00, 0x00, 0x00,  
    0x02,   0x58,   0x04,   0x00, 0x00, 0x00, 0x00,  
    0x02,   0x5C,   0x04,   0x00, 0x00, 0x00, 0x00,  
    0x02,   0xBC,   0x04,   0x00, 0x00, 0x00, 0x00,   
    0x01,   0xEC,   0x04,   0x00, 0x00, 0x40, 0x00,  
    0x03,   0x7F,   0x06,   0x0C, 0xC9, 0x2C, 0x97, 0x97, 0x97,
    0x04,   0x02,   0x03,   0x0D, 0x35, 0x5D,         
    0x04,   0x09,   0x04,   0x87, 0x2D, 0x35, 0x3D,   
    0x00,   0xA3,   0x01,   0x00,                    
    0x00,   0x00,   0x01,   0x01, 
    //原程序中此行代码为
    //0x00,   0x00,   0x00,   0x01,  即LENGTH=0x00，有错
        
    0x07,   0x86,   0x01,   0xFE,                     
    0x07,   0x41,   0x05,   0xF1, 0x20, 0x28, 0x30, 0x38, 
    0x07,   0x7E,   0x01,   0x30,                   
    0x07,   0x46,   0x01,   0x9A,                    
    0x07,   0x47,   0x04,   0xF1, 0x28, 0x30, 0x38,   
    0x07,   0x6C,   0x04,   0xF1, 0x28, 0x30, 0x38,  
    0x02,   0x16,   0x02,   0x00, 0x00,              
// 上行最后一个数据调整FIFO rate :0x00=200HZ,0x01=100HZ,0x02=66HZ,0x03=50HZ ,0x04=40HZ,0x05=33.33HZ, 
// 可从 datasheet 公式推算

//dmp updates
    0x01,   0xB2,   0x02,   0xFF, 0xFF,
    0x01,   0x90,   0x04,   0x09, 0x23, 0xA1, 0x35,
    0x01,   0x6A,   0x02,   0x06, 0x00,
    0x01,   0x60,   0x08,   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00,   0x60,   0x04,   0x40, 0x00, 0x00, 0x00,
    0x01,   0x62,   0x02,   0x00, 0x00,
    0x00,   0x60,   0x04,   0x00, 0x40, 0x00, 0x00
};

/***********************************spi_soft************************************/
void MPU9250_Write_Reg(uint8 register_address, uint8 data)
{
  MPU9250_CS_HIGH();
  SPI2_SCLK_LOW();
  MPU9250_CS_LOW();
  spi_send_byte(register_address);
  spi_send_byte(data);
  MPU9250_CS_HIGH();
}

uint8 MPU9250_Read_Reg(uint8 register_address)
{
  uint8 data;
  MPU9250_CS_HIGH();
  SPI2_SCLK_LOW();
  MPU9250_CS_LOW();
  spi_send_byte(0x80 | register_address);
  data = spi_receive_byte();
  MPU9250_CS_HIGH();
  return data;
}

void MPU9250_ReadBytes(uint8 REG_Address,uint8 length,uint8 *data)
{
  for(int count=0;count<length;count++)
  {
    data[count]=MPU9250_Read_Reg(REG_Address);
  }
}

void MPU9250_writeBit(uint8 reg, uint8 bitNum, uint8 data)
{
  uint8 b;
  b = MPU9250_Read_Reg(reg);
  b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
  MPU9250_Write_Reg(reg, b);
}
//地磁计
void Mag_write(uint8 reg,uint8 value)
{
  MPU9250_Write_Reg(I2C_SLV0_ADDR ,MPU9250_AK8963_ADDR);//设置磁力计地址,mode: write
  MPU9250_Write_Reg(I2C_SLV0_REG ,reg);//set reg addr
  MPU9250_Write_Reg(I2C_SLV0_DO ,value);//send value	
  Delay_T3_uS(10);//此处因为MPU内部I2C读取速度较慢，延时等待内部写完毕
}

uint8 Mag_read(uint8 reg)
{
  MPU9250_Write_Reg(I2C_SLV0_ADDR ,MPU9250_AK8963_ADDR|0x80); //设置磁力计地址，mode：read
  MPU9250_Write_Reg(I2C_SLV0_REG ,reg);// set reg addr
  MPU9250_Write_Reg(I2C_SLV0_DO ,0xff);//read
  Delay_T3_uS(10);//此处因为MPU内部I2C读取速度较慢，必须延时等待内部读取完毕
  return MPU9250_Read_Reg(EXT_SENS_DATA_00);
}

int16 MPU9250_GetResult_spisoft(uint8 register_address)
{
  uint8 high_data,low_data;
  int16 resultdata = 0;
  high_data = MPU9250_Read_Reg(register_address);
  low_data = MPU9250_Read_Reg(register_address+1);
  resultdata = (high_data << 8) | low_data;
  //resultdata = resultdata>>2;
  return resultdata;
}

//读取DMP FIFO 的值，保存在dmpdatas 数组中
void readdmp(void)
{
  MPU9250_ReadBytes(0x74,42,dmpdatas);
  MPU9250_Write_Reg(MPU9250_RA_USER_CTRL , 0xcc);//FIFO_RST | DMP_RST | DMP_EN | FIFO_EN 
}

long getdmplong(unsigned char address)
{
  long dmptempl;
  dmptempl = dmpdatas[address];
  dmptempl <<= 8;
  dmptempl |= dmpdatas[address+1];
  dmptempl <<= 8;
  dmptempl |= dmpdatas[address+2];
  dmptempl <<= 8;
  dmptempl |= dmpdatas[address+3];
  return dmptempl;
}

/***********************************************************************
*根据FIFO中的值计算四元数，计算yaw,pitch,roll  计算加速度，角速度
************************************************************************/

//计算四元数
void getquaternion(void)
{    
  q[0] = getdmplong(0)/1073741824.0;  
  q[1] = getdmplong(4)/1073741824.0;
  q[2] = getdmplong(8)/1073741824.0;
  q[3] = getdmplong(12)/1073741824.0;
}

//计算yaw、pitch、roll(函数中包含了四元数，无需再调用之前再计算四元数)
void getyawpitchroll(void)
{
  getquaternion();//计算四元数
  
  	//yprf[0]=-atan2(2.0*(q[0]*q[3] + q[1]*q[2]),
  	//                     1 - 2.0*(q[2]*q[2] + q[3]*q[3]))*57.3;  //yaw
  	
  	//yprf[1]=-asin(2.0*(q[0]*q[2] - q[3]*q[1]))*57.3;	    //pitch
  
  	//yprf[2] = atan2(2.0*(q[0]*q[1] + q[2]*q[3]), 1 - 2.0*(q[1]*q[1] + q[2]*q[2]))*57.3; //roll
}

//计算加速度和角速度(有问题)
void getAcc_gyro()
{
  uint8_t buf[20];
  MPU9250_ReadBytes(MPU9250_RA_ACCEL_XOUT_H,14,buf);
  accelf[0]=	((int16_t)((buf[1]<<8)|buf[0]))/2048.0;//acc-x
  accelf[1]=	((int16_t)((buf[3]<<8)|buf[2]))/2048.0;//acc-y
  accelf[2]=	((int16_t)((buf[5]<<8)|buf[4]))/2048.0;//acc-z
  gyrof[0]=	((int16_t)((buf[7]<<8)|buf[6]))/16384.0;//gyro-x
  gyrof[1]=	((int16_t)((buf[9]<<8)|buf[8]))/16384.0;//gyro-y
  gyrof[2]=	((int16_t)((buf[11]<<8)|buf[10]))/16384.0;//gyro-z
  
}
/**********************************************************/

//获取 X/Y/Z 磁力计值
void getmag()
{
  uint8 BUF[10];
  uint8 temp;
  
  //	IICwriteByte(GYRO_ADDRESS,0x37,0x02);//turn on Bypass Mode 
  //	Delayms(30);	
  
  while(1)
  {
    temp = MPU9250_Read_Reg(0x09)&0x08;
    if(temp)
    {		
      MPU9250_Read_Reg (MPU9250_RA_MAG_XOUT_L);
      return;
    }
    else
    {
      MPU9250_ReadBytes(MPU9250_RA_MAG_XOUT_L,6,BUF);
      mag[0]= ((int16_t)(BUF[1]<<8)|BUF[0]) * 0.15;//mag_x
      mag[1]= ((int16_t)(BUF[3]<<8)|BUF[2]) * 0.15;//mag_y
      mag[2]= ((int16_t)(BUF[5]<<8)|BUF[4]) *0.15;//mag_z
      break;
    }
  }
}

void loadfirmware(void)
{
  unsigned int datanum=0;
  unsigned char bank;
  unsigned lastbank=1;
  for (datanum=0;datanum<1929;datanum++)
  {
    bank=datanum/256;
    if(lastbank!=bank)
    {
      MPU9250_Write_Reg(0x6d,bank);
      MPU9250_Write_Reg(0x6e,0);
    }
    
    MPU9250_Write_Reg(0x6f,dmpmemorydata[datanum]);  
    lastbank=bank; 
  }
}

void loadcfgupd(void)
{
  unsigned char line;
  unsigned char bank;
  unsigned char datacounts=0;
  unsigned char bytes2write;
  unsigned char offset;
  unsigned char writingcounts;
  
  for (line=0;line<37;line++)
  {
    bank=dmpcfgupddata[datacounts];
    datacounts++;
    offset=dmpcfgupddata[datacounts];
    datacounts++;
    bytes2write=dmpcfgupddata[datacounts];
    
    MPU9250_Write_Reg(0x6d,bank);  
    MPU9250_Write_Reg(0x6e,offset);
    for (writingcounts=0;writingcounts<bytes2write;writingcounts++)
    {
      datacounts++;
      MPU9250_Write_Reg(0x6f,dmpcfgupddata[datacounts]);  
    }
    datacounts++;
  }
}

void MPU9250_Sleep()
{
  MPU9250_Write_Reg(MPU9250_RA_PWR_MGMT_1, 0x40);//pw1	
}

void MPU9250_WakeUp()
{
  MPU9250_Write_Reg(MPU9250_RA_PWR_MGMT_1, 0x00);//pw1	
}

void MPU9250_resetFIFO(void)
{
  MPU9250_writeBit(MPU9250_RA_USER_CTRL, MPU9250_USERCTRL_FIFO_RESET_BIT, 1);
}

uint16_t MPU9250_getFIFOCount(void) 
{
  uint8_t buffer[2] = {0};
  MPU9250_ReadBytes(MPU9250_RA_FIFO_COUNTH, 2, buffer);
  return (((uint16_t)buffer[0]) << 8) | buffer[1];
}


//MPU9250初始化
void MPU9250_Init_spisoft(void)
{
  MPU9250_Write_Reg(PWR_MGMT_1, 0x00);	//解除休眠状态
  MPU9250_Write_Reg(CONFIG, 0x07);      //低通滤波频率，典型值：0x07(3600Hz)此寄存器内决定Internal_Sample_Rate==8K
  
  /**********************Init SLV0 i2c**********************************/	
  //Use SPI-bus read slave0
  MPU9250_Write_Reg(INT_PIN_CFG ,0x30);// INT Pin / Bypass Enable Configuration  
  MPU9250_Write_Reg(I2C_MST_CTRL,0x4d);//I2C MAster mode and Speed 400 kHz
  MPU9250_Write_Reg(USER_CTRL ,0x20); // I2C_MST _EN 
  MPU9250_Write_Reg(I2C_MST_DELAY_CTRL ,0x01);//延时使能I2C_SLV0 _DLY_ enable 	
  MPU9250_Write_Reg(I2C_SLV0_CTRL ,0x81); //enable IIC	and EXT_SENS_DATA==1 Byte
  
  /*******************Init GYRO and ACCEL******************************/	
  MPU9250_Write_Reg(SMPLRT_DIV, 0x07);  //陀螺仪采样率，典型值：0x07(1kHz) (SAMPLE_RATE= Internal_Sample_Rate / (1 + SMPLRT_DIV) )
  MPU9250_Write_Reg(GYRO_CONFIG, 0x10); //陀螺仪自检及测量范围，典型值：0x00(250eg/s) ; 0x08(500eg/s);0x10(1000eg/s);0x18(2000eg/s)
  MPU9250_Write_Reg(ACCEL_CONFIG, 0x08);//加速计自检、测量范围及高通滤波频率，典型值：0x18(不自检，16G) 0x10(8g) 0x08(4g) 0x00(2g)
  MPU9250_Write_Reg(ACCEL_CONFIG_2, 0x08);//加速计高通滤波频率 典型值 ：0x08  （460Hz）	
  
  /**********************Init MAG **********************************/
  Mag_write(AK8963_CNTL2_REG,AK8963_CNTL2_SRST); // Reset AK8963
  Mag_write(AK8963_CNTL1_REG,0x12); // use i2c to set AK8963 working on Continuous measurement mode1 & 16-bit output	
  
  MPU9250_Write_Reg(MPU9250_RA_FIFO_EN, 0xFF);//FIFO使能	
  Delay_T3_mS(10);
  
  /***********************DMP控制**********************************/
  MPU9250_Write_Reg(MPU9250_RA_PWR_MGMT_1, 0x80);//pw1	
  Delay_T3_mS(100);
  MPU9250_Write_Reg(MPU9250_RA_PWR_MGMT_1, 0x00);//pw1	
  MPU9250_Write_Reg(MPU9250_RA_PWR_MGMT_2, 0x00);//pw2	
  
  MPU9250_Write_Reg(MPU9250_RA_SMPLRT_DIV, 0x04);//1000/5 = 200hz
  MPU9250_Write_Reg(MPU9250_RA_CONFIG, 0x06); //1KHZ
  MPU9250_Write_Reg(MPU9250_RA_GYRO_CONFIG, 0x18);//+- 2000
  MPU9250_Write_Reg(MPU9250_RA_ACCEL_CONFIG, 0x10);//+- 4g()
  
  MPU9250_Write_Reg(MPU9250_RA_INT_PIN_CFG,0x00); //中断引脚设置
  MPU9250_Write_Reg(MPU9250_RA_INT_ENABLE,0x01);  //FIFO中断使能
  
  loadfirmware(); 
  loadcfgupd();
  
  MPU9250_Write_Reg(MPU9250_RA_DMP_CFG_1 , 0x03);                         
  MPU9250_Write_Reg(MPU9250_RA_DMP_CFG_2 , 0x00);                         
  MPU9250_Write_Reg(MPU9250_RA_USER_CTRL , 0xcc);  
}

/*******************************************************************************/
