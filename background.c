#include "background.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

/* ======================================
   دوال مساعدة
   ====================================== */

/* عشوائي بين min و max */
static f32 randf(f32 min, f32 max) {
    return min + ((f32)rand() / (f32)RAND_MAX) * (max - min);
}

/* ======================================
   تهيئة الخلفية
   ====================================== */
void background_init(Background *bg) {
    int i;
    memset(bg, 0, sizeof(Background));

    bg->road_y = SCREEN_H * 0.58f; /* الطريق يبدأ عند 58% من الارتفاع */
    bg->frame  = 0;

    /* --- المباني --- */
    f32 bx = 0.0f;
    for (i = 0; i < MAX_BUILDINGS; i++) {
        bg->buildings[i].x = bx;
        bg->buildings[i].w = randf(50, 90);
        bg->buildings[i].h = randf(100, 260);
        bg->buildings[i].y = bg->road_y - bg->buildings[i].h;

        /* لون البناية: درجات داكنة */
        bg->buildings[i].r = (u8)randf(8, 18);
        bg->buildings[i].g = (u8)randf(8, 18);
        bg->buildings[i].b = (u8)randf(18, 35);

        /* نيون عشوائي */
        bg->buildings[i].has_neon = (rand() % 2 == 0);
        /* الوان النيون: وردي, ازرق, اخضر, برتقالي */
        int neon_type = rand() % 4;
        switch(neon_type) {
            case 0: bg->buildings[i].neon_r=255; bg->buildings[i].neon_g=0;   bg->buildings[i].neon_b=100; break;
            case 1: bg->buildings[i].neon_r=0;   bg->buildings[i].neon_g=200; bg->buildings[i].neon_b=255; break;
            case 2: bg->buildings[i].neon_r=0;   bg->buildings[i].neon_g=255; bg->buildings[i].neon_b=100; break;
            case 3: bg->buildings[i].neon_r=255; bg->buildings[i].neon_g=160; bg->buildings[i].neon_b=0;   break;
        }
        bg->buildings[i].neon_timer = rand() % 120;

        bx += bg->buildings[i].w + randf(3, 15);
    }

    /* --- السيارات --- */
    /* 4 سيارات تسير يميناً (حارة 1 و 2) */
    for (i = 0; i < 4; i++) {
        bg->cars[i].dir   = 1;
        bg->cars[i].x     = randf(-200, SCREEN_W);
        bg->cars[i].y     = bg->road_y + 10 + (i % 2) * 22;
        bg->cars[i].speed = randf(2.5f, 5.0f);
        bg->cars[i].w     = randf(50, 75);
        bg->cars[i].h     = 14;
        /* مصابيح بيضاء / صفراء */
        bg->cars[i].head_r = 255; bg->cars[i].head_g = 245; bg->cars[i].head_b = 200;
        bg->cars[i].tail_r = 255; bg->cars[i].tail_g = 30;  bg->cars[i].tail_b = 0;
    }
    /* 4 سيارات تسير يساراً (حارة 3 و 4) */
    for (i = 4; i < 8; i++) {
        bg->cars[i].dir   = -1;
        bg->cars[i].x     = randf(0, SCREEN_W + 200);
        bg->cars[i].y     = bg->road_y + 60 + ((i-4) % 2) * 22;
        bg->cars[i].speed = randf(2.5f, 5.0f);
        bg->cars[i].w     = randf(50, 75);
        bg->cars[i].h     = 14;
        bg->cars[i].head_r = 255; bg->cars[i].head_g = 245; bg->cars[i].head_b = 200;
        bg->cars[i].tail_r = 255; bg->cars[i].tail_g = 30;  bg->cars[i].tail_b = 0;
    }

    /* --- المطر --- */
    for (i = 0; i < MAX_RAIN; i++) {
        bg->rain[i].x     = randf(0, SCREEN_W);
        bg->rain[i].y     = randf(0, SCREEN_H);
        bg->rain[i].speed = randf(8, 16);
        bg->rain[i].len   = randf(12, 22);
        bg->rain[i].alpha = (u8)randf(30, 80);
    }

    /* --- النجوم --- */
    for (i = 0; i < MAX_STARS; i++) {
        bg->stars[i].x            = randf(0, SCREEN_W);
        bg->stars[i].y            = randf(0, SCREEN_H * 0.45f);
        bg->stars[i].brightness   = randf(0.3f, 1.0f);
        bg->stars[i].twinkle_speed = randf(0.02f, 0.06f);
        bg->stars[i].twinkle_phase = randf(0, 6.28f);
    }
}

