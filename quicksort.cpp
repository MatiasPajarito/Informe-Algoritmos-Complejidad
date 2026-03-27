#include <vector>
#include <cstdlib>
#include <random>

using namespace std;

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(vector<int>& array, int low, int high) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> distrib(low, high);
    
    int random_pivot_index = distrib(gen);
    int pivot = array[random_pivot_index];
    
    // Particionamiento de Hoare (Mucho más veloz y robusto con números duplicados)
    int i = low - 1;
    int j = high + 1;
    
    while (true) {
        // Encontrar elemento a la izquierda que sea mayor o igual al pivote
        do {
            i++;
        } while (array[i] < pivot);
        
        // Encontrar elemento a la derecha que sea menor o igual al pivote
        do {
            j--;
        } while (array[j] > pivot);
        
        // Si los índices se cruzan, terminamos de particionar
        if (i >= j) {
            return j;
        }
        
        swap(&array[i], &array[j]);
    }
}

void quickSort(vector<int>& array, int low, int high) {
    // Tail Call Optimization manual
    while (low < high) {
        int pi = partition(array, low, high);
        
        // Recursión en el pedazo más pequeño para evitar Stack Overflow
        // Nota: en Hoare, la partición incluye 'pi', por lo que los rangos son [low, pi] y [pi+1, high]
        if (pi - low < high - (pi + 1)) {
            quickSort(array, low, pi);
            low = pi + 1;
        } else {
            quickSort(array, pi + 1, high);
            high = pi;
        }
    }
}

void run_quicksort(vector<int>& arr) {
    if (!arr.empty()) {
        quickSort(arr, 0, arr.size() - 1);
    }
}