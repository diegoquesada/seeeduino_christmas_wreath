#pragma once

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} Color_t;

const Color_t BLACK = { 0, 0, 0 };
const Color_t RED = { 0xFF, 0, 0 };
const Color_t RED75 = { 0xC0, 0, 0 };
const Color_t RED50 = { 0x80, 0, 0 };
const Color_t RED25 = { 0x40, 0, 0 };
const Color_t GREEN = { 0, 0xFF, 0 };
const Color_t GREEN75 = { 0, 0xC0, 0 };
const Color_t GREEN50 = { 0, 0x80, 0 };
const Color_t GREEN25 = { 0, 0x40, 0 };
const Color_t BLUE = { 0, 0, 0xFF };
const Color_t YELLOW = { 0xFF, 0xFF, 0 };
const Color_t YELLOW75 = { 0xC0, 0xC0, 0 };
const Color_t YELLOW50 = { 0x80, 0x80, 0 };
const Color_t YELLOW25 = { 0x40, 0x40, 0 };
const Color_t WHITE = { 0xFF, 0xFF, 0xFF };

constexpr Color_t RGBtoColor(uint32_t rgb) { return { (rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF }; }
