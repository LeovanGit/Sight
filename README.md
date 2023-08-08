# BUILD INSTRUCTIONS
To compile this just use __build.bat__ or you can compile it yourself smth like this:
* for dynamic linking:
```cpp
g++ source/main.cpp source/window.cpp source/translucent_window.cpp source/menu_window.cpp source/texture.cpp -lgdi32 -lmsimg32 -o crosshair -DLOGS -mwindows
```
* for static linking:
```cpp
g++ source/main.cpp source/window.cpp source/translucent_window.cpp source/menu_window.cpp source/texture.cpp -lgdi32 -lmsimg32 -static-libgcc -static-libstdc++ -static -lpthread -o crosshair -DLOGS -mwindows
```

Note that __-lgdi32__ and other libraries on some compilers must be after all *.cpp files!

Also you can add __-mwindows__ if you want to hide terminal on startup.

# HOW TO USE
Actually you can use any image of any format that is supported by [stb library (QUICK NOTES)](https://github.com/nothings/stb/blob/master/stb_image.h) as crosshair texture, just drag and drop it on menu window client area.

Note that texture dimensions should be even (but not necessary quad) to draw crosshair accurate in the center.

You can find some default crosshairs in assets directory.

# TODO
* Write own image loader library
