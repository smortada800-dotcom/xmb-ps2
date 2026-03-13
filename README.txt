============================================================
   TOKYO XMB - PS2 Homebrew Interface
   واجهة طوكيو XMB - هومبرو بلايستيشن 2
============================================================

--- الملفات ---

main.c       → البرنامج الرئيسي (نقطة البداية)
background.c → خلفية طوكيو: مباني + سيارات JDM + مطر + نجوم
background.h → تعريفات الخلفية
xmb.c        → نظام التنقل XMB (التصنيفات والعناصر)
xmb.h        → تعريفات XMB
input.c      → يد التحكم PS2
input.h      → تعريفات الإدخال
types.h      → أنواع البيانات الأساسية
Makefile     → ملف البناء

--- المتطلبات للترجمة ---

1. ps2sdk   : https://github.com/ps2dev/ps2dev
2. gsKit    : https://github.com/ps2dev/gsKit
3. Linux أو WSL على ويندوز

--- طريقة الترجمة ---

$ export PS2DEV=/usr/local/ps2dev
$ export PS2SDK=$PS2DEV/ps2sdk
$ export GSKIT=$PS2DEV/gsKit
$ export PATH=$PATH:$PS2DEV/bin:$PS2SDK/bin
$ make

الناتج: tokyo_xmb.elf

--- تشغيله على PS2 ---

1. يجب أن يكون PS2 مُعدَّل بـ FreeMCBoot
2. انسخ tokyo_xmb.elf إلى USB أو Memory Card
3. افتح wLaunchELF أو uLaunchELF
4. انتقل للملف وشغّله

--- التحكم ---

← → تبديل التصنيفات
↑ ↓ تبديل العناصر داخل التصنيف
O   تأكيد / فتح
X   رجوع
START + SELECT  خروج للقائمة

--- الميزات ---

✓ خلفية متحركة 60 إطار
✓ سيارات JDM تجري في شوارع طوكيو
✓ مطر + نجوم + قمر + مباني بنيون
✓ واجهة XMB أصيلة
✓ تعمل بدون إنترنت
✓ 7 تصنيفات: Users / Settings / Photo / Music / Video / Game / Network

--- ملاحظة ---

النصوص في PS2 تحتاج مكتبة font خارجية لعرض الخط العربي.
الكود الحالي يستخدم مستطيلات بدلاً من النص حتى تضيف مكتبة
الخط بنفسك (مثل gsKit_fontm أو مكتبة FreeType للـ PS2).

============================================================
