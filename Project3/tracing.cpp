#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>

using namespace std;

const int MAP_SIZE = 4;
const int WALL = 1;
const int ROAD = 0;

vector<vector<int>> grid(MAP_SIZE, vector<int>(MAP_SIZE, ROAD));

struct NODE {
    int x, y;
    int g; // 출발지에서 현재까지 온 거리
    int h; // 현재에서 목적지까지 남은 예상 거리
    NODE* parent; // 경로 역추적을 위한 이전 노드 포인터

    // 생성자: 초기화를 편하게!
    NODE(int _x, int _y, int _g, int _h, NODE* _p = nullptr)
        : x(_x), y(_y), g(_g), h(_h), parent(_p) {
    }

    // f = g + h 계산 함수
    int getF() const { return g + h; }

    // 우선순위 큐에서 '작은 f값'이 먼저 나오게 하기 위한 비교 연산자
    // std::priority_queue는 기본적으로 '큰 값'이 우선이므로, 
    // > 연산자를 오버로딩하여 반대로 동작하게 만듭니다.
    bool operator>(const NODE& other) const {
        return getF() > other.getF();
    }
};

int calculateH(int x, int y, int gx, int gy) {
    // 맨해튼 거리: 가로 차이 + 세로 차이
    return abs(x - gx) + abs(y - gy);
}
int main() {
    int goalX = 3, goalY = 3; // 4x4 맵이므로 마지막 인덱스는 3

    // 방향 배열
    int dx[] = { 0, 1, 0, -1 };
    int dy[] = { -1, 0, 1, 0 };

    priority_queue<NODE, vector<NODE>, greater<NODE>> pq;
    bool visited[MAP_SIZE][MAP_SIZE] = { false, };

    // 경로 역추적을 위해 할당된 노드들을 관리할 벡터 (나중에 메모리 해제용)
    vector<NODE*> memoryPool;

    // 시작 노드 생성
    NODE* startNode = new NODE(0, 0, 0, calculateH(0, 0, goalX, goalY), nullptr);
    memoryPool.push_back(startNode);
    pq.push(*startNode);

    while (!pq.empty()) {
        NODE current = pq.top();
        pq.pop();

        if (visited[current.y][current.x]) continue;
        visited[current.y][current.x] = true;

        if (current.x == goalX && current.y == goalY) {
            cout << "목적지 도착! 총 비용: " << current.g << endl;

            // 역추적 로직
			vector<pair<int, int>> path;
			NODE* traceNode = &current;
			while (traceNode) {
				path.push_back({ traceNode->x, traceNode->y });
				traceNode = traceNode->parent;
			}
			reverse(path.begin(), path.end());
			cout << "경로: ";
			for (const auto& p : path) {
				cout << "(" << p.first << ", " << p.second << ") ";
			}
			cout << endl;
            break;
        }

        for (int i = 0; i < 4; i++) {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];

            if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE &&
                grid[ny][nx] == ROAD && !visited[ny][nx]) {

                int nextG = current.g + 1;
                int nextH = calculateH(nx, ny, goalX, goalY);

                // 부모 노드 정보를 유지하기 위해 현재 노드를 복사해서 풀에 저장
                // (실제로는 더 복잡한 관리가 필요하지만, 지금은 핵심만!)
                NODE* parentForNext = new NODE(current);
                memoryPool.push_back(parentForNext);

                pq.push(NODE(nx, ny, nextG, nextH, parentForNext));
            }
        }
    }

    // [중요] 사용한 메모리 해제 (임베디드 시니어의 필수 습관)
    for (auto node : memoryPool) delete node;

    return 0;
}