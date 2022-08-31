cc=gcc
cflags = -W -g
target = main 
objects = main.o Tool.o Calpage.o # 매크로들

$(target) : $(objects)
	$(cc) $(cflags) -o $(target) $(objects) #매크로를 사용한 표현

%.o : %.c # SUFFIX rule
	$(cc) $(cflags) -c -o $@ $<

main.o : Header.h # 의존관계
Tool.o : Header.h
Calpage.o : Header.h

.PHONY : clean  # 파일명 중복을 대비하는 phony
clean :
	rm $(target) $(objects)
