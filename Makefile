bin: main.c
	@gcc -std=c99 -pedantic -Wall \
             -F/Library/Frameworks -rpath /Library/Frameworks -framework SDL2 \
             -framework OpenGL \
	     -o bin \
	     main.c
run: bin
	@./bin
clean:
	@rm -f bin
