#ifndef TYPES_H
#define TYPES_H

/* ======================================
   انواع البيانات الاساسية
   ====================================== */

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef signed char    s8;
typedef signed short   s16;
typedef signed int     s32;
typedef float          f32;

/* لون RGBA */
typedef struct {
    u8 r, g, b, a;
} Color;

/* نقطة ثنائية الابعاد */
typedef struct {
    f32 x, y;
} Vec2;

/* مستطيل */
typedef struct {
    f32 x, y, w, h;
} Rect;

/* ماكرو لبناء لون */
#define MAKE_COLOR(r,g,b,a) ((Color){(r),(g),(b),(a)})

/* الوان جاهزة */
#define COLOR_WHITE  MAKE_COLOR(255,255,255,255)
#define COLOR_BLACK  MAKE_COLOR(0,0,0,255)
#define COLOR_CLEAR  MAKE_COLOR(0,0,0,0)

/* دقة الشاشة */
#define SCREEN_W  640
#define SCREEN_H  448

/* هدف 60 اطار */
#define TARGET_FPS 60

#endif /* TYPES_H */
