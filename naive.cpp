#include <vector>
using namespace std;

vector<vector<int>> run_naive(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n = A.size();
    
    vector<vector<int>> R(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                R[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return R;
}