#include "fonts.h"

// clang-format off
const uint8_t Icons_Table[] = {
  // camera
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X7F, 0XFC, 0X00, 0X00,
  0X00, 0X00, 0XFF, 0XFE, 0X00, 0X00,
  0X07, 0XC1, 0XFF, 0XFF, 0X00, 0X00,
  0X04, 0X43, 0XFF, 0XFF, 0X80, 0X00,
  0X04, 0X47, 0XFF, 0XFF, 0XC0, 0X00,
  0X04, 0X4F, 0XFF, 0XFF, 0XE0, 0X00,
  0X3F, 0XFF, 0XFF, 0XFF, 0XFF, 0XF8,
  0X7F, 0XFF, 0XFF, 0XFF, 0XFF, 0XFC,
  0XFF, 0XFF, 0XE0, 0X0F, 0XFF, 0XFE,
  0XFF, 0XFF, 0X80, 0X03, 0XFF, 0XFE,
  0XFF, 0XFF, 0X00, 0X01, 0XFF, 0XFE,
  0XFF, 0XFE, 0X00, 0X00, 0XFF, 0XFE,
  0XFF, 0XFC, 0X00, 0X00, 0X7F, 0XFE,
  0XFF, 0XFC, 0X00, 0X00, 0X7F, 0XFE,
  0XFF, 0XF8, 0X00, 0X00, 0X3F, 0XFE,
  0XFF, 0XF8, 0X00, 0X00, 0X3F, 0XFE,
  0XFF, 0XF8, 0X00, 0X00, 0X3F, 0XFE,
  0XFF, 0XF8, 0X00, 0X00, 0X3F, 0XFE,
  0XFF, 0XF8, 0X00, 0X00, 0X3F, 0XFE,
  0XFF, 0XF8, 0X00, 0X00, 0X3F, 0XFE,
  0XFF, 0XF8, 0X00, 0X00, 0X3F, 0XFE,
  0XFF, 0XF8, 0X00, 0X00, 0X3F, 0XFE,
  0XFF, 0XF8, 0X00, 0X00, 0X3F, 0XFE,
  0XFF, 0XFC, 0X00, 0X00, 0X7F, 0XFE,
  0XFF, 0XFC, 0X00, 0X00, 0X7F, 0XFE,
  0XFF, 0XFE, 0X00, 0X00, 0XFF, 0XFE,
  0XFF, 0XFF, 0X00, 0X01, 0XFF, 0XFE,
  0XFF, 0XFF, 0X80, 0X03, 0XFF, 0XFE,
  0XFF, 0XFF, 0XE0, 0X0F, 0XFF, 0XFE,
  0X7F, 0XFF, 0XFF, 0XFF, 0XFF, 0XFC,
  0X3F, 0XFF, 0XFF, 0XFF, 0XFF, 0XF8,

  // camera_pressed
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X7F, 0XFC, 0X00, 0X00,
  0X00, 0X00, 0XFF, 0XFE, 0X00, 0X00,
  0X00, 0X01, 0XFF, 0XFF, 0X00, 0X00,
  0X00, 0X03, 0XFF, 0XFF, 0X80, 0X00,
  0X07, 0XC7, 0XFF, 0XFF, 0XC0, 0X00,
  0X04, 0X4F, 0XFF, 0XFF, 0XE0, 0X00,
  0X3F, 0XFF, 0XFF, 0XFF, 0XFF, 0XF8,
  0X7F, 0XFF, 0XFF, 0XFF, 0XFF, 0XFC,
  0XFF, 0XFF, 0XE0, 0X0F, 0XFF, 0XFE,
  0XFF, 0XFF, 0X80, 0X03, 0XFF, 0XFE,
  0XFF, 0XFF, 0X00, 0X01, 0XFF, 0XFE,
  0XFF, 0XFE, 0X00, 0X00, 0XFF, 0XFE,
  0XFF, 0XFC, 0X00, 0X00, 0X7F, 0XFE,
  0XFF, 0XFC, 0X00, 0X00, 0X7F, 0XFE,
  0XFF, 0XF8, 0X00, 0X00, 0X3F, 0XFE,
  0XFF, 0XF8, 0X00, 0X00, 0X3F, 0XFE,
  0XFF, 0XF8, 0X00, 0X00, 0X3F, 0XFE,
  0XFF, 0XF8, 0X00, 0X00, 0X3F, 0XFE,
  0XFF, 0XF8, 0X00, 0X00, 0X3F, 0XFE,
  0XFF, 0XF8, 0X00, 0X00, 0X3F, 0XFE,
  0XFF, 0XF8, 0X00, 0X00, 0X3F, 0XFE,
  0XFF, 0XF8, 0X00, 0X00, 0X3F, 0XFE,
  0XFF, 0XF8, 0X00, 0X00, 0X3F, 0XFE,
  0XFF, 0XFC, 0X00, 0X00, 0X7F, 0XFE,
  0XFF, 0XFC, 0X00, 0X00, 0X7F, 0XFE,
  0XFF, 0XFE, 0X00, 0X00, 0XFF, 0XFE,
  0XFF, 0XFF, 0X00, 0X01, 0XFF, 0XFE,
  0XFF, 0XFF, 0X80, 0X03, 0XFF, 0XFE,
  0XFF, 0XFF, 0XE0, 0X0F, 0XFF, 0XFE,
  0X7F, 0XFF, 0XFF, 0XFF, 0XFF, 0XFC,
  0X3F, 0XFF, 0XFF, 0XFF, 0XFF, 0XF8,

  // camera_small_flash
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X7F, 0XFC, 0X00, 0X00,
  0X00, 0X00, 0XFF, 0XFE, 0X00, 0X00,
  0X00, 0X01, 0XFF, 0XFF, 0X00, 0X00,
  0X00, 0X03, 0XFF, 0XFF, 0X80, 0X00,
  0X07, 0XC7, 0XFF, 0XFF, 0XC0, 0X00,
  0X04, 0X4F, 0XFF, 0XFF, 0XE0, 0X00,
  0X3F, 0XFF, 0XFF, 0XFF, 0XFF, 0XF8,
  0X7F, 0XFF, 0XFF, 0XFF, 0XFF, 0XFC,
  0XFF, 0XFF, 0XE0, 0X0F, 0XFF, 0XFE,
  0XFF, 0XFF, 0X80, 0X03, 0XFF, 0XFE,
  0XFF, 0XFF, 0X00, 0X01, 0XFF, 0XFE,
  0XFF, 0XFE, 0X00, 0X00, 0XFF, 0XFE,
  0XFF, 0XFC, 0X00, 0X00, 0X7F, 0XFE,
  0XFF, 0XFC, 0X00, 0X00, 0X7F, 0XFE,
  0XFF, 0XF8, 0X00, 0X00, 0X3F, 0XFE,
  0XFF, 0XF8, 0X01, 0X00, 0X3F, 0XFE,
  0XFF, 0XF8, 0X01, 0X00, 0X3F, 0XFE,
  0XFF, 0XF8, 0X03, 0X80, 0X3F, 0XFE,
  0XFF, 0XF8, 0X0F, 0XE0, 0X3F, 0XFE,
  0XFF, 0XF8, 0X03, 0X80, 0X3F, 0XFE,
  0XFF, 0XF8, 0X01, 0X00, 0X3F, 0XFE,
  0XFF, 0XF8, 0X01, 0X00, 0X3F, 0XFE,
  0XFF, 0XF8, 0X00, 0X00, 0X3F, 0XFE,
  0XFF, 0XFC, 0X00, 0X00, 0X7F, 0XFE,
  0XFF, 0XFC, 0X00, 0X00, 0X7F, 0XFE,
  0XFF, 0XFE, 0X00, 0X00, 0XFF, 0XFE,
  0XFF, 0XFF, 0X00, 0X01, 0XFF, 0XFE,
  0XFF, 0XFF, 0X80, 0X03, 0XFF, 0XFE,
  0XFF, 0XFF, 0XE0, 0X0F, 0XFF, 0XFE,
  0X7F, 0XFF, 0XFF, 0XFF, 0XFF, 0XFC,
  0X3F, 0XFF, 0XFF, 0XFF, 0XFF, 0XF8,

  // camera_big_flash
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X7F, 0XFC, 0X00, 0X00,
  0X00, 0X00, 0XFF, 0XFE, 0X00, 0X00,
  0X00, 0X01, 0XFF, 0XFF, 0X00, 0X00,
  0X00, 0X03, 0XFF, 0XFF, 0X80, 0X00,
  0X07, 0XC7, 0XFF, 0XFF, 0XC0, 0X00,
  0X04, 0X4F, 0XFF, 0XFF, 0XE0, 0X00,
  0X3F, 0XFF, 0XFF, 0XFF, 0XFF, 0XF8,
  0X7F, 0XFF, 0XFF, 0XFF, 0XFF, 0XFC,
  0XFF, 0XFF, 0XE0, 0X0F, 0XFF, 0XFE,
  0XFF, 0XFF, 0X80, 0X03, 0XFF, 0XFE,
  0XFF, 0XFF, 0X00, 0X01, 0XFF, 0XFE,
  0XFF, 0XFE, 0X00, 0X00, 0XFF, 0XFE,
  0XFF, 0XFC, 0X01, 0X00, 0X7F, 0XFE,
  0XFF, 0XFC, 0X01, 0X00, 0X7F, 0XFE,
  0XFF, 0XF8, 0X01, 0X00, 0X3F, 0XFE,
  0XFF, 0XF8, 0X03, 0X80, 0X3F, 0XFE,
  0XFF, 0XF8, 0X03, 0X80, 0X3F, 0XFE,
  0XFF, 0XF8, 0X0F, 0XE0, 0X3F, 0XFE,
  0XFF, 0XF8, 0X7F, 0XFC, 0X3F, 0XFE,
  0XFF, 0XF8, 0X0F, 0XE0, 0X3F, 0XFE,
  0XFF, 0XF8, 0X03, 0X80, 0X3F, 0XFE,
  0XFF, 0XF8, 0X03, 0X80, 0X3F, 0XFE,
  0XFF, 0XF8, 0X01, 0X00, 0X3F, 0XFE,
  0XFF, 0XFC, 0X01, 0X00, 0X7F, 0XFE,
  0XFF, 0XFC, 0X01, 0X00, 0X7F, 0XFE,
  0XFF, 0XFE, 0X00, 0X00, 0XFF, 0XFE,
  0XFF, 0XFF, 0X00, 0X01, 0XFF, 0XFE,
  0XFF, 0XFF, 0X80, 0X03, 0XFF, 0XFE,
  0XFF, 0XFF, 0XE0, 0X0F, 0XFF, 0XFE,
  0X7F, 0XFF, 0XFF, 0XFF, 0XFF, 0XFC,
  0X3F, 0XFF, 0XFF, 0XFF, 0XFF, 0XF8,

  // wrench
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X03, 0XFE, 0X00, 0X00, 0X00, 0X00,
  0X0F, 0XFF, 0X80, 0X00, 0X00, 0X00,
  0X1F, 0XFF, 0XC0, 0X00, 0X00, 0X00,
  0X1F, 0XFF, 0XE0, 0X00, 0X00, 0X00,
  0X03, 0XFF, 0XF0, 0X00, 0X00, 0X00,
  0X00, 0X7F, 0XF0, 0X00, 0X00, 0X00,
  0X00, 0X0F, 0XF8, 0X00, 0X00, 0X00,
  0X00, 0X0F, 0XF8, 0X00, 0X00, 0X00,
  0X00, 0X07, 0XF8, 0X00, 0X00, 0X00,
  0X00, 0X07, 0XF8, 0X00, 0X00, 0X00,
  0X00, 0X07, 0XFF, 0X00, 0X00, 0X00,
  0X00, 0X07, 0XFF, 0XE0, 0X00, 0X00,
  0X80, 0X0F, 0XFF, 0XFC, 0X00, 0X00,
  0XF0, 0X1F, 0XFF, 0XFF, 0X80, 0X00,
  0XFE, 0X3F, 0XFF, 0XFF, 0XF0, 0X00,
  0X7F, 0XFF, 0XFF, 0XFF, 0XFE, 0X00,
  0X7F, 0XFF, 0XFF, 0XFF, 0XFF, 0XC0,
  0X3F, 0XFF, 0XFF, 0XFF, 0XFF, 0XF0,
  0X1F, 0XFF, 0XDF, 0XFF, 0XFF, 0XF8,
  0X0F, 0XFF, 0X83, 0XFF, 0XFC, 0X78,
  0X03, 0XFE, 0X00, 0X7F, 0XF8, 0X38,
  0X00, 0X00, 0X00, 0X0F, 0XF8, 0X38,
  0X00, 0X00, 0X00, 0X01, 0XFC, 0X78,
  0X00, 0X00, 0X00, 0X00, 0X3F, 0XF0,
  0X00, 0X00, 0X00, 0X00, 0X07, 0XE0,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,

  // wrench_middle
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X03, 0XFE, 0X00, 0X00, 0X00, 0X00,
  0X0F, 0XFF, 0X80, 0X00, 0X00, 0X00,
  0X1F, 0XFF, 0XC0, 0X00, 0X00, 0X00,
  0X3F, 0XFF, 0XE0, 0X00, 0X00, 0X00,
  0X3F, 0XFF, 0XF0, 0X00, 0X00, 0X00,
  0X3F, 0XFF, 0XF0, 0X00, 0X00, 0X00,
  0X00, 0X3F, 0XFF, 0XFF, 0XFF, 0XFC,
  0X00, 0X1F, 0XFF, 0XFF, 0XFF, 0XFE,
  0X00, 0X0F, 0XFF, 0XFF, 0XFF, 0XC7,
  0X00, 0X07, 0XFF, 0XFF, 0XFF, 0X83,
  0X00, 0X07, 0XFF, 0XFF, 0XFF, 0X83,
  0X00, 0X07, 0XFF, 0XFF, 0XFF, 0X83,
  0X00, 0X0F, 0XFF, 0XFF, 0XFF, 0XC7,
  0X00, 0X1F, 0XFF, 0XFF, 0XFF, 0XFE,
  0X00, 0X3F, 0XFF, 0XFF, 0XFF, 0XFC,
  0X3F, 0XFF, 0XF0, 0X00, 0X00, 0X00,
  0X3F, 0XFF, 0XF0, 0X00, 0X00, 0X00,
  0X3F, 0XFF, 0XE0, 0X00, 0X00, 0X00,
  0X1F, 0XFF, 0XC0, 0X00, 0X00, 0X00,
  0X0F, 0XFF, 0X80, 0X00, 0X00, 0X00,
  0X03, 0XFE, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,

  // wrench_up
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X07, 0XE0,
  0X00, 0X00, 0X00, 0X00, 0X3F, 0XF0,
  0X00, 0X00, 0X00, 0X01, 0XFC, 0X78,
  0X00, 0X00, 0X00, 0X0F, 0XF8, 0X38,
  0X03, 0XFE, 0X00, 0X7F, 0XF8, 0X38,
  0X0F, 0XFF, 0X83, 0XFF, 0XFC, 0X78,
  0X1F, 0XFF, 0XDF, 0XFF, 0XFF, 0XF8,
  0X3F, 0XFF, 0XFF, 0XFF, 0XFF, 0XF0,
  0X7F, 0XFF, 0XFF, 0XFF, 0XFF, 0XC0,
  0X7F, 0XFF, 0XFF, 0XFF, 0XFE, 0X00,
  0XFE, 0X3F, 0XFF, 0XFF, 0XF0, 0X00,
  0XF0, 0X1F, 0XFF, 0XFF, 0X80, 0X00,
  0X80, 0X0F, 0XFF, 0XFC, 0X00, 0X00,
  0X00, 0X07, 0XFF, 0XE0, 0X00, 0X00,
  0X00, 0X07, 0XFF, 0X00, 0X00, 0X00,
  0X00, 0X07, 0XF8, 0X00, 0X00, 0X00,
  0X00, 0X07, 0XF8, 0X00, 0X00, 0X00,
  0X00, 0X0F, 0XF8, 0X00, 0X00, 0X00,
  0X00, 0X0F, 0XF8, 0X00, 0X00, 0X00,
  0X00, 0X7F, 0XF0, 0X00, 0X00, 0X00,
  0X03, 0XFF, 0XF0, 0X00, 0X00, 0X00,
  0X1F, 0XFF, 0XE0, 0X00, 0X00, 0X00,
  0X1F, 0XFF, 0XC0, 0X00, 0X00, 0X00,
  0X0F, 0XFF, 0X80, 0X00, 0X00, 0X00,
  0X03, 0XFE, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,

  // wranch_middle_2
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X03, 0XFE, 0X00, 0X00, 0X00, 0X00,
  0X0F, 0XFF, 0X80, 0X00, 0X00, 0X00,
  0X1F, 0XFF, 0XC0, 0X00, 0X00, 0X00,
  0X3F, 0XFF, 0XE0, 0X00, 0X00, 0X00,
  0X3F, 0XFF, 0XF0, 0X00, 0X00, 0X00,
  0X3F, 0XFF, 0XF0, 0X00, 0X00, 0X00,
  0X00, 0X3F, 0XFF, 0XFF, 0XFF, 0XFC,
  0X00, 0X1F, 0XFF, 0XFF, 0XFF, 0XFE,
  0X00, 0X0F, 0XFF, 0XFF, 0XFF, 0XC7,
  0X00, 0X07, 0XFF, 0XFF, 0XFF, 0X83,
  0X00, 0X07, 0XFF, 0XFF, 0XFF, 0X83,
  0X00, 0X07, 0XFF, 0XFF, 0XFF, 0X83,
  0X00, 0X0F, 0XFF, 0XFF, 0XFF, 0XC7,
  0X00, 0X1F, 0XFF, 0XFF, 0XFF, 0XFE,
  0X00, 0X3F, 0XFF, 0XFF, 0XFF, 0XFC,
  0X3F, 0XFF, 0XF0, 0X00, 0X00, 0X00,
  0X3F, 0XFF, 0XF0, 0X00, 0X00, 0X00,
  0X3F, 0XFF, 0XE0, 0X00, 0X00, 0X00,
  0X1F, 0XFF, 0XC0, 0X00, 0X00, 0X00,
  0X0F, 0XFF, 0X80, 0X00, 0X00, 0X00,
  0X03, 0XFE, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00,

  // cloud
  0X00, 0X00, 0X1F, 0XFC, 0X00, 0X00,
  0X00, 0X00, 0X7F, 0XFF, 0X00, 0X00,
  0X00, 0X00, 0XFF, 0XFF, 0X80, 0X00,
  0X00, 0X01, 0XFF, 0XFF, 0XC0, 0X00,
  0X00, 0X03, 0XFF, 0XFF, 0XE0, 0X00,
  0X00, 0X03, 0XFF, 0XFF, 0XE0, 0X00,
  0X00, 0X07, 0XFF, 0XFF, 0XF0, 0X00,
  0X00, 0X07, 0XFF, 0XFF, 0XF0, 0X00,
  0X00, 0X07, 0XFF, 0XFF, 0XF0, 0X00,
  0X00, 0X07, 0XFF, 0XFF, 0XF0, 0X00,
  0X00, 0X07, 0XFF, 0XFF, 0XF0, 0X00,
  0X03, 0XFF, 0XFF, 0XFF, 0XF0, 0X00,
  0X0F, 0XFF, 0XFF, 0XFF, 0XF0, 0X00,
  0X1F, 0XFF, 0XFF, 0XFF, 0XFF, 0X80,
  0X3F, 0XFF, 0XFF, 0XFF, 0XFF, 0XE0,
  0X7F, 0XFF, 0XFF, 0X7F, 0XFF, 0XF0,
  0X7F, 0XFF, 0XFE, 0X3F, 0XFF, 0XF8,
  0XFF, 0XFF, 0XFC, 0X1F, 0XFF, 0XFC,
  0XFF, 0XFF, 0XF8, 0X0F, 0XFF, 0XFC,
  0XFF, 0XFF, 0XF0, 0X07, 0XFF, 0XFE,
  0XFF, 0XFF, 0XE0, 0X03, 0XFF, 0XFE,
  0XFF, 0XFF, 0XC0, 0X01, 0XFF, 0XFE,
  0XFF, 0XFF, 0XF8, 0X0F, 0XFF, 0XFE,
  0XFF, 0XFF, 0XF8, 0X0F, 0XFF, 0XFE,
  0XFF, 0XFF, 0XF8, 0X0F, 0XFF, 0XFE,
  0XFF, 0XFF, 0XF8, 0X0F, 0XFF, 0XFE,
  0X7F, 0XFF, 0XF8, 0X0F, 0XFF, 0XFC,
  0X7F, 0XFF, 0XF8, 0X0F, 0XFF, 0XFC,
  0X3F, 0XFF, 0XF8, 0X0F, 0XFF, 0XF8,
  0X1F, 0XFF, 0XF8, 0X0F, 0XFF, 0XF0,
  0X0F, 0XFF, 0XF8, 0X0F, 0XFF, 0XE0,
  0X03, 0XFF, 0XF8, 0X0F, 0XFF, 0X80,

  // cloud_filled
  0X00, 0X00, 0X1F, 0XFC, 0X00, 0X00,
  0X00, 0X00, 0X7F, 0XFF, 0X00, 0X00,
  0X00, 0X00, 0XFF, 0XFF, 0X80, 0X00,
  0X00, 0X01, 0XFF, 0XFF, 0XC0, 0X00,
  0X00, 0X03, 0XFF, 0XFF, 0XE0, 0X00,
  0X00, 0X03, 0XFF, 0XFF, 0XE0, 0X00,
  0X00, 0X07, 0XFF, 0XFF, 0XF0, 0X00,
  0X00, 0X07, 0XFF, 0XFF, 0XF0, 0X00,
  0X00, 0X07, 0XFF, 0XFF, 0XF0, 0X00,
  0X00, 0X07, 0XFF, 0XFF, 0XF0, 0X00,
  0X00, 0X07, 0XFF, 0XFF, 0XF0, 0X00,
  0X03, 0XFF, 0XFF, 0XFF, 0XF0, 0X00,
  0X0F, 0XFF, 0XFF, 0XFF, 0XF0, 0X00,
  0X1F, 0XFF, 0XFF, 0XFF, 0XFF, 0X80,
  0X3F, 0XFF, 0XFF, 0XFF, 0XFF, 0XE0,
  0X7F, 0XFF, 0XFF, 0XFF, 0XFF, 0XF0,
  0X7F, 0XFF, 0XFF, 0XFF, 0XFF, 0XF8,
  0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFC,
  0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFC,
  0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFE,
  0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFE,
  0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFE,
  0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFE,
  0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFE,
  0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFE,
  0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFE,
  0X7F, 0XFF, 0XFF, 0XFF, 0XFF, 0XFC,
  0X7F, 0XFF, 0XFF, 0XFF, 0XFF, 0XFC,
  0X3F, 0XFF, 0XFF, 0XFF, 0XFF, 0XF8,
  0X1F, 0XFF, 0XFF, 0XFF, 0XFF, 0XF0,
  0X0F, 0XFF, 0XFF, 0XFF, 0XFF, 0XE0,
  0X03, 0XFF, 0XFF, 0XFF, 0XFF, 0X80,

  // cloud_short_arrow
  0X00, 0X00, 0X1F, 0XFC, 0X00, 0X00,
  0X00, 0X00, 0X7F, 0XFF, 0X00, 0X00,
  0X00, 0X00, 0XFF, 0XFF, 0X80, 0X00,
  0X00, 0X01, 0XFF, 0XFF, 0XC0, 0X00,
  0X00, 0X03, 0XFF, 0XFF, 0XE0, 0X00,
  0X00, 0X03, 0XFF, 0XFF, 0XE0, 0X00,
  0X00, 0X07, 0XFF, 0XFF, 0XF0, 0X00,
  0X00, 0X07, 0XFF, 0XFF, 0XF0, 0X00,
  0X00, 0X07, 0XFF, 0XFF, 0XF0, 0X00,
  0X00, 0X07, 0XFF, 0XFF, 0XF0, 0X00,
  0X00, 0X07, 0XFF, 0XFF, 0XF0, 0X00,
  0X03, 0XFF, 0XFF, 0XFF, 0XF0, 0X00,
  0X0F, 0XFF, 0XFF, 0XFF, 0XF0, 0X00,
  0X1F, 0XFF, 0XFF, 0XFF, 0XFF, 0X80,
  0X3F, 0XFF, 0XFF, 0XFF, 0XFF, 0XE0,
  0X7F, 0XFF, 0XFF, 0XFF, 0XFF, 0XF0,
  0X7F, 0XFF, 0XFF, 0XFF, 0XFF, 0XF8,
  0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFC,
  0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFC,
  0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFE,
  0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFE,
  0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFE,
  0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFE,
  0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFE,
  0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFE,
  0XFF, 0XFF, 0XFF, 0X7F, 0XFF, 0XFE,
  0X7F, 0XFF, 0XFE, 0X3F, 0XFF, 0XFC,
  0X7F, 0XFF, 0XFC, 0X1F, 0XFF, 0XFC,
  0X3F, 0XFF, 0XF8, 0X0F, 0XFF, 0XF8,
  0X1F, 0XFF, 0XF0, 0X07, 0XFF, 0XF0,
  0X0F, 0XFF, 0XE0, 0X03, 0XFF, 0XE0,
  0X03, 0XFF, 0XC0, 0X01, 0XFF, 0X80,

  // cloud_medium_arrow
  0X00, 0X00, 0X1F, 0XFC, 0X00, 0X00,
  0X00, 0X00, 0X7F, 0XFF, 0X00, 0X00,
  0X00, 0X00, 0XFF, 0XFF, 0X80, 0X00,
  0X00, 0X01, 0XFF, 0XFF, 0XC0, 0X00,
  0X00, 0X03, 0XFF, 0XFF, 0XE0, 0X00,
  0X00, 0X03, 0XFF, 0XFF, 0XE0, 0X00,
  0X00, 0X07, 0XFF, 0XFF, 0XF0, 0X00,
  0X00, 0X07, 0XFF, 0XFF, 0XF0, 0X00,
  0X00, 0X07, 0XFF, 0XFF, 0XF0, 0X00,
  0X00, 0X07, 0XFF, 0XFF, 0XF0, 0X00,
  0X00, 0X07, 0XFF, 0XFF, 0XF0, 0X00,
  0X03, 0XFF, 0XFF, 0XFF, 0XF0, 0X00,
  0X0F, 0XFF, 0XFF, 0XFF, 0XF0, 0X00,
  0X1F, 0XFF, 0XFF, 0XFF, 0XFF, 0X80,
  0X3F, 0XFF, 0XFF, 0XFF, 0XFF, 0XE0,
  0X7F, 0XFF, 0XFF, 0XFF, 0XFF, 0XF0,
  0X7F, 0XFF, 0XFF, 0XFF, 0XFF, 0XF8,
  0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFC,
  0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFC,
  0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFE,
  0XFF, 0XFF, 0XFF, 0X7F, 0XFF, 0XFE,
  0XFF, 0XFF, 0XFE, 0X3F, 0XFF, 0XFE,
  0XFF, 0XFF, 0XFC, 0X1F, 0XFF, 0XFE,
  0XFF, 0XFF, 0XF8, 0X0F, 0XFF, 0XFE,
  0XFF, 0XFF, 0XF0, 0X07, 0XFF, 0XFE,
  0XFF, 0XFF, 0XE0, 0X03, 0XFF, 0XFE,
  0X7F, 0XFF, 0XC0, 0X01, 0XFF, 0XFC,
  0X7F, 0XFF, 0XF8, 0X0F, 0XFF, 0XFC,
  0X3F, 0XFF, 0XF8, 0X0F, 0XFF, 0XF8,
  0X1F, 0XFF, 0XF8, 0X0F, 0XFF, 0XF0,
  0X0F, 0XFF, 0XF8, 0X0F, 0XFF, 0XE0,
  0X03, 0XFF, 0XF8, 0X0F, 0XFF, 0X80,
};

sFONT Icons = {
  Icons_Table,
  48, /* Width */
  32, /* Height */
};
// clang-format on
