jamly: jamly.c
	gcc -o jamly -Wall -Wextra jamly.c -I./vendor/raylib-5.5_linux_amd64/include -I./vendor/nob -L./vendor/raylib-5.5_linux_amd64/lib -l:libraylib.a -lm
