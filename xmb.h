#ifndef XMB_H
#define XMB_H

#include "types.h"
#include <gsKit.h>

/* ======================================
   نظام التنقل XMB
   ====================================== */

#define XMB_MAX_CATS   7
#define XMB_MAX_ITEMS  8

/* --- عنصر داخل تصنيف --- */
typedef struct {
    char label[32];   /* اسم العنصر (انجليزي) */
    char sub[32];     /* وصف ثانوي */
    char icon_char;   /* رمز بسيط */
} XMBItem;

/* --- تصنيف رئيسي --- */
typedef struct {
    char     label[24];
    char     icon_char;
    u8       color_r, color_g, color_b;
    XMBItem  items[XMB_MAX_ITEMS];
    int      item_count;
} XMBCategory;

/* --- حالة XMB الكاملة --- */
typedef struct {
    XMBCategory cats[XMB_MAX_CATS];
    int         cat_count;
    int         cat_idx;    /* التصنيف النشط */
    int         item_idx;   /* العنصر النشط */
    f32         cat_anim;   /* انيميشن انتقال التصنيف (0.0 - 1.0) */
    f32         item_anim;  /* انيميشن انتقال العنصر */
    u32         frame;
} XMBState;

/* --- الدوال --- */
void xmb_init(XMBState *xmb);
void xmb_update(XMBState *xmb);
void xmb_move_left(XMBState *xmb);
void xmb_move_right(XMBState *xmb);
void xmb_move_up(XMBState *xmb);
void xmb_move_down(XMBState *xmb);
void xmb_draw(GSGLOBAL *gs, XMBState *xmb);

#endif /* XMB_H */
