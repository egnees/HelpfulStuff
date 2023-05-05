#include "ZD_heruist_QAP.h"

#include <ctime>
#include <random>
#include <cstring>
#include <cassert>
#include <algorithm>

// Solution

ZD_heruist_QAP::Solution::Solution() : p{nullptr}, obv{-1}, size{0} {}

ZD_heruist_QAP::Solution::Solution(int* perm, int obvPerm, int n) : p{perm}, obv{obvPerm}, size{n} {}

ZD_heruist_QAP::Solution::Solution(const ZD_heruist_QAP::Solution& other) {
    p = new int[other.size]; 
    memcpy(p, other.p, other.size << 2); // assert that sizeof(int) == 4
    obv = other.obv;
    size = other.size;
}

ZD_heruist_QAP::Solution::Solution(ZD_heruist_QAP::Solution&& other) {
    p = other.p;
    other.p = nullptr;
    obv = other.obv;
}

ZD_heruist_QAP::Solution& ZD_heruist_QAP::Solution::operator=(const ZD_heruist_QAP::Solution& other) {
    // size == other.size
    memcpy(p, other.p, other.size << 2);
    obv = other.obv;
    return *this;
}

ZD_heruist_QAP::Solution& ZD_heruist_QAP::Solution::operator=(ZD_heruist_QAP::Solution&& other) {
    delete[] p;
    p = other.p;
    other.p = nullptr;
    obv = other.obv;
    return *this;
}

ZD_heruist_QAP::Solution::~Solution() {
    delete[] p;
}

// List

ZD_heruist_QAP::List::List(int k) : size{0}, worst{nullptr}, K{k} {
    a = new Solution*[K];
}

ZD_heruist_QAP::List::List(const ZD_heruist_QAP::List& other) {
    K = other.K;
    a = new Solution*[K];
    size = other.size;
    for (int i = 0; i < size; ++i) {
        a[i] = new Solution(*other.a[i]);
    }
    worst = a + (other.worst - other.a);
}

ZD_heruist_QAP::List::List(ZD_heruist_QAP::List&& other) {
    K = other.K;
    a = other.a;
    other.a = nullptr;
    other.worst = nullptr;
    other.size = 0;
}

ZD_heruist_QAP::List& ZD_heruist_QAP::List::operator=(const ZD_heruist_QAP::List& other) {
    delete[] a;
    K = other.K;
    a = new Solution*[K];
    size = other.size;
    for (int i = 0; i < size; ++i) {
        a[i] = new Solution(*other.a[i]);
    }
    worst = a + (other.worst - other.a);
    return *this;
}

ZD_heruist_QAP::List& ZD_heruist_QAP::List::operator=(ZD_heruist_QAP::List&& other) {
    delete[] a;
    K = other.K;
    a = other.a;
    other.a = nullptr;
    other.worst = nullptr;
    other.size = 0;
    return *this;
}

ZD_heruist_QAP::List::~List() {
    delete[] a;
}

void ZD_heruist_QAP::List::add(ZD_heruist_QAP::Solution* s) {
    a[size] = s;
    if (worst == nullptr || (*worst)->obv < s->obv) {
        worst = a + size;
    }
    ++size;
}

void ZD_heruist_QAP::List::clear() {
    worst = nullptr;
    size = 0;
}

// ZD_herist_QAP

ZD_heruist_QAP::ZD_heruist_QAP(const std::vector<std::vector<int>>& cost, const std::vector<std::vector<int>>& dist, int maxListSize) 
    : K{maxListSize}, n{(int) cost.size()} {
    
    C = new int*[n];
    D = new int*[n];
    
    for (int i = 0; i < n; ++i) {
        C[i] = new int[n];
        D[i] = new int[n];

        if (cost[i][i] != 0) {
            puts("cost-matrix is not zero-diagonal");
            break;
        }
        if (dist[i][i] != 0) {
            puts("dist-matrix is not zero-diagonal");
            break;
        }

        for (int j = 0; j < n; ++j) {
            C[i][j] = cost[i][j];
            D[i][j] = dist[i][j];

            if (cost[i][j] != cost[j][i]) {
                puts("cost-matrix is assymetrical");
                break;
            }
            if (dist[i][j] != dist[j][i]) {
                puts("dist-matrix is assymetrical");
                break;
            }

        }
    }
}

ZD_heruist_QAP::~ZD_heruist_QAP() {
    for (int i = 0; i < n; ++i) {
        delete[] C[i];
        delete[] D[i];
    }
    delete[] C;
    delete[] D;
}

std::vector<int> ZD_heruist_QAP::solve() {

    int* p = randPerm();
    Solution center = Solution(p, obv(p), n);
    Solution bfs = center;

    std::mt19937 rnd(time(0));

    int c = 0;

    while (1) {
        d = n - (rnd() % 3 + 2); // n-4 <= d <= n-2

        if (d <= 0) {
            d = 1;
        }

        Solution bfs2, bfs3;
        List memory;
        QAP_iter(center, bfs2, bfs3, memory); // find new solutions and write it to bfs2 and bfs3

        if (bfs.obv > bfs2.obv) { // found better solution
            c = 0;
            bfs = std::move(bfs2);
        }

        ++c;

        if (c == 1 || c == 3) {
            center = bestMemory(memory);
        } else if (c == 2 || c == 4) {
            center = std::move(bfs3);
        } else {
            break;
        }
    }

    return std::vector<int>(bfs.p, bfs.p + n);
}

