#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "types.h"
#include <gsKit.h>

/* ======================================
   خلفية طوكيو الليلية - JDM
   ====================================== */

/* عدد المباني */
#define MAX_BUILDINGS  12

/* عدد السيارات */
#define MAX_CARS       8

/* عدد قطرات المطر */
#define MAX_RAIN       150

/* عدد النجوم */
#define MAX_STARS      60

/* ---- بناء ---- */
typedef struct {
    f32 x, y;       /* موضع البناية (x=يسار, y=قمة) */
    f32 w, h;       /* عرض وارتفاع */
    u8  r,g,b;      /* لون البناية */
    u8  neon_r, neon_g, neon_b; /* لون إشارة النيون */
    int has_neon;   /* هل تحتوي نيون؟ */
    u32 neon_timer; /* عداد وميض النيون */
} Building;

/* ---- سيارة ---- */
typedef struct {
    f32 x, y;       /* موضع السيارة */
    f32 speed;      /* السرعة (موجب=يمين، سالب=يسار) */
    f32 w, h;       /* حجم السيارة */
    u8  head_r, head_g, head_b; /* لون المصابيح الامامية */
    u8  tail_r, tail_g, tail_b; /* لون الاضواء الخلفية */
    int dir;        /* اتجاه: 1=يمين، -1=يسار */
} Car;

/* ---- قطرة مطر ---- */
typedef struct {
    f32 x, y;
    f32 speed;
    f32 len;
    u8  alpha;
} RainDrop;

/* ---- نجمة ---- */
typedef struct {
    f32 x, y;
    f32 brightness;
    f32 twinkle_speed;
    f32 twinkle_phase;
} Star;

/* ---- حالة الخلفية ---- */
typedef struct {
    Building  buildings[MAX_BUILDINGS];
    Car       cars[MAX_CARS];
    RainDrop  rain[MAX_RAIN];
    Star      stars[MAX_STARS];
    u32       frame;
    f32       road_y;   /* ارتفاع الطريق من الأعلى */
} Background;

/* --- الدوال --- */
void background_init(Background *bg);
void background_update(Background *bg);
void background_draw(GSGLOBAL *gs, Background *bg);

#endif /* BACKGROUND_H */