/* ======================================
   تحديث الحالة كل إطار
   ====================================== */
void background_update(Background *bg) {
    int i;
    bg->frame++;

    /* تحريك السيارات */
    for (i = 0; i < MAX_CARS; i++) {
        bg->cars[i].x += bg->cars[i].speed * bg->cars[i].dir;
        /* إعادة السيارة من الجهة المقابلة */
        if (bg->cars[i].dir == 1 && bg->cars[i].x > SCREEN_W + 100)
            bg->cars[i].x = -100;
        if (bg->cars[i].dir == -1 && bg->cars[i].x < -100)
            bg->cars[i].x = SCREEN_W + 100;
    }

    /* تحريك المطر */
    for (i = 0; i < MAX_RAIN; i++) {
        bg->rain[i].y += bg->rain[i].speed;
        bg->rain[i].x -= 1.0f; /* انحراف طفيف */
        if (bg->rain[i].y > SCREEN_H) {
            bg->rain[i].y = -(bg->rain[i].len);
            bg->rain[i].x = randf(0, SCREEN_W);
        }
    }

    /* تحديث وميض النيون */
    for (i = 0; i < MAX_BUILDINGS; i++) {
        bg->buildings[i].neon_timer++;
    }

    /* تحديث النجوم */
    for (i = 0; i < MAX_STARS; i++) {
        bg->stars[i].twinkle_phase += bg->stars[i].twinkle_speed;
        if (bg->stars[i].twinkle_phase > 6.28f)
            bg->stars[i].twinkle_phase -= 6.28f;
    }
}

/* ======================================
   رسم الخلفية
   ====================================== */
