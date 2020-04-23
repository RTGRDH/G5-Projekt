build:
	gcc -Wfatal-errors \
	-std=c99 \
	./*.c \
	-I"C:\C:\sdl2\SDL32Bit\include" \
	-L"C:\sdl2\SDL32Bit\lib" \
	-lmingw32 \
	-lSDL2main \
	-lSDL2 \
	-lSDL2_image \
	-lSDL2_net \
	-lSDL2_ttf \
	-o luffare.exe
