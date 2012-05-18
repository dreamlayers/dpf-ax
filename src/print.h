// This is currently fixed. Don't change.
#define CHAR_WIDTH_SMALL   4 
#define CHAR_WIDTH_LARGE   9
#define CHAR_HEIGHT_SMALL  8
#define CHAR_HEIGHT_LARGE  16

#define NUM_COLS_SMALL     (RESOL_X / CHAR_WIDTH_SMALL)
#define NUM_LINES_SMALL    (RESOL_Y / CHAR_HEIGHT_SMALL)
#define NUM_COLS_LARGE     (RESOL_X / CHAR_WIDTH_LARGE)
#define NUM_LINES_LARGE    (RESOL_Y / CHAR_HEIGHT_LARGE)

#define FONT_SMALL 0
#define FONT_SMALL_REVERSE 1
#define FONT_LARGE 2
#define FONT_LARGE_WHITE 3
#define FONT_LARGE_BLACK 4

#define WHITE RGB565(255, 255, 255)
#define BLACK RGB565(0, 0, 0)
#define DEV_SCREEN_COL RGB565(0, 255, 0)
#define DEV_SCREEN_BGCOL RGB565(0, 60, 40)

extern __data unsigned char g_chartbl_offs[3];
extern __bit g_largefont;
extern char __pdata g_printbuf[];

// blit.s:
void blit_char(unsigned char c);
#ifndef BUILD_DEVEL
void _blit_splash(unsigned int, unsigned int);
#endif

void term_init(void) __banked;
void term_selfont(unsigned char which) __banked;
//void disp_home(void) __banked;
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
void print_hex(unsigned char val) __banked;
void print_short(unsigned short val) __banked;
void print_splash(void) __banked;


unsigned char num_cols() __banked;
unsigned char num_lines() __banked;

#define set_color(f) g_fgcol = f
#define set_bgcolor(f) g_bgcol = f
