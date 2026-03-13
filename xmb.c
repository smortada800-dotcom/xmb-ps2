#include "xmb.h"
#include <string.h>
#include <math.h>

/* ======================================
   تهيئة بيانات XMB
   ====================================== */
void xmb_init(XMBState *xmb) {
    memset(xmb, 0, sizeof(XMBState));

    xmb->cat_idx  = 2; /* نبدأ على تصنيف الصور */
    xmb->item_idx = 0;
    xmb->cat_anim  = 1.0f;
    xmb->item_anim = 1.0f;

    /* ======== التصنيفات ======== */

    /* 0 - المستخدمون */
    xmb->cats[0].color_r = 0;
    xmb->cats[0].color_g = 160;
    xmb->cats[0].color_b = 255;
    xmb->cats[0].icon_char = 'U';
    strcpy(xmb->cats[0].label, "Users");
    xmb->cats[0].item_count = 2;
    strcpy(xmb->cats[0].items[0].label, "JDM Driver");
    strcpy(xmb->cats[0].items[0].sub,   "Level 99");
    strcpy(xmb->cats[0].items[1].label, "Add User");
    strcpy(xmb->cats[0].items[1].sub,   "New profile");

    /* 1 - الإعدادات */
    xmb->cats[1].color_r = 130;
    xmb->cats[1].color_g = 130;
    xmb->cats[1].color_b = 140;
    xmb->cats[1].icon_char = 'S';
    strcpy(xmb->cats[1].label, "Settings");
    xmb->cats[1].item_count = 5;
    strcpy(xmb->cats[1].items[0].label, "Display");
    strcpy(xmb->cats[1].items[0].sub,   "Screen settings");
    strcpy(xmb->cats[1].items[1].label, "Sound");
    strcpy(xmb->cats[1].items[1].sub,   "Audio settings");
    strcpy(xmb->cats[1].items[2].label, "Controllers");
    strcpy(xmb->cats[1].items[2].sub,   "Pad config");
    strcpy(xmb->cats[1].items[3].label, "Date & Time");
    strcpy(xmb->cats[1].items[3].sub,   "Clock");
    strcpy(xmb->cats[1].items[4].label, "Language");
    strcpy(xmb->cats[1].items[4].sub,   "AR / EN");

    /* 2 - الصور */
    xmb->cats[2].color_r = 0;
    xmb->cats[2].color_g = 200;
    xmb->cats[2].color_b = 130;
    xmb->cats[2].icon_char = 'P';
    strcpy(xmb->cats[2].label, "Photo");
    xmb->cats[2].item_count = 2;
    strcpy(xmb->cats[2].items[0].label, "Memory Card");
    strcpy(xmb->cats[2].items[0].sub,   "Slot 1 / 2");
    strcpy(xmb->cats[2].items[1].label, "USB Device");
    strcpy(xmb->cats[2].items[1].sub,   "Insert USB");

    /* 3 - الموسيقى */
    xmb->cats[3].color_r = 160;
    xmb->cats[3].color_g = 60;
    xmb->cats[3].color_b = 255;
    xmb->cats[3].icon_char = 'M';
    strcpy(xmb->cats[3].label, "Music");
    xmb->cats[3].item_count = 2;
    strcpy(xmb->cats[3].items[0].label, "CD / DVD");
    strcpy(xmb->cats[3].items[0].sub,   "Audio disc");
    strcpy(xmb->cats[3].items[1].label, "Memory Card");
    strcpy(xmb->cats[3].items[1].sub,   "Saved music");

    /* 4 - الفيديو */
    xmb->cats[4].color_r = 230;
    xmb->cats[4].color_g = 50;
    xmb->cats[4].color_b = 50;
    xmb->cats[4].icon_char = 'V';
    strcpy(xmb->cats[4].label, "Video");
    xmb->cats[4].item_count = 2;
    strcpy(xmb->cats[4].items[0].label, "DVD");
    strcpy(xmb->cats[4].items[0].sub,   "Play DVD movie");
    strcpy(xmb->cats[4].items[1].label, "Memory Card");
    strcpy(xmb->cats[4].items[1].sub,   "Saved video");

    /* 5 - الألعاب */
    xmb->cats[5].color_r = 255;
    xmb->cats[5].color_g = 130;
    xmb->cats[5].color_b = 0;
    xmb->cats[5].icon_char = 'G';
    strcpy(xmb->cats[5].label, "Game");
    xmb->cats[5].item_count = 3;
    strcpy(xmb->cats[5].items[0].label, "Play Game Disc");
    strcpy(xmb->cats[5].items[0].sub,   "Insert disc");
    strcpy(xmb->cats[5].items[1].label, "Save Data");
    strcpy(xmb->cats[5].items[1].sub,   "Memory Card");
    strcpy(xmb->cats[5].items[2].label, "Game Browser");
    strcpy(xmb->cats[5].items[2].sub,   "USB / HDD");

    /* 6 - الشبكة */
    xmb->cats[6].color_r = 0;
    xmb->cats[6].color_g = 200;
    xmb->cats[6].color_b = 240;
    xmb->cats[6].icon_char = 'N';
    strcpy(xmb->cats[6].label, "Network");
    xmb->cats[6].item_count = 2;
    strcpy(xmb->cats[6].items[0].label, "Browser");
    strcpy(xmb->cats[6].items[0].sub,   "Internet");
    strcpy(xmb->cats[6].items[1].label, "Online");
    strcpy(xmb->cats[6].items[1].sub,   "Multiplayer");

    xmb->cat_count = 7;
}

