// ============================================================
// 03-3-dft-centrada.cpp
// Programación Aplicada al Sonido II — UNA
// Clase 2: DFT centrada alrededor de cero
// ============================================================
//
// Igual que ejemplo-dft-real.cpp pero los índices van de
// -N/2 a N/2 — los dos picos aparecen en k=7 y k=-7,
// más intuitivo que 7 y 57.
//
// Compilar:  clang++ 03-3-dft-centrada.cpp -o dft-centrado
// Ejecutar:  ./dft-centrado
// ============================================================

#include <iostream>
#include <vector>
#include <complex>
#include <cmath>

int main() {

    // ----------------------------------------------------------
    // SEÑAL DE ENTRADA — coseno (señal real)
    // ----------------------------------------------------------
    int N = 64;
    int k0 = 7;

    std::vector<int> nv;
    std::vector<int> kv;
    for (int i = -N/2; i < N/2; i++) {
        nv.push_back(i);
        kv.push_back(i);
    }

    std::vector<std::complex<float>> x;
    for (int n : nv) {
        float valor = std::cos(2.0f * M_PI * k0 * n / N);
        x.push_back(std::complex<float>(valor, 0));
    }

    // ----------------------------------------------------------
    // DFT centrada
    // ----------------------------------------------------------
    std::vector<std::complex<float>> X;

    for (int k : kv) {
        std::complex<float> suma(0, 0);
        for (int i = 0; i < N; i++) {
            float exponent = 2.0f * M_PI * k * nv[i] / N;
            std::complex<float> s = std::exp(std::complex<float>(0, exponent));
            suma += x[i] * std::conj(s);
        }
        X.push_back(suma);
    }

    // ----------------------------------------------------------
    // MOSTRAR RESULTADO
    // ----------------------------------------------------------
    std::cout << "Bin\tMagnitud" << std::endl;
    for (int i = 0; i < N; i++) {
        float magnitud = std::abs(X[i]);
        if (magnitud > 0.5f) {
            std::cout << kv[i] << "\t" << magnitud << std::endl;
        }
    }

    return 0;
}
