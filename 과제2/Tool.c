#include "Header.h"

int power(int a, int b) { // 간단한 제곱 함수
    if (b == 0) return 1; // a^0 =1 : 초기 조건
    else return a * power(a, b-1); // 재귀
}

void printarr(int* arr, int len) { // arr를 조건에 맞게 출력
    for (int i=0; i<len-1; i++) printf("%d ", arr[i]); // 한 칸의 공백으로 띄어쓰기
    printf("%d", arr[len-1]); // 마지막 공백을 없애기 위함
    printf("\n"); // 줄바꿈
}

int getdeg(int last_page) { // last_page > 10^n 인 가장 큰 n을 반환
    int t = 0; // t를 더해가며 조건에 걸릴 때 그 값이 n
    for (t;; t++) {
        last_page /= 10; // 몫을 구하면서
        if (last_page == 0)  break; // 몫이 0이 되는 시점의 t가 곧 구하고자 하는 n
    }
    return t;
}