/* ======================================
   تحديث الانيميشن
   ====================================== */
void xmb_update(XMBState *xmb) {
    xmb->frame++;
    /* تسريع ناعم نحو 1.0 */
    if (xmb->cat_anim < 1.0f) {
        xmb->cat_anim += 0.12f;
        if (xmb->cat_anim > 1.0f) xmb->cat_anim = 1.0f;
    }
    if (xmb->item_anim < 1.0f) {
        xmb->item_anim += 0.14f;
        if (xmb->item_anim > 1.0f) xmb->item_anim = 1.0f;
    }
}

/* ======================================
   حركات التنقل
   ====================================== */
void xmb_move_left(XMBState *xmb) {
    if (xmb->cat_idx > 0) {
        xmb->cat_idx--;
        xmb->item_idx = 0;
        xmb->cat_anim = 0.0f;
    }
}

void xmb_move_right(XMBState *xmb) {
    if (xmb->cat_idx < xmb->cat_count - 1) {
        xmb->cat_idx++;
        xmb->item_idx = 0;
        xmb->cat_anim = 0.0f;
    }
}

void xmb_move_up(XMBState *xmb) {
    if (xmb->item_idx > 0) {
        xmb->item_idx--;
        xmb->item_anim = 0.0f;
    }
}

void xmb_move_down(XMBState *xmb) {
    XMBCategory *cat = &xmb->cats[xmb->cat_idx];
    if (xmb->item_idx < cat->item_count - 1) {
        xmb->item_idx++;
        xmb->item_anim = 0.0f;
    }
}

/* ======================================
   رسم مربع ملون بسيط (بدلاً من gsKit font)
   ====================================== */
static void draw_rect(GSGLOBAL *gs, f32 x, f32 y, f32 w, f32 h,
                      u8 r, u8 g, u8 b, u8 a, int z) {
    gsKit_prim_sprite(gs, x, y, x+w, y+h, z,
        GS_SETREG_RGBAQ(r, g, b, a, 0));
}

/* رسم حرف بسيط بمستطيلات (pixel font بسيط لأيقونة التصنيف) */
static void draw_icon_letter(GSGLOBAL *gs, char c, f32 cx, f32 cy,
                             u8 r, u8 g, u8 b, f32 scale) {
    /* مجرد مربع مع حرف - PS2 لا يدعم خطوط كاملة بدون مكتبة font */
    (void)c;
    f32 s = scale * 10;
    draw_rect(gs, cx-s*0.5f, cy-s*0.5f, s, s, r, g, b, 0x80, 3);
}

