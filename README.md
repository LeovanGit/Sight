-lgdi32 обязательно должен быть после всех .cpp:
```
clear && g++ main.cpp window.cpp -lgdi32 -o sight && ./sight.exe
clear && g++ main.cpp window.cpp -lgdi32 -static-libgcc -static-libstdc++ -o sight
```
