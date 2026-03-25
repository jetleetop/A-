#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>

using namespace std;

int solution(int N, int M, int K, vector<int> A) {
    int max_day_time = 0;
    int current_day_time = 0;
    int solved_count = 0;
    int easy_stack = 0;

    for (int i = 0; i < N; i++) {
        double time_to_add = 0;

        // 1. 난이도 판별 및 소요 시간 계산
        if (A[i] > 2 * M) {
            time_to_add = A[i] * 2.0;
            M++; // 어려운 문제를 풀면 실력 상승!
        }
        else if (2 * A[i] >= M) {
            time_to_add = (double)A[i];
        }
        else {
            easy_stack++; // 너무 쉬운 문제: 지루함 스택 증가
            continue;
        }

        // 2. 지루함 패널티 적용 (3개 이상 쌓였을 때 이번 문제 시간 10% 증가)
        if (easy_stack >= 3) {
            time_to_add *= 1.1;
            easy_stack = 0; // 패널티 적용 후 초기화
        }

        current_day_time += (int)round(time_to_add);
        solved_count++;

        // 3. 하루 정산
        if (solved_count == K) {
            max_day_time = max(max_day_time, current_day_time);
            current_day_time = 0;
            solved_count = 0;
            easy_stack = 0; // 자고 일어나면 지루함 초기화
        }
    }

    if (solved_count > 0) {
        max_day_time = max(max_day_time, current_day_time);
    }

    return max_day_time;
}

void test(int id, int N, int M, int K, vector<int> A, int expected) {
    int result = solution(N, M, K, A);
    if (result == expected) {
        cout << "Test " << id << ": Success (Result: " << result << ")" << endl;
    }
    else {
        cout << "Test " << id << ": Fail (Result: " << result << ", Expected: " << expected << ")" << endl;
    }
}

int main() {
    // Case 1: 실력 상승의 효과
    // M=2로 시작. 10은 2M(4)보다 크므로 20분 소요 & M=3으로 상승.
    // 다음 10은 2M(6)보다 크므로 20분 소요 & M=4로 상승. 
    // 하루에 2문제씩 풀면 1일차 합계 40.
    test(1, 4, 2, 2, { 10, 10, 10, 10 }, 40);

    // Case 2: 지루함 패널티 (10% 증가)
    // M=10, K=1. 앞의 0, 0, 0은 모두 너무 쉬워 건너뜀 (easy_stack = 3).
    // 마지막 20을 풀 때 패널티 10% 적용되어 20 * 1.1 = 22분 소요.
    test(2, 4, 10, 1, { 0, 0, 0, 20 }, 22);

    // Case 3: 실력 상승으로 인해 '도움되는 문제'가 '쉬운 문제'로 변하는 경우
    // M=5, K=2. 
    // A[0]=11: 2배(22분), M=6
    // A[1]=11: 2배(22분), M=7 -> 1일차: 44분
    // A[2]=3: 2*3 >= 7(참) -> 3분
    // A[3]=3: 2*3 < 8(거짓, M이 7일 때 기준) -> M이 상승했다면 여기서 '쉬움'으로 판정될 수 있음
    test(3, 4, 5, 2, { 11, 11, 3, 3 }, 44);

    return 0;
}