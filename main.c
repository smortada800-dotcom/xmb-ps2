/* ============================================================
   Tokyo XMB - Custom PS2 Homebrew Interface
   by: JDM Tokyo Project

   المتطلبات:
     - ps2sdk  (https://github.com/ps2dev/ps2dev)
     - gsKit   (https://github.com/ps2dev/gsKit)

   لبناء الملف:
     $ make

   لتشغيله على PS2:
     انسخ tokyo_xmb.elf إلى USB أو Memory Card
     ثم شغّله من wLaunchELF أو uLaunchELF
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* PS2SDK */
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <iopcontrol.h>

/* gsKit */
#include <gsKit.h>
#include <dmaKit.h>
#include <gsToolkit.h>

/* مكتبة يد التحكم */
#include <libpad.h>

/* ملفاتنا */
#include "types.h"
#include "background.h"
#include "xmb.h"
#include "input.h"

/* ======================================
   الدوال المساعدة لـ IOP
   ====================================== */
static void reset_iop(void) {
    /* إعادة تشغيل IOP (معالج الإدخال/إخراج) */
    while (!SifIopReset("", 0)) { ; }
    while (!SifIopSync())        { ; }
    SifInitRpc(0);
    SifLoadFileInit();

    /* تحميل وحدات النظام الأساسية */
    SifLoadModule("rom0:SIO2MAN", 0, NULL);
    SifLoadModule("rom0:PADMAN",  0, NULL);
}

/* ======================================
   تهيئة gsKit (نظام الجرافيك)
   ====================================== */
static GSGLOBAL *init_gskit(void) {
    GSGLOBAL *gs;

    dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
                D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);
    dmaKit_chan_init(DMA_CHANNEL_GIF);

    gs = gsKit_init_global();

    /* وضع PAL أو NTSC حسب المنطقة */
    gs->PSM   = GS_PSM_CT16;
    gs->PSMZ  = GS_PSMZ_16;
    gs->ZBuffering = GS_SETTING_OFF;

    /* دقة الشاشة */
    gs->Width  = SCREEN_W;
    gs->Height = SCREEN_H;

    /* وضع العرض */
#ifdef PAL_MODE
    gs->Mode = GS_MODE_PAL;
#else
    gs->Mode = GS_MODE_NTSC;
#endif

    gs->Interlace    = GS_INTERLACE_ON;
    gs->Field        = GS_FIELD_FIELD;
    gs->DoubleBuffer = GS_SETTING_ON;

    gsKit_init_screen(gs);
    gsKit_mode_switch(gs, GS_ONESHOT);

    return gs;
}

/* ======================================
   حلقة اللعب الرئيسية (Game Loop)
   ====================================== */
static void game_loop(GSGLOBAL *gs) {
    Background bg;
    XMBState   xmb;
    InputState inp;

    /* التهيئة */
    srand(time(NULL));
    background_init(&bg);
    xmb_init(&xmb);
    input_init(&inp);

    /* انتظر جاهزية يد التحكم */
    {
        int wait = 0;
        while (wait < 60) {
            input_update(&inp);
            wait++;
        }
    }

    /* حالة التطبيق */
    int running = 1;

    while (running) {
        /* --- 1. قراءة الإدخال --- */
        input_update(&inp);

        /* يمين/يسار → تغيير التصنيف */
        if (input_just_pressed(&inp, BTN_LEFT))
            xmb_move_left(&xmb);
        if (input_just_pressed(&inp, BTN_RIGHT))
            xmb_move_right(&xmb);

        /* أعلى/أسفل → تغيير العنصر */
        if (input_just_pressed(&inp, BTN_UP))
            xmb_move_up(&xmb);
        if (input_just_pressed(&inp, BTN_DOWN))
            xmb_move_down(&xmb);

        /* زر Start → خروج (للمطورين فقط) */
        if (input_just_pressed(&inp, BTN_START) &&
            input_is_down(&inp, BTN_SELECT)) {
            running = 0;
        }

        /* --- 2. تحديث الحالة --- */
        background_update(&bg);
        xmb_update(&xmb);

        /* --- 3. الرسم --- */
        gsKit_clear(gs, GS_SETREG_RGBAQ(0, 0, 8, 0x80, 0));

        /* رسم الخلفية (طوكيو) */
        background_draw(gs, &bg);

        /* رسم واجهة XMB فوق الخلفية */
        xmb_draw(gs, &xmb);

        /* --- 4. عرض الإطار --- */
        gsKit_sync_flip(gs);
        gsKit_queue_exec(gs);
    }
}

/* ======================================
   نقطة الدخول الرئيسية
   ====================================== */
int main(int argc, char *argv[]) {
    /* تهيئة النظام */
    reset_iop();

    /* تهيئة الجرافيك */
    GSGLOBAL *gs = init_gskit();

    /* بدء الحلقة الرئيسية */
    game_loop(gs);

    /* تنظيف عند الخروج */
    gsKit_deinit_global(gs);
    padPortClose(0, 0);
    padEnd();

    return 0;
}
