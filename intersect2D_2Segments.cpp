#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>

#define SMALL_NUM 0.00000001

using namespace std;

struct Point {
    float x, y;
};

struct Segment {
    Point P0, P1;
};

struct Vector {
    float x, y;
};

float dot(Vector u, Vector v) {
    return u.x * v.x + u.y * v.y;
}

float perp(Vector u, Vector v) {
    return u.x * v.y - u.y * v.x;
}

// inSegment(): 验证点是否在线段内
//    输入:  a point P, and a collinear segment S
//    返回值: 1 = P is inside S
//            0 = P is not inside S
int inSegment(Point P, Segment S) {
    if (S.P0.x != S.P1.x) {    // S is not vertical
        if ((S.P0.x <= P.x && P.x <= S.P1.x) || (S.P0.x >= P.x && P.x >= S.P1.x)) {
            return 1;
        }
    } else {    // S is vertical, so test y coordinate
        if ((S.P0.y <= P.y && P.y <= S.P1.y) || (S.P0.y >= P.y && P.y >= S.P1.y)) {
            return 1;
        }
    }
    return 0;
}

// intersect2D_2Segments():2D有限线段相交
//   输入：两条有限线段S1和S2
//   输出: *I0 = 相交点(如果存在)
//         *I1 = 相交线段的终点 [I0,I1] (如果存在)
//   返回值: 0=disjoint (no intersect)
//          1=intersect in unique point I0
//          2=overlap in segment from I0 to I1
int intersect2D_Segments(Segment S1, Segment S2, Point* I0, Point* I1) {
    Vector u = { S1.P1.x - S1.P0.x, S1.P1.y - S1.P0.y };
    Vector v = { S2.P1.x - S2.P0.x, S2.P1.y - S2.P0.y };
    Vector w = { S1.P0.x - S2.P0.x, S1.P0.y - S2.P0.y };
    float D = perp(u, v);

    // 检查两个线段是否平行，在线段平行的情况下，检查线段是否共线或者是否包含一个点。
    if (std::fabs(D) < SMALL_NUM) {
        if (perp(u, w) != 0 || perp(v, w) != 0) {
            return 0;
        }
        float du = dot(u, u);
        float dv = dot(v, v);

        if (du == 0 && dv == 0) {
            if (S1.P0.x != S2.P0.x || S1.P0.y != S2.P0.y) {
                return 0;
            }
            *I0 = S1.P0;
            return 1;
        }

        if (du == 0) {
            if (inSegment(S1.P0, S2) == 0) {
                return 0;
            }
            *I0 = S1.P0;
            return 1;
        }

        if (dv == 0) {
            if (inSegment(S2.P0, S1) == 0) {
                return 0;
            }
            *I0 = S2.P0;
            return 1;
        }

        float t0, t1;
        Vector w2 = { S1.P1.x - S2.P0.x, S1.P1.y - S2.P0.y };

        if (v.x != 0) {
            t0 = w.x / v.x;
            t1 = w2.x / v.x;
        } else {
            t0 = w.y / v.y;
            t1 = w2.y / v.y;
        }

        if (t0 > t1) {
            float t = t0;
            t0 = t1;
            t1 = t;
        }

        if (t0 > 1 || t1 < 0) {
            return 0;
        }

        t0 = t0 < 0 ? 0 : t0;
        t1 = t1 > 1 ? 1 : t1;

        if (t0 == t1) {
            *I0 = { S2.P0.x + t0 * v.x, S2.P0.y + t0 * v.y };
            return 1;
        }

        *I0 = { S2.P0.x + t0 * v.x, S2.P0.y + t0 * v.y };
        *I1 = { S2.P0.x + t1 * v.x, S2.P0.y + t1 * v.y };
        return 2;
    }

    float sI = perp(v, w) / D;
    if (sI < 0 || sI > 1) {
        return 0;
    }

    float tI = perp(u, w) / D;
    if (tI < 0 || tI > 1) {
        return 0;
    }

    I0->x = S1.P0.x + sI * u.x;
    I0->y = S1.P0.y + sI * u.y;

    return 1;
}

int main(int argc, char const *argv[])
{
    Segment S1 = { {0, 0}, {5, 5} };
    Segment S2 = { {0, 2}, {3, 0} };
    Point I0, I1;

    int result = intersect2D_Segments(S1, S2, &I0, &I1);

    if (result == 0) {
        std::cout << "No intersection." << std::endl;
    } else if (result == 1) {
        std::cout << "Intersection at (" << I0.x << ", " << I0.y << ")." << std::endl;
    } else {
        std::cout << "Segments overlap from (" << I0.x << ", " << I0.y << ") to (" << I1.x << ", " << I1.y << ")." << std::endl;
    }
    return 0;
}
