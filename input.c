#include "input.h"
#include <string.h>

/* ======================================
   تهيئة يد التحكم
   ====================================== */
void input_init(InputState *inp) {
    memset(inp, 0, sizeof(InputState));

    /* تشغيل مكتبة pad */
    padInit(0);

    /* فتح المنفذ 0 (اليد الأولى) */
    padPortOpen(0, 0, inp->buf[0]);

    inp->ready = 0;
}

/* ======================================
   تحديث حالة الأزرار كل إطار
   ====================================== */
void input_update(InputState *inp) {
    int state = padGetState(0, 0);

    /* انتظر حتى تصبح اليد جاهزة */
    if (!inp->ready) {
        if (state == PAD_STATE_STABLE || state == PAD_STATE_FINDCTP1) {
            /* تفعيل وضع DualShock2 */
            if (padSetMainMode(0, 0, PAD_MMODE_DUALSHOCK, PAD_MMODE_LOCK) == 1)
                inp->ready = 1;
        }
        inp->current  = 0xFFFF; /* لا زر مضغوط */
        inp->pressed  = 0xFFFF;
        inp->released = 0xFFFF;
        inp->previous = 0xFFFF;
        return;
    }

    if (state != PAD_STATE_STABLE) return;

    /* قراءة الأزرار */
    padRead(0, 0, &inp->buttons);

    inp->previous = inp->current;
    inp->current  = inp->buttons.btns;

    /* ضُغط = كان مرفوعاً والآن مضغوط */
    /* في PS2: 0 = مضغوط، 1 = مرفوع */
    inp->pressed  = (~inp->current) & inp->previous;
    inp->released = inp->current & (~inp->previous);
}

/* ======================================
   التحقق من حالة الأزرار
   ====================================== */
int input_is_down(InputState *inp, u16 btn) {
    /* 0 = مضغوط في PS2 */
    return !(inp->current & btn);
}

int input_just_pressed(InputState *inp, u16 btn) {
    return !!(inp->pressed & btn);
}
