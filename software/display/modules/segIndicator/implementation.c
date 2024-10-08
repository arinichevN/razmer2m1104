#include "interface.h"

#define SEGINDICATOR_FIRST_CHAR					' '
#define SEGINDICATOR_LAST_CHAR					'z'

const static uint8_t asciiToSegment [] = {
	0b00000000, // 032
	0b00000110, // 033
	0b00100010, // 034
	0b01110110, // 035
	0b01101101, // 036
	0b00100100, // 037
	0b01111111, // 038
	0b00100000, // 039
	0b00111001, // 040
	0b00001111, // 041
	0b01011100, // 042
	0b00000000, // 043 + 0b01010000
	0b00010000, // 044
	0b01000000, // 045 -
	0b00001000, // 046
	0b00000110, // 047
	0b00111111, // 048 0
	0b00000110, // 049 1
	0b01011011, // 050 2
	0b01001111, // 051 3
	0b01100110, // 052 4
	0b01101101, // 053 5
	0b01111101, // 054 6
	0b00000111, // 055 7
	0b01111111, // 056 8
	0b01101111, // 057 9
	0b00000000, // 058 :
	0b00000000, // 059 ;
	0b00000000, // 060 <
	0b01001000, // 061 =
	0b00000000, // 062 >
	0b00000000, // 063 ?
	0b00000000, // 064 @
	0b01110111, // 065 A
	0b01111100, // 066 B
	0b00111001, // 067 C
	0b01011110, // 068 D
	0b01111001, // 069 E
	0b01110001, // 070 F
	0b00111101, // 071 G
	0b01110110, // 072 H
	0b00000110, // 073 I
	0b00011110, // 074 J
	0b01110110, // 075 K
	0b00111000, // 076 L
	0b00010101, // 077 M
	0b00110111, // 078 N
	0b00111111, // 079 O
	0b01110011, // 080 P
	0b01100111, // 081 Q
	0b00110001, // 082 R
	0b01101101, // 083 S
	0b01111000, // 084 T
	0b00111110, // 085 U
	0b00011100, // 086 V
	0b00101010, // 087 W
	0b01110110, // 088 X
	0b01101110, // 089 Y
	0b01011011, // 090 Z
	0b00000000, // 091 [
	0b01100100, // 092 (backslash)
	0b00000000, // 093 ]
	0b00000000, // 094 ^
	0b00000000, // 095 _
	0b00000000, // 096 `
	0b01110111, // 097 a
	0b01111100, // 098 b
	0b01011000, // 099 c
	0b01011110, // 100 d
	0b01111001, // 101 e
	0b01110001, // 102 f
	0b01101111, // 103 g
	0b01110100, // 104 h
	0b00000100, // 105 i
	0b00011110, // 106 j
	0b01110110, // 107 k
	0b00011000, // 108 l
	0b00010101, // 109 m
	0b01010100, // 110 n
	0b01011100, // 111 o
	0b01110011, // 112 p
	0b01100111, // 113 q
	0b01010000, // 114 r
	0b01101101, // 115 s
	0b01111000, // 116 t
	0b00011100, // 117 u
	0b00011100, // 118 v
	0b00101010, // 119 w
	0b01110110, // 120 x
	0b01101110, // 121 y
	0b01011011 // 122 z
};

static uint8_t swapSignBits(uint8_t v){
	uint8_t out = 0;
	for(int i = 0; i < 7; i++){
		 if(v & (1 << i)){
			 out |= (0b01000000 >> i);
		 }
	}
	return out;
}

uint8_t segIndicator_encodeASCIISwap(uint8_t v){
	if(v > SEGINDICATOR_LAST_CHAR || v < SEGINDICATOR_FIRST_CHAR) return 0;
	return swapSignBits(asciiToSegment[v - SEGINDICATOR_FIRST_CHAR]);
}

uint8_t segIndicator_setDot(uint8_t v){
	return v | 0b10000000;
}