/* ======================================
   رسم XMB كامل
   ====================================== */
void xmb_draw(GSGLOBAL *gs, XMBState *xmb) {
    int i;
    XMBCategory *active_cat = &xmb->cats[xmb->cat_idx];

    /* --- الخط الافقي الفاصل --- */
    f32 line_y = SCREEN_H * 0.62f;
    draw_rect(gs, 0, line_y, SCREEN_W, 1,
              active_cat->color_r, active_cat->color_g, active_cat->color_b,
              0x55, 2);

    /* توهج الخط */
    draw_rect(gs, 0, line_y-2, SCREEN_W, 5,
              active_cat->color_r, active_cat->color_g, active_cat->color_b,
              0x18, 2);

    /* --- التصنيفات الافقية --- */
    f32 cat_center_x = SCREEN_W / 2.0f;
    f32 cat_y        = line_y - 50;
    f32 cat_spacing  = 55;

    for (i = 0; i < xmb->cat_count; i++) {
        XMBCategory *cat = &xmb->cats[i];
        int dist = i - xmb->cat_idx;
        f32 x    = cat_center_x + dist * cat_spacing * xmb->cat_anim;
        f32 scale, alpha_f;

        if (dist == 0) {
            scale   = 1.0f;
            alpha_f = 1.0f;
        } else if (abs(dist) == 1) {
            scale   = 0.72f;
            alpha_f = 0.55f;
        } else if (abs(dist) == 2) {
            scale   = 0.52f;
            alpha_f = 0.30f;
        } else {
            scale   = 0.38f;
            alpha_f = 0.15f;
        }

        u8 alpha = (u8)(alpha_f * 0x80);
        f32 icon_size = 36 * scale;
        f32 ix = x - icon_size * 0.5f;
        f32 iy = cat_y - icon_size * 0.5f;

        /* خلفية الأيقونة */
        draw_rect(gs, ix, iy, icon_size, icon_size,
                  cat->color_r / 4, cat->color_g / 4, cat->color_b / 4,
                  alpha, 3);

        /* حد الأيقونة */
        draw_rect(gs, ix-1, iy-1, icon_size+2, 1,
                  cat->color_r, cat->color_g, cat->color_b, alpha/2, 3);
        draw_rect(gs, ix-1, iy+icon_size, icon_size+2, 1,
                  cat->color_r, cat->color_g, cat->color_b, alpha/2, 3);
        draw_rect(gs, ix-1, iy, 1, icon_size,
                  cat->color_r, cat->color_g, cat->color_b, alpha/2, 3);
        draw_rect(gs, ix+icon_size, iy, 1, icon_size,
                  cat->color_r, cat->color_g, cat->color_b, alpha/2, 3);

        /* مربع داخلي (يمثل الايقونة) */
        f32 inner = icon_size * 0.5f;
        draw_rect(gs,
                  x - inner*0.5f, cat_y - inner*0.5f,
                  inner, inner,
                  cat->color_r, cat->color_g, cat->color_b, alpha, 4);

        /* توهج التصنيف النشط */
        if (dist == 0) {
            draw_rect(gs, ix-6, iy-6, icon_size+12, icon_size+12,
                      cat->color_r, cat->color_g, cat->color_b, 0x10, 2);
        }
    }

    /* --- عناصر التصنيف النشط (عمودي) --- */
    f32 items_x = SCREEN_W * 0.5f - 80;
    f32 items_y = line_y + 18;
    f32 item_h  = 36;

    for (i = 0; i < active_cat->item_count; i++) {
        XMBItem *item = &active_cat->items[i];
        int dist = i - xmb->item_idx;
        f32 y = items_y + dist * item_h * xmb->item_anim;
        f32 alpha_f, scale;

        if (dist == 0) {
            alpha_f = 1.0f; scale = 1.0f;
        } else if (abs(dist) == 1) {
            alpha_f = 0.6f; scale = 0.9f;
        } else {
            alpha_f = 0.3f; scale = 0.78f;
        }

        u8 alpha = (u8)(alpha_f * 0x80);
        f32 row_w = 220 * scale;
        f32 row_h = 28 * scale;

        /* خلفية العنصر النشط */
        if (dist == 0) {
            draw_rect(gs, items_x - 8, y - 2, row_w + 16, row_h + 4,
                      active_cat->color_r, active_cat->color_g, active_cat->color_b,
                      0x15, 2);
            /* حد يسار ملون */
            draw_rect(gs, items_x - 8, y, 3, row_h,
                      active_cat->color_r, active_cat->color_g, active_cat->color_b,
                      0x80, 3);
        }

        /* أيقونة العنصر */
        draw_rect(gs, items_x, y + (row_h - 20*scale)*0.5f,
                  20*scale, 20*scale,
                  active_cat->color_r / 2, active_cat->color_g / 2,
                  active_cat->color_b / 2, alpha, 3);

        /* شريط اسم العنصر (بدل النص الحقيقي) */
        /* النص الحقيقي يحتاج gsKit_fontm أو مكتبة font خارجية */
        f32 text_x = items_x + 28;
        f32 text_y = y + row_h * 0.25f;

        /* خط يمثل النص الرئيسي */
        f32 text_w = (f32)strlen(item->label) * 6.5f * scale;
        draw_rect(gs, text_x, text_y, text_w, 6 * scale,
                  230, 230, 240, alpha, 4);

        /* خط يمثل النص الفرعي */
        if (strlen(item->sub) > 0) {
            f32 sub_w = (f32)strlen(item->sub) * 5.0f * scale;
            draw_rect(gs, text_x, text_y + 10*scale, sub_w, 4*scale,
                      140, 140, 160, alpha/2, 4);
        }

        /* سهم للعنصر النشط */
        if (dist == 0) {
            draw_rect(gs, items_x + row_w + 4, y + row_h*0.3f,
                      8, row_h * 0.4f,
                      active_cat->color_r, active_cat->color_g, active_cat->color_b,
                      0x80, 4);
        }
    }

    /* --- شريط المعلومات العلوي (الوقت) --- */
    {
        /* مستطيل خلفية علوي */
        draw_rect(gs, 0, 0, SCREEN_W, 28,
                  0, 0, 8, 0x50, 1);
        /* مؤشر بطارية */
        draw_rect(gs, SCREEN_W - 40, 7, 26, 14, 40, 40, 50, 0x80, 3);
        draw_rect(gs, SCREEN_W - 14, 10, 4, 8,  40, 40, 50, 0x80, 3);
        draw_rect(gs, SCREEN_W - 39, 8, 18, 12, 80, 200, 80, 0x80, 4);
        /* مؤشر واي فاي */
        draw_rect(gs, SCREEN_W - 70, 9, 22, 10, 60, 140, 200, 0x60, 3);
    }

    /* --- شريط الأزرار السفلي --- */
    {
        f32 bh = 26;
        draw_rect(gs, 0, SCREEN_H - bh, SCREEN_W, bh,
                  0, 0, 10, 0x60, 2);

        /* زر X - رجوع */
        draw_rect(gs, SCREEN_W - 180, SCREEN_H - 20, 14, 14, 40, 80, 180, 0x80, 3);
        draw_rect(gs, SCREEN_W - 160, SCREEN_H - 18, 40, 8, 140, 140, 160, 0x60, 3);

        /* زر O - تأكيد */
        draw_rect(gs, SCREEN_W - 110, SCREEN_H - 20, 14, 14, 180, 40, 40, 0x80, 3);
        draw_rect(gs, SCREEN_W - 90, SCREEN_H - 18, 40, 8, 140, 140, 160, 0x60, 3);

        /* JDM tag - شريط برتقالي */
        draw_rect(gs, 10, SCREEN_H - 22, 4, 14,
                  255, 160, 0, 0x80, 3);
        draw_rect(gs, 20, SCREEN_H - 20, 70, 8,
                  200, 120, 0, 0x50, 3);
    }
}
