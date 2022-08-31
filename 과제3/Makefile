cc  = g++ # C++ compiler: g++
cflags = -g -c # compiler option

target = main # file will be generated
objects = main.o str.o # object files

$(target) : $(objects)
	$(cc) -o $(target) $(objects) # complie, use macro

.PHONY : clean # 파일명, 명령어 중복을 대비한 phony
clean:
	rm -rf $(objects) $(target) # object files, target will be deleted

.cpp.o : # .o files depend on .cpp files
	$(cc) $(cflags) -c $< -o $@ # compile, use marco


main.o : Str.h  # dependency
str.o : Str.h  # dependency

