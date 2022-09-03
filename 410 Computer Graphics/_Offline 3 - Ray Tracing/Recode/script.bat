set objectFile=%1.o
set cppFile=%1.cpp
set exeFile=%1.exe 

g++ -c -o %objectFile% %cppFile% -I"D:\CodeBlocks\MinGW\include"
g++ -o %exeFile% %objectFile% -L"D:\CodeBlocks\MinGW\lib" -lopengl32 -lglu32 -lfreeglut

%exeFile%