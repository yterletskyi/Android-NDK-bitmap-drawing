#include <jni.h>
#include <stdlib.h>
#include <android/bitmap.h>

void draw_color(u_int32_t *pixel, u_int32_t color) {
    uint32_t alpha = (color & 0xFF000000) >> 24;
    uint32_t blue = (color & 0x00FF0000) >> 16;
    u_short green = (color & 0x0000FF00) >> 8;
    u_char red = color & 0x00000FF;
    *pixel = (alpha << 24) | (red << 16) | (green << 8) | blue;
}

void draw(jint stride, void *pixels, u_short x, u_short y, u_int32_t color) {
    pixels = (char *) pixels + y * stride;
    u_int32_t *pixel = ((u_int32_t *) pixels) + x;
    draw_color(pixel, color);
}

JNIEXPORT void JNICALL
Java_com_example_plasma_DrawingView_drawPixel(JNIEnv *env, jobject instance, jobject bmp,
                                              u_int32_t x,
                                              u_int32_t y,
                                              u_int32_t color) {

    AndroidBitmapInfo info;
    void *pixels;

    if (AndroidBitmap_getInfo(env, bmp, &info) < 0) {
        return;
    }

    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        return;
    }

    if (AndroidBitmap_lockPixels(env, bmp, &pixels) < 0) {
        return;
    }

    draw(info.stride, pixels, x, y, color);

    AndroidBitmap_unlockPixels(env, bmp);

}