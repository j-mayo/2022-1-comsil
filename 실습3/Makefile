cc = g++
cflags = -g -c

target = main
objects = main.o array.o rangearray.o

$(target) : $(objects)
	$(cc) -o $(target) $(objects)

.PHONY : clean
clean:
	rm -rf $(objects) $(target) *.gch

.cpp.o : 
	$(cc) $(cflags) -c $< -o $@

main.o : Array.h RangeArray.h
Array.o : Array.h
RangeArray.o : RangeArray.h
