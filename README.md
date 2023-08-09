# BUILD INSTRUCTIONS
To compile this just use __build.bat__ for dynamic linking or __static_build.bat__ for static linking (all in one .exe).

Note that __-lgdi32__ and other libraries on some compilers must be after all *.cpp files!

Also you can add __-mwindows__ if you want to hide terminal on startup.

# HOW TO USE
Actually you can use any image of any format that is supported by [stb library (QUICK NOTES)](https://github.com/nothings/stb/blob/master/stb_image.h) as crosshair texture, just drag and drop it on menu window client area.

Note that texture dimensions should be even (but not necessary quad) to draw crosshair accurate in the center.

You can find some default crosshairs in assets directory.

