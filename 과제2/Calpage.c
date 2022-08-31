#include "Header.h"


void get_a_pgnum(int* arr, int X, int step) { // X페이지의 숫자 개수 갱신
    while (1) { 
        arr[X % 10] += power(10, step); // 마지막 자리 수 갱신
        // get_XY_pagenum에서 쓰이므로 step에 따라 수가 달라짐
        X /= 10; // 다음 자리로 넘어가기
        if (X == 0) { // 다 했으면
            break; //끝!
        }
    }
}

void get_n_pgnum(int* arr, int deg) { // 1~10^(deg)-1까지 있는 숫자 구하기 
    if (deg == 0) return; // 10^0=0
    else if (deg == 1) { // 10^1=10
        for (int i = 1; i < 10; i++) { // 1~9는 1~9가 한 번씩만 등장
            arr[i] = 1;
        }
        arr[0] = 0; // 0은 1~9 사이에 없음 ㅠ
    }
    else {
        get_n_pgnum(arr, deg - 1); // 재귀(top-down)
        arr[0] = 9 * arr[1] + arr[0]; // 1~10^(deg)-1의 페이지들에 대해
        // t=0일때의 arr[t]의 일반식
        // a[0]_i = 9a[t]_i (t != 0) + a[0]_(i-1)
        for (int i = 1; i < 10; i++) {
            arr[i] = arr[i] * 10 + power(10, deg - 1);
            // 1~10^(deg)-1의 페이지들에 대해 t!=0일때 arr[t]의 일반식
            // a[i]_t = 10a[i-1]_t + 10^(i-1) if t != 0
        }
        
    }
}

void get_XY_pagenum(int* arr, int X, int Y, int step) {
    // X, Y의 자릿수는 같게 세팅되어 입력됨
    // X ~ Y의 모든 숫자를 세어 갱신

    if (X / 10 == 0) {  // X, Y의 자릿수가 같으니 X의 몫만 살펴봐도 됨
        for (int t = X; t < Y + 1; t++) {
            arr[t] += power(10, step); // 몫이 0이면, 마지막 남은 숫자들을 처리
        }
        return; // 끝!
    }

    // X, Y의 끝자리를 0, 9로 맞추는 작업
    int i;
    int x = 10 - X % 10; int y = Y % 10; // 빼줘야 할 수
    if (X % 10 != 0) { // X의 끝자리각 0이 아니면
        for (i = 0; i < x; i++) {
            get_a_pgnum(arr, X, step); // 더하기 전에 현재 X에 나오는 숫자들 갱신
            X++; // 더하기
        }
    }
    if (Y % 10 != 9) { // Y의 끝자리가 9가 아니면
        for (i = 0; i < y + 1; i++) {
            get_a_pgnum(arr, Y, step); // 빼주기 전에 현재 y에 나오는 숫자들 갱신
            Y--; // 빼주기
        }
    }
    if (X > Y) { return; } // X>Y이면 전부 봤으니 종료
    int temp = (Y / 10 - X / 10 + 1) * power(10, step);
    // 끝자리가 맞춰진 X, Y에서 X~Y의 사의의 수들의 일의 자리만 고려할 시
    // 0~9는 정확히 같은 수만큼 등장하고 그 양은 위와 같다
    // step은 재귀를 돌린 횟수
    for (i = 0; i < 10; i++) {
        arr[i] += temp; // arr 갱신
    }

    get_XY_pagenum(arr, X / 10, Y / 10, step + 1); // 재귀

}
