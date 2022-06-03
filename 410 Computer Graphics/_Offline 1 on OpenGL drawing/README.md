# Setting up OPENGL with freeglut

Setting up OPENGL basically means adding some files under your compiler's directory and windows directory. The required files can be found in *../freeglut* and *../GL files* and can also be downloaded from [this link](https://www.transmissionzero.co.uk/software/freeglut-devel/).

1. Find where your compiler (MinGW) is.

2. Copy and paste all the *freeglut/include* and *freeglut/lib* files in your *MinGW/include* and *MinGW/lib* folder respectively.

> *freeglut/include* -> *MinGW/include*

> *freeglut/lib* -> *MinGW/lib*


3. *.dll* files represent windows files. Every such file should go into *Windows/System* and/or *Windows/System32*. If 64-bit files are used, then paste the *.dll* files under *Windows/SysWOW64* too.

> *freeglut/bin* -> *Windows/System* and *Windows/System32*

> *GL Files/dll* -> *Windows/System* and *Windows/System32*



# Writing Code

- Just include these two header files and you're good to go

```cpp
#include <windows.h>
#include <glut.h>
```

- If ```include <glut.h>``` gives an error, try ```include<GL/glut.h>```

- Remember to include **windows.h** **before** **glut.h**



# Compiling the Code

- Use the following two commands to run a file named **file.cpp**

```bash
g++ -c -o file.o file.cpp -I"...\MinGW\include"

g++ -o file.exe file.o -L"...\MinGW\lib" -lopengl32 -lglu32 -lfreeglut
```

- ```...\MinGW\include``` and ```...\MinGW\bin``` should be edited properly to point to the path of your gcc/g++ compiler

- The first command creates and object file and the second command creates an executable file. So, after executing the two commands, there should be two new files named ```file.o``` and ```file.exe```

- The two commands can be used to create the following **script.bat** batch file

```bash
set objectFile=%1.o
set cppFile=%1.cpp
set exeFile=%1.exe 

g++ -c -o %objectFile% %cppFile% -I"...\MinGW\include"
g++ -o %exeFile% %objectFile% -L"...\MinGW\lib" -lopengl32 -lglu32 -lfreeglut

%exeFile%
```

- To compile and run a file named **file.cpp**, execute the following command
```bash
> ./script.bat file
```