#include "Header.h"
// 필요한 걸 모은 헤더 파일 include
void main() {
    int T; // 테게 개수를 받는 int T
    scanf("%d", &T); // 테케 개수
    int* iter = (int*)malloc(T * sizeof(int)); // 만큼 배열 동적 할당
    for (int i=0; i<T; i++) {
        scanf("%d", &iter[i]); //할당한 배열에 각 태케의 페이지를 받음
    }
    for (int i=0; i<T; i++) { 
        int arr[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // arr 초기화
        int deg = getdeg(iter[i]); // 마지막 페이지의 자릿수-1을 구함
        get_n_pgnum(arr, deg);  // 1~10^deg-1까지 나오는 숫자들에 대해 갱신
        int x_0 = power(10, deg); // 10^deg
        get_XY_pagenum(arr, x_0, iter[i], 0); // 10^deg부터 마지막 페이지까지 나오는 숫자들에 대해 갱신 후 
        printarr(arr, 10); // 출력!
    }
}
