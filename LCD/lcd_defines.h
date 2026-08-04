#define LCD_DATA 10        // Starting bit position of LCD data pins (D10–D17)
#define RS 8              // Register Select pin connected to P0.8
#define RW 18             // Read/Write control pin connected to P0.18
#define EN 9              // Enable pin connected to P0.9

#define GOTO_LINE1_POS0  0x80 // LCD command to move cursor to line 1, position 0
#define GOTO_LINE2_POS0  0xC0 // LCD command to move cursor to line 2, position 0
#define GOTO_LINE3_POS0  0x94 // LCD command to move cursor to line 3, position 0
#define GOTO_LINE4_POS0  0xD4 // LCD command to move cursor to line 4, position 0
#define CLEAR_LCD  0x01   // LCD command to clear the display screen
