// ============================================================
// 03-3-dft-centrada.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 2: DFT centrada alrededor de cero
// ============================================================
//
// Compilar:  g++ -std=c++17 03-3-dft-centrada.cpp -o dft-centrada
// Ejecutar:  ./dft-centrada
// ============================================================
// La diferencia clave respecto a la básica está en la señal de entrada:
// antes — sinusoide compleja
// senal[muestra] = std::complex<float>(std::cos(angulo), std::sin(angulo));
// ahora — coseno, parte imaginaria = 0
// senal[muestra] = std::complex<float>(std::cos(angulo), 0.0f);
// Para que hacemos esto > Para conectar con el audio real.
/* En el ejemplo anterior usamos una sinusoide compleja — que no existe en la naturaleza, es sólo una herramienta matemática. 
Ningún micrófono te da números complejos. Un micrófono te da floats — valores reales. 
Un coseno con parte imaginaria = 0 es exactamente eso: una señal real. 
Es el puente entre el ejemplo de laboratorio y el audio del mundo real. Y la consecuencia 
importante: con señal real siempre aparecen dos picos — en bin 7 y en bin 57. 
Eso es lo que van a ver siempre cuando analicen audio de verdad.
*/
#include <iostream>
#include <vector>
#include <complex>
#include <cmath>

int main() {

    // ----------------------------------------------------------
    // PARÁMETROS
    // ----------------------------------------------------------
    int N  = 64;
    int k0 = 7;

    // índices centrados — de -N/2 a N/2-1
    std::vector<int> indices(N);
    for (int i = 0; i < N; ++i) {
        indices[i] = i - N / 2;
    }

    // ----------------------------------------------------------
    // SEÑAL DE ENTRADA — coseno (señal real)
    // ----------------------------------------------------------
    std::vector<std::complex<float>> senal(N);
    for (int i = 0; i < N; ++i) {
        float angulo = 2.0f * M_PI * k0 * indices[i] / N;
        senal[i] = std::complex<float>(std::cos(angulo), 0.0f);
    }

    // ----------------------------------------------------------
    // DFT centrada — mismo algoritmo, índices centrados
    // ----------------------------------------------------------
    std::vector<std::complex<float>> espectro(N);
    for (int i = 0; i < N; ++i) {
        int bin = indices[i];
        std::complex<float> suma(0.0f, 0.0f);
        for (int j = 0; j < N; ++j) {
            float angulo = 2.0f * M_PI * bin * indices[j] / N;
            std::complex<float> referencia(std::cos(angulo), std::sin(angulo));
            suma += senal[j] * std::conj(referencia);
        }
        espectro[i] = suma;
    }

    // ----------------------------------------------------------
    // RESULTADO
    // ----------------------------------------------------------
    // Resultado esperado: picos en -7 y +7, magnitud ≈ 32 cada uno
    // Mismo resultado que dft-real — solo cambia cómo se presentan

    std::cout << "Bin\tMagnitud\n";
    for (int i = 0; i < N; ++i) {
        float magnitud = std::abs(espectro[i]);
        if (magnitud > 0.5f) {
            std::cout << indices[i] << "\t" << magnitud << "\n";
        }
    }

    return 0;
}

// ----------------------------------------------------------
// PARA EXPLORAR
// ----------------------------------------------------------
// Los picos aparecen en -k0 y +k0 — simétricos alrededor de cero
// Esta es la convención que vas a ver en matplotlib, MATLAB y plugins