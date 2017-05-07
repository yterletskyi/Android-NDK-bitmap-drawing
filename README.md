This code draws specific (x, y) pixel. So it's very small on a real screen.
Usually I do drawing by circles as following:

```C
void draw(jint stride, void *pixels, u_int32_t x, u_int32_t y, u_int32_t color, u_int32_t width) {
    for (jint yy = -width; yy <= width; yy++) {
        for (jint xx = -width; xx <= width; xx++) {
            if (yy * yy + xx * xx <= width * width) {
                pixels = (char *) pixels_array + y * stride;
                jint *pixel = ((u_int32_t *) pixels_void) + x;
                draw_pixel(pixel, color);
            }
        }
    }
}
```

In most of cases using NDK for drawing bitmaps is motivated by increasing speed.
This code doesn't provide us with quick drawing performance. So it's because we lock/unlock and finally redraw(from java code) bitmap for each (x, y) pixel (or more than one pixel).

But if we improve code to pass an array of points to be drawn to C function, we achieve stunning drawing speed.

For example:

Add one more function to manage drawing an array of ints(pixles):

```C
void draw_array(void *pixels, jint stride, jint *points_array, jint length, jint width, jint color) {
    jint *pixels_array = (jint *) pixels;
    for (jint i = 0; i < length; i++) {
        draw(pixels_array, stride, points_array[i], width, color);
    }
}
```

This way I reached the best drawing performance I've ever seen. It's really fast on large bitmaps and high screen densities.
