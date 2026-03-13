#ifndef INPUT_H
#define INPUT_H

#include "types.h"
#include <libpad.h>

/* ======================================
   نظام الإدخال - يد التحكم PS2
   ====================================== */

/* حجم buffer يد التحكم */
#define PAD_BUF_SIZE 256

typedef struct {
    char         buf[2][PAD_BUF_SIZE] __attribute__((aligned(64)));
    struct padButtonStatus buttons;
    u16          current;   /* الأزرار المضغوطة الآن */
    u16          pressed;   /* الأزرار التي ضُغطت للتو */
    u16          released;  /* الأزرار التي رُفعت */
    u16          previous;  /* الأزرار في الإطار السابق */
    int          ready;     /* هل اليد جاهزة؟ */
} InputState;

/* --- تعريفات الأزرار --- */
#define BTN_LEFT    PAD_LEFT
#define BTN_RIGHT   PAD_RIGHT
#define BTN_UP      PAD_UP
#define BTN_DOWN    PAD_DOWN
#define BTN_CROSS   PAD_CROSS    /* X */
#define BTN_CIRCLE  PAD_CIRCLE   /* O */
#define BTN_SQUARE  PAD_SQUARE   /* [] */
#define BTN_TRIANGLE PAD_TRIANGLE /* /\ */
#define BTN_START   PAD_START
#define BTN_SELECT  PAD_SELECT
#define BTN_L1      PAD_L1
#define BTN_R1      PAD_R1

/* ماكرو: هل الزر مضغوط الآن؟ */
#define IS_DOWN(inp, btn)    (!((inp)->current & (btn)))
/* ماكرو: هل الزر ضُغط للتو؟ */
#define IS_PRESSED(inp, btn) (!((inp)->pressed & (btn)))

/* --- الدوال --- */
void  input_init(InputState *inp);
void  input_update(InputState *inp);
int   input_is_down(InputState *inp, u16 btn);
int   input_just_pressed(InputState *inp, u16 btn);

#endif /* INPUT_H */
