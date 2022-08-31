#include <stdio.h>
#include <stdlib.h>
// 필요한 라이브러리

int power(int a, int b);
// a^b를 계산해주는 함수
void printarr(int* arr, int len);
// arr를 형식에 맞게 출력해주는 함수
int getdeg(int last_page);
// last_page > 10^n인 가장 큰 n을 구해주는 함수
void get_a_pgnum(int* arr, int X, int step);
// X페이지에 있는 숫자의 개수를 arr에 갱신해주는 함수
void get_n_pgnum(int* arr, int deg);
// 1 ~ 10^deg-1 페이지에 있는 숫자의 개수를 arr에 갱신해주는 함수
void get_XY_pagenum(int* arr, int X, int Y, int step);
// 같은 자리수인 X와 Y페이지 사이에 있는 숫자의 개수를 arr에 갱신해주는 재귀를 사용한 함수