void background_draw(GSGLOBAL *gs, Background *bg) {
    int i;
    f32 road_y = bg->road_y;

    /* --- السماء (تدرج داكن) --- */
    /* gsKit لا يدعم تدرجاً مباشراً، نرسم مستطيلات بتدرج يدوي */
    int sky_bands = 12;
    f32 band_h = road_y / sky_bands;
    for (i = 0; i < sky_bands; i++) {
        f32 t = (f32)i / sky_bands;
        u8 r = (u8)(0  + t * 20);
        u8 g = (u8)(0  + t * 0);
        u8 b = (u8)(15 + t * 50);
        gsKit_prim_sprite(gs,
            0, i * band_h,
            SCREEN_W, (i+1) * band_h,
            1,
            GS_SETREG_RGBAQ(r, g, b, 0x80, 0)
        );
    }

    /* --- النجوم --- */
    for (i = 0; i < MAX_STARS; i++) {
        f32 bright = bg->stars[i].brightness *
                     (0.5f + 0.5f * sinf(bg->stars[i].twinkle_phase));
        u8 sb = (u8)(bright * 220);
        gsKit_prim_sprite(gs,
            bg->stars[i].x - 1, bg->stars[i].y - 1,
            bg->stars[i].x + 1, bg->stars[i].y + 1,
            2,
            GS_SETREG_RGBAQ(sb, sb, sb+30, 0x80, 0)
        );
    }

    /* --- القمر --- */
    {
        f32 mx = SCREEN_W * 0.85f, my = SCREEN_H * 0.10f;
        /* هالة القمر */
        gsKit_prim_sprite(gs,
            mx-30, my-30, mx+30, my+30, 2,
            GS_SETREG_RGBAQ(30, 30, 60, 0x40, 0)
        );
        /* جسم القمر */
        gsKit_prim_sprite(gs,
            mx-14, my-14, mx+14, my+14, 3,
            GS_SETREG_RGBAQ(200, 200, 230, 0x80, 0)
        );
    }

    /* --- المباني --- */
    for (i = 0; i < MAX_BUILDINGS; i++) {
        Building *b = &bg->buildings[i];

        /* جسم البناية */
        gsKit_prim_sprite(gs,
            b->x, b->y, b->x + b->w, road_y,
            1,
            GS_SETREG_RGBAQ(b->r, b->g, b->b, 0x80, 0)
        );

        /* نوافذ مضاءة - شبكة بسيطة */
        {
            int col, row;
            int cols = (int)(b->w / 10);
            int rows = (int)(b->h / 14);
            for (row = 0; row < rows; row++) {
                for (col = 0; col < cols; col++) {
                    /* 60% من النوافذ مضاءة عشوائياً بناءً على رقم الاطار */
                    int seed = (i*1000 + row*50 + col*7 + bg->frame/90);
                    if ((seed % 10) < 6) {
                        f32 wx = b->x + col * 10 + 3;
                        f32 wy = b->y + row * 14 + 4;
                        gsKit_prim_sprite(gs,
                            wx, wy, wx+5, wy+7, 2,
                            GS_SETREG_RGBAQ(240, 220, 160, 0x80, 0)
                        );
                    }
                }
            }
        }

        /* اشارة النيون */
        if (b->has_neon) {
            /* وميض النيون */
            int on = ((b->neon_timer / 30) % 3 != 2);
            if (on) {
                f32 nx = b->x + b->w * 0.15f;
                f32 ny = b->y + b->h * 0.25f;
                f32 nw = b->w * 0.7f;
                /* خط نيون علوي */
                gsKit_prim_sprite(gs,
                    nx, ny, nx+nw, ny+2, 3,
                    GS_SETREG_RGBAQ(b->neon_r, b->neon_g, b->neon_b, 0x80, 0)
                );
                /* خط نيون سفلي */
                gsKit_prim_sprite(gs,
                    nx, ny+16, nx+nw, ny+18, 3,
                    GS_SETREG_RGBAQ(b->neon_r, b->neon_g, b->neon_b, 0x80, 0)
                );
                /* خط نيون يسار */
                gsKit_prim_sprite(gs,
                    nx, ny, nx+2, ny+18, 3,
                    GS_SETREG_RGBAQ(b->neon_r, b->neon_g, b->neon_b, 0x80, 0)
                );
                /* خط نيون يمين */
                gsKit_prim_sprite(gs,
                    nx+nw-2, ny, nx+nw, ny+18, 3,
                    GS_SETREG_RGBAQ(b->neon_r, b->neon_g, b->neon_b, 0x80, 0)
                );
            }
        }

        /* هوائي على السطح */
        if (b->h > 180) {
            f32 ax = b->x + b->w * 0.5f;
            gsKit_prim_sprite(gs,
                ax-1, b->y-18, ax+1, b->y, 2,
                GS_SETREG_RGBAQ(60, 60, 60, 0x80, 0)
            );
            /* ضوء احمر وامض */
            if ((bg->frame / 30) % 2 == 0) {
                gsKit_prim_sprite(gs,
                    ax-3, b->y-21, ax+3, b->y-15, 3,
                    GS_SETREG_RGBAQ(255, 30, 0, 0x80, 0)
                );
            }
        }
    }

    /* --- الطريق --- */
    gsKit_prim_sprite(gs,
        0, road_y, SCREEN_W, SCREEN_H,
        1,
        GS_SETREG_RGBAQ(12, 12, 18, 0x80, 0)
    );

    /* خط فاصل حارات (متقطع متحرك) */
    {
        f32 dash_y = road_y + (SCREEN_H - road_y) * 0.45f;
        int dash_offset = (bg->frame * 3) % 50;
        f32 dx;
        for (dx = -50 + dash_offset; dx < SCREEN_W; dx += 50) {
            gsKit_prim_sprite(gs,
                dx, dash_y-1, dx+28, dash_y+1, 2,
                GS_SETREG_RGBAQ(180, 160, 0, 0x60, 0)
            );
        }
    }

    /* خط حافة الطريق العلوي */
    gsKit_prim_sprite(gs,
        0, road_y, SCREEN_W, road_y+2, 2,
        GS_SETREG_RGBAQ(80, 80, 120, 0x80, 0)
    );

    /* --- أعمدة الإنارة --- */
    {
        f32 px;
        for (px = 60; px < SCREEN_W; px += 160) {
            /* العمود */
            gsKit_prim_sprite(gs,
                px-1, road_y-70, px+1, road_y, 2,
                GS_SETREG_RGBAQ(50, 50, 60, 0x80, 0)
            );
            /* الذراع الأفقي */
            gsKit_prim_sprite(gs,
                px-20, road_y-70, px, road_y-68, 2,
                GS_SETREG_RGBAQ(50, 50, 60, 0x80, 0)
            );
            /* المصباح */
            gsKit_prim_sprite(gs,
                px-23, road_y-73, px-17, road_y-67, 3,
                GS_SETREG_RGBAQ(255, 200, 100, 0x80, 0)
            );
            /* مخروط الضوء على الأرض */
            gsKit_prim_sprite(gs,
                px-50, road_y, px+10, road_y+40, 1,
                GS_SETREG_RGBAQ(40, 35, 15, 0x20, 0)
            );
        }
    }

    /* --- السيارات --- */
    for (i = 0; i < MAX_CARS; i++) {
        Car *c = &bg->cars[i];
        f32 cx = c->x, cy = c->y;
        f32 cw = c->w, ch = c->h;

        /* جسم السيارة */
        gsKit_prim_sprite(gs,
            cx - cw/2, cy - ch/2, cx + cw/2, cy + ch/2, 2,
            GS_SETREG_RGBAQ(15, 15, 28, 0x80, 0)
        );

        /* سقف السيارة */
        gsKit_prim_sprite(gs,
            cx - cw*0.3f, cy - ch/2 - ch*0.65f,
            cx + cw*0.3f, cy - ch/2, 3,
            GS_SETREG_RGBAQ(10, 10, 20, 0x80, 0)
        );

        if (c->dir == 1) {
            /* مصابيح امامية (يمين) */
            gsKit_prim_sprite(gs,
                cx + cw/2 - 4, cy - 4, cx + cw/2, cy + 4, 4,
                GS_SETREG_RGBAQ(c->head_r, c->head_g, c->head_b, 0x80, 0)
            );
            /* خط ضوء امامي */
            gsKit_prim_sprite(gs,
                cx + cw/2, cy - 2, cx + cw/2 + 35, cy + 2, 4,
                GS_SETREG_RGBAQ(255, 245, 200, 0x40, 0)
            );
        } else {
            /* اضواء خلفية حمراء (يسار) */
            gsKit_prim_sprite(gs,
                cx - cw/2, cy - 4, cx - cw/2 + 4, cy + 4, 4,
                GS_SETREG_RGBAQ(c->tail_r, c->tail_g, c->tail_b, 0x80, 0)
            );
            /* خط ضوء خلفي */
            gsKit_prim_sprite(gs,
                cx - cw/2 - 30, cy - 2, cx - cw/2, cy + 2, 4,
                GS_SETREG_RGBAQ(200, 20, 0, 0x40, 0)
            );
        }

        /* انعكاس السيارة على الاسفلت */
        gsKit_prim_sprite(gs,
            cx - cw/2, cy + ch/2, cx + cw/2, cy + ch/2 + 5, 2,
            GS_SETREG_RGBAQ(
                c->dir == 1 ? c->head_r : c->tail_r,
                c->dir == 1 ? c->head_g : c->tail_g,
                c->dir == 1 ? c->head_b : c->tail_b,
                0x18, 0
            )
        );
    }

    /* --- المطر --- */
    for (i = 0; i < MAX_RAIN; i++) {
        RainDrop *d = &bg->rain[i];
        gsKit_prim_sprite(gs,
            d->x,   d->y,
            d->x+1, d->y + d->len, 5,
            GS_SETREG_RGBAQ(120, 150, 220, d->alpha, 0)
        );
    }

    /* --- تأثير vignette (تعتيم الحواف) --- */
    /* يسار */
    gsKit_prim_sprite(gs,
        0, 0, 60, SCREEN_H, 1,
        GS_SETREG_RGBAQ(0, 0, 5, 0x60, 0)
    );
    /* يمين */
    gsKit_prim_sprite(gs,
        SCREEN_W-60, 0, SCREEN_W, SCREEN_H, 1,
        GS_SETREG_RGBAQ(0, 0, 5, 0x60, 0)
    );
    /* اسفل */
    gsKit_prim_sprite(gs,
        0, SCREEN_H-60, SCREEN_W, SCREEN_H, 1,
        GS_SETREG_RGBAQ(0, 0, 8, 0x70, 0)
    );
}
