#pragma once

#include <vector>

class ZD_heruist_QAP {
public:
    /// @brief Z. Drezner, A New Heuristic for the Quadratic Assignment Problem implementation.
    /// @brief Journal of Applied Mathematics and Decision Sciences, 6(3), 2002, 143-153.
    /// @param cost is symmetric zero-diagonal matrix.
    /// @param dist is symmetric zero-diagonal matrix.
    /// @param maxListSize is the maximum size of list0, list1, list2.
    ZD_heruist_QAP(const std::vector<std::vector<int>>& cost, const std::vector<std::vector<int>>& dist, int maxListSize);

    /// @brief solves QAP problem using Z. Drezner heuristic
    /// @return permutation \param p where i-th facility assigned to p[i]-th position.
    std::vector<int> solve();

private:
    ~ZD_heruist_QAP();

    int** C;
    int** D;
    int K;
    int d;

    int n;

    struct Solution {
        int* p;
        int obv;
        int size;

        Solution();
        Solution(int* perm, int obvPerm, int n);
        Solution(const Solution& other);
        Solution(Solution&& other);
        Solution& operator=(const Solution& other);
        Solution& operator=(Solution&& other);
        ~Solution();
    };

    struct List {
        Solution** a;
        int size;
        Solution** worst;
        int K;

        void add(Solution* s);
        void clear();

        List() = default;
        explicit List(int k);
        List(const List& other);
        List(List&& other);
        List& operator=(const List& other);
        List& operator=(List&& other);
        ~List();
    };

    int obv(int* w) const;
    int* randPerm() const;

    int deltaObv(int r, int s, int* w) const;
    int deltaP(int* p, int* w) const;
    int deltadeltaP(int* w, int* bfs, int j, int k) const;

    void newBfs(List& list0, Solution& bfs, Solution& bfs2);
    void updLists(List& list0, List& list1, List& list2, int dp, const Solution& bfs);
    void inlist(List& x, Solution* s);
    void QAP_iter(Solution& center, Solution& bfs, Solution& bfs2, List& memory);

    Solution bestMemory(const List& memory);
};
