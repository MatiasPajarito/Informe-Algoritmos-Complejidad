#include <vector>
using namespace std;

// Función auxiliar para sumar dos matrices
void sumar_matrices(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

// Función auxiliar para restar dos matrices
void restar_matrices(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
}

// Función auxiliar de multiplicación tradicional para el caso base optimizado
vector<vector<int>> multiplicacion_tradicional(const vector<vector<int>>& A, const vector<vector<int>>& B, int n) {
    vector<vector<int>> C(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n; j++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

// La función recursiva de Strassen real
vector<vector<int>> strassen_recursivo(const vector<vector<int>>& A, const vector<vector<int>>& B, int n) {
    // Caso base OPTIMIZADO: Si n es pequeño, usar multiplicación iterativa normal
    // Evita la creación de cientos de millones de ramas recursivas y vectores de 1x1.
    // Reducción del árbol de recursión de ~282,000,000 de llamadas a solo ~2400 llamadas.
    if (n <= 64) {
        return multiplicacion_tradicional(A, B, n);
    }

    int k = n / 2;
    vector<int> inner(k);
    
    // Declaración de submatrices
    vector<vector<int>> A11(k, inner), A12(k, inner), A21(k, inner), A22(k, inner);
    vector<vector<int>> B11(k, inner), B12(k, inner), B21(k, inner), B22(k, inner);

    // Dividir las matrices A y B en 4 submatrices
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + k];
            A21[i][j] = A[i + k][j];
            A22[i][j] = A[i + k][j + k];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + k];
            B21[i][j] = B[i + k][j];
            B22[i][j] = B[i + k][j + k];
        }
    }

    // Matrices temporales para los 7 productos de Strassen
    vector<vector<int>> S1(k, inner), S2(k, inner), S3(k, inner), S4(k, inner), S5(k, inner), S6(k, inner), S7(k, inner);
    vector<vector<int>> S8(k, inner), S9(k, inner), S10(k, inner);
    vector<vector<int>> P1(k, inner), P2(k, inner), P3(k, inner), P4(k, inner), P5(k, inner), P6(k, inner), P7(k, inner);

    restar_matrices(B12, B22, S1, k);
    sumar_matrices(A11, A12, S2, k);
    sumar_matrices(A21, A22, S3, k);
    restar_matrices(B21, B11, S4, k);
    sumar_matrices(A11, A22, S5, k);
    sumar_matrices(B11, B22, S6, k);
    restar_matrices(A12, A22, S7, k);
    sumar_matrices(B21, B22, S8, k);
    restar_matrices(A11, A21, S9, k);
    sumar_matrices(B11, B12, S10, k);

    // Llamadas recursivas
    P1 = strassen_recursivo(A11, S1, k);
    P2 = strassen_recursivo(S2, B22, k);
    P3 = strassen_recursivo(S3, B11, k);
    P4 = strassen_recursivo(A22, S4, k);
    P5 = strassen_recursivo(S5, S6, k);
    P6 = strassen_recursivo(S7, S8, k);
    P7 = strassen_recursivo(S9, S10, k);

    // Calcular las submatrices de resultado C
    vector<vector<int>> C11(k, inner), C12(k, inner), C21(k, inner), C22(k, inner);
    vector<vector<int>> tempA(k, inner), tempB(k, inner);

    // C11 = P5 + P4 - P2 + P6
    sumar_matrices(P5, P4, tempA, k);
    restar_matrices(tempA, P2, tempB, k);
    sumar_matrices(tempB, P6, C11, k);

    // C12 = P1 + P2
    sumar_matrices(P1, P2, C12, k);

    // C21 = P3 + P4
    sumar_matrices(P3, P4, C21, k);

    // C22 = P5 + P1 - P3 - P7
    sumar_matrices(P5, P1, tempA, k);
    restar_matrices(tempA, P3, tempB, k);
    restar_matrices(tempB, P7, C22, k);

    // Unir las 4 submatrices en la matriz final C
    vector<vector<int>> C(n, vector<int>(n));
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            C[i][j] = C11[i][j];
            C[i][j + k] = C12[i][j];
            C[i + k][j] = C21[i][j];
            C[i + k][j + k] = C22[i][j];
        }
    }

    return C;
}

vector<vector<int>> run_strassen(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n = A.size();
    return strassen_recursivo(A, B, n);
}