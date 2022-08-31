#include "Array.h"
#include <cstdlib>
#include<iostream>
using namespace std;
Array::Array(int size)
{
	//사이즈를 확인하고 양수이면 new를 사용하여 배열 data를 할당, len값 초기화
	if(size<=0)
	{
		cout << "Array size error!" << endl; // Error message! (size)
		exit(1); // End program
	}
	else
	{
		data = new int[size]; // init the data: memomry allocate
		len = size; // init the len
	}
}
Array::~Array() // destructor
{
	delete[] data; // delete the memory
}
int Array::length() const // return the length of array
{
	return len;
}

// 배열에 원소를 대입하거나 값을 반환하는 부분으로 []연산자의 오버로딩이다
int& Array::operator[](int i) // insert the element in the array[i]
{
	static int tmp;

	// 배열의 인덱스가 범위 내에 있으면 값 리턴, 그렇지 않으면 에러메세지 출력하고 tmp리턴
	if(i >= 0 && i < len)
	{
		return *(data + i); // return the address of data[i]
	}
	else
	{
		cout<<" Array bound error!"<<endl; // Error message!
		return tmp;
	}
}

int Array::operator[](int i) const // return the element of array
{
	//배열의 인덱스가 범위 내에 있으면 값을 리턴 	
    if(i >= 0 && i<len)
	{
		return data[i]; // return data[i]
	}
	else // 그렇지 않으면 에러메세지 출력하고 0을 리턴
	{
		cout<<"Array bound error!"<<endl;; // Error message!
		return 0;
	}
}
void Array::print() // print the array
{
	int i;
	cout<<" ["; // 먼저 ' [' 출력 후
	for (i = 0; i < len-1; i++) {
		cout << data[i] << " "; // 공백, data[i] 를 출력
	}
	cout << data[i] << "]"; // 마지막에 형식 맞추기 위해 data[i]와 ']' 출력
		
	cout<<endl;
}
