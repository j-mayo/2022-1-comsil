#include "Str.h"
#include <cstdlib>
#include <iostream>
#include <string.h>

Str::Str(int leng) // constructor
{
    if (leng > 0) { // if len is positive integer
        str = new char[leng+1];  // allocate memory to str, use new char[]
        len = leng; // init n
		for (int i=0; i<len; str[i++]=' ')
			; // len은 항상 str의 길이를 가지고 있어야 하니 공백 문자열로 초기화해줌
		str[len] = '\0'; // end of string
    }
    else { // if len is not the positive integer
        // Print the error message
        std::cout << "SizeError: String size is not positive integer" << std::endl;
        exit(1); // exit the program
    }
}

Str::Str(const char *neyong) // constructor
{
    len = strlen(neyong); // strlen을 통해 문자열의 길이를 알아내 len 초기화
    str = new char[len+1]; // str을 new를 통해 동적 할당
    strcpy(str, neyong); // str에 neyong 문자열을 복사
}

Str::~Str() { // deconstructor
    delete[] str; // str에 할당한 메모리를 해제
}

int Str::length(void) { return len; } // return the length(len) of Str

char* Str::contents(void) { return str; } // return the string(str) of Str

int Str::compare(class Str& a) { return strcmp(str, a.contents()); }
// return the value of strcmp(str, a.contents())

int Str::compare(const char *a) { return strcmp(str, a); }
// return the value of strcmp(str, a)

void Str::operator=(const char *a) 
{
    len = strlen(a); // change the value of len = strlen(a)
    delete[] str; // new는 재할당이 불가능하니 먼저 str에 할당된 메모리를 해제
    str = new char[len+1]; // len의 길이를 가진 문자열을 저장하기 위해 다시 동적 할당
    strcpy(str, a); // str에 a를 복사
}
    
void Str::operator=(class Str& a)
{
    len = a.length(); // change the value of len = len of a
    delete[] str; // 먼저 str에  할당된 메모리를 해제
    str = new char[len+1]; // len의 길이를 가진 문자열을 저장하기 위해 다시 동적 할당
    strcpy(str, a.contents()); // str에 a의 내용을 복사
}