ZD_heruist_QAP::Solution ZD_heruist_QAP::bestMemory(const List& memory) {
    assert(memory.size >= 1);

    int best = 0;

    for (int i = 1; i < memory.size; ++i) {
        if (memory.a[best]->obv > memory.a[i]->obv) {
            best = i;
        }
    }

    return *memory.a[best];
}

void ZD_heruist_QAP::QAP_iter(Solution& center, Solution& bfs, Solution& bfs2, List& memory) {

    int dp = 0; // distance between p and bfs

    List list0{K}; // best K permutations with dist = dp
    List list1{K}; // best K permutations with dist = dp+1 
    List list2{K}; // best K permutations with dist = dp+2

    list0.add(&center);

    bfs = center;

    while (dp <= d) {
        int prevObv = bfs.obv;

        newBfs(list0, bfs, bfs2); // find new solutions

        if (prevObv != bfs.obv) {
            // updated
            list1.clear();
            list2.clear();
            dp = 0;
        }

        updLists(list0, list1, list2, dp, bfs); // update list1, list2 and find new solutions
        memory = std::move(list0);

        if (list1.size == 0) {
            list0 = list2;
            list1.clear();
            list2.clear();
            ++dp;
        } else {
            list0 = std::move(list1);
            list1 = std::move(list2);
            list2 = List{K};
        }
        
        ++dp;
    }
}

void ZD_heruist_QAP::inlist(List& x, Solution* s) {
    for (int i = 0; i < x.size; ++i) {
        if (x.a[i]->obv == s->obv && deltaP(x.a[i]->p, s->p) == 0) {
            return;
        }
    }

    if (x.size < K) {
        x.add(s);
        return;
    }

    *x.worst = s;

    for (int i = 0; i < x.size; ++i) {
        if ((*x.worst)->obv < x.a[i]->obv) {
            x.worst = x.a + i;
        }
    }
}

void ZD_heruist_QAP::updLists(List& list0, List& list1, List& list2, int dp, const Solution& bfs) {
    for (int i = 0; i < list0.size; ++i) {

        Solution* curSol = list0.a[i];

        for (int j = 0; j + 1 < n; ++j) {
            for (int k = j + 1; k < n; ++k) {
                
                int deltaW = dp + deltadeltaP(curSol->p, bfs.p, j, k);
                if (deltaW <= 0) {
                    continue;
                }

                Solution* newSol = new Solution(*curSol);
                newSol->obv = deltaObv(j, k, curSol->p) + curSol->obv;
                std::swap(newSol->p[j], newSol->p[k]);

                if (deltaW == dp + 1) {
                    inlist(list1, newSol);
                } else if (deltaW == dp + 2) {
                    inlist(list2, newSol);
                } else {
                    puts("deltaW > 2");
                    delete newSol;
                }

            }
        }
    }
}

void ZD_heruist_QAP::newBfs(List& list0, Solution& bfs, Solution& bfs2) {

    // bfs inited, bfs may be not

    while (1) {

        int found = 0;
        
        for (int i = 0; i < list0.size; ++i) {
        
            Solution* curSol = list0.a[i];

            for (int j = 0; j + 1 < list0.size; ++j) {
                for (int k = j + 1; k < list0.size; ++k) {

                    int obvW = deltaObv(j, k, curSol->p) + curSol->obv;

                    if (obvW < bfs.obv) {
                        bfs = *curSol;
                        std::swap(bfs.p[j], bfs.p[k]);
                        bfs.obv = obvW;
                        found = 1;
                    } else if (bfs2.obv == -1 || obvW < bfs2.obv) {
                        bfs2 = *curSol;
                        std::swap(bfs2.p[j], bfs2.p[k]);
                        bfs2.obv = obvW;
                    }

                }
            }
        }

        if (found) {
            list0.clear();
            list0.add(&bfs);
        }
    }
}

int ZD_heruist_QAP::obv(int* w) const {
    int ret = 0;
    for (int i = 0; i + 1 < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            ret += C[i][j] * D[w[i]][w[j]];
        }
    }
    return ret;
}

int* ZD_heruist_QAP::randPerm() const {
    std::mt19937 rnd(time(0));
    int* p = new int[n];
    for (int i = 0; i < n; ++i) {
        p[i] = i;
    }
    std::shuffle(p, p + n, rnd);
    return p;
}

int ZD_heruist_QAP::deltaObv(int r, int s, int* w) const {
    int ret = 0;
    for (int i = 0; i < n; ++i) {
        if (i != r && i != s) {
            ret += (C[i][r] - C[i][s]) * (D[w[i]][w[s]] - D[w[i]][w[r]]);
        }
    }
    return ret;
}

int ZD_heruist_QAP::deltaP(int* p, int* w) const {
    int ret = 0;
    for (int i = 0; i < n; ++i) {
        if (p[i] != w[i]) {
            ++ret;
        }
    }
    return ret;
}

int ZD_heruist_QAP::deltadeltaP(int* w, int* bfs, int j, int k) const {
    int ret = 0;

    if (w[j] == bfs[j]) {
        ++ret;
    } else if (w[k] == bfs[j]) {
        --ret;
    }
    
    if (w[j] == bfs[k]) {
        ++ret;
    } else if (w[k] == bfs[k]) {
        --ret;
    }

    return ret;
}
