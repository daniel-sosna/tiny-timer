#pragma once

#include <Arduino.h>

//! Constant lookup table
extern const uint8_t ASCIIto7Seg[78];

/**
 * Encode ASCII char to 7-segment code. Invalid characters are mapped to 0x00.
 *
 * @param chr Character to encode
 * @return Segment byte
 */
uint8_t encodeChar(uint8_t chr);

/**
 * Converts a string to a segment array using ASCIIto7Seg.
 *
 * @param str Input string
 * @param chars Output array of segment values (must be pre-allocated)
 */
void stringToSegmentsArray(const String& str, uint8_t* chars);
