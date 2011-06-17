// This is currently fixed. Don't change.
#define CHAR_WIDTH   4
#define CHAR_HEIGHT  8

#define TABLE_WIDTH  (CHAR_WIDTH * 16)
#define NUM_COLS     (RESOL_X / CHAR_WIDTH)
#define NUM_LINES    (RESOL_Y / CHAR_HEIGHT)

extern
__data unsigned char g_chartbl_offs[3];

extern char __pdata g_printbuf[];

// blit.s:
void blit_char4x8(unsigned char c);

void term_init(void) __banked;
void term_selfont(unsigned char which) __banked;
void disp_home(void) __banked;
void clr_line(unsigned char n) __banked;
void guard_term(void);
void rawputc(const char c);
void putc(const char c);
void putchar(char c) __banked;
// Global function. Copies string from str into pseg, then jumps into
// a banked function.
void puts(const char __code *str);

void putsi(const char __idata *str);
void putsp(const char __pdata *str) __banked;
void out_short(unsigned short val);
/* Careful: Must make sure that caller puts strings in a location
 * where they can not be overwritten by the overlayed print module */
// void putsx(const char __xdata *str);
void putsc(const char __code *str);
void print_dec(unsigned char val) __banked;
void print_short(unsigned short val) __banked;

#define char_width() 4
#define char_height() 8
