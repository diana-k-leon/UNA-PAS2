// ============================================================
// 05-pshift-con-cambio.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 5 — Pitch Shifting: efecto ardillita
// ============================================================
//
// Subir el pitch de una sinusoide usando interpolación + diezmado.
// La duración cambia — ese es el efecto ardillita clásico.
//
// Parámetros:
//   M = factor de interpolación (insertar muestras)
//   L = factor de diezmado (tirar muestras)
//   pitch neto = L/M  (L > M → pitch sube, señal se acorta)
//
// Ejemplo con M=2, L=3:
//   Interpolar ×2 → señal más larga, pitch baja
//   Diezmar ÷3   → señal se acorta más, pitch sube
//   Neto: pitch × (3/2) = quinta arriba, duración × (2/3)
//
// Compilar:  g++ -std=c++17 05-pshift-con-cambio.cpp ../dsp_utils.cpp -o pshift-con-cambio
// Ejecutar:  ./pshift-con-cambio
//
// Graficar espectros:
// gnuplot -e "set datafile separator ','; set title 'Pitch Shift: original vs shifted'; set xrange [0:3000]; plot 'pshift-espectro.csv' using 1:2 with lines lw 2 title 'original', 'pshift-espectro.csv' using 1:3 with lines lw 2 title 'shifted'; pause -1"
//
// ============================================================

#include "../dsp_utils.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

int main()
{
    // ─── parámetros ───────────────────────────────────────────────────────
    const int   N          = 2048;       // potencia de 2 para FFT
    const float sampleRate = 44100.0f;
    const float f0         = 440.0f;
    const int   M          = 2;          // factor de interpolación
    const int   L          = 3;          // factor de diezmado
    // pitch neto × (L/M) = × 1.5 → quinta arriba
    // duración  × (M/L) = × 0.67 → señal más corta

    std::cout << "=== Pitch Shifting con cambio de duración ===" << std::endl;
    std::cout << "Señal original: " << f0 << " Hz" << std::endl;
    std::cout << "M=" << M << " (interpolar), L=" << L << " (diezmar)" << std::endl;
    std::cout << "Pitch esperado: " << f0 * L / M << " Hz" << std::endl;
    std::cout << "Duración: × " << (float)M / L << " (señal se acorta)" << std::endl;
    std::cout << std::endl;

    // ─── paso 1: generar señal original ───────────────────────────────────
    std::vector<float> original(N);
    for (int n = 0; n < N; n++)
        original[n] = std::sin(2.0f * PI * f0 * n / sampleRate);

    std::cout << "✓ Señal generada: " << f0 << " Hz, " << N << " samples" << std::endl;

    // ─── paso 2: interpolar ×M ───────────────────────────────────────────────────
    // el array interpolado tiene M veces más huecos que el original. 
    int N_interp = (N - 1) * M + 1;
    std::vector<float> interpolada(N_interp);

    // va hasta N-2
    for (int i = 0; i < N - 1; i++) {
        interpolada[i * M] = original[i];           // muestra original
        // loop interno que llena los huecos
        for (int m = 1; m < M; m++) {
            // peso: 0 → 1
            float t = (float)m / M;                  
            // mezcla la muestra actual con la siguiente según t
            interpolada[i * M + m] = (1.0f - t) * original[i]
                                    + t * original[i + 1];
        }
    }
    // la última muestra nunca entra al loop, se copia sola al final.
    interpolada[N_interp - 1] = original[N - 1];    

    std::cout << "✓ Interpolada ×" << M << ": " << N_interp << " samples" << std::endl;

    // ─── paso 3: diezmar ÷L ───────────────────────────────────────────────
    // Tomar cada L-ésima muestra de la señal interpolada.
    int N_out = N_interp / L;
    std::vector<float> resultado(N_out);

    for (int i = 0; i < N_out; i++)
        resultado[i] = interpolada[i * L];           // 1 de cada L muestras

    std::cout << "✓ Diezmada ÷" << L << ": " << N_out << " samples" << std::endl;

    // ─── paso 4: calcular espectros ───────────────────────────────────────
    // Usar N samples de cada señal para que los bins coincidan.
    int N_fft = std::min(N, N_out);
    std::vector<float> orig_fft(original.begin(), original.begin() + N_fft);
    std::vector<float> res_fft(resultado.begin(), resultado.begin() + N_fft);

    auto magOriginal  = magnitudes(orig_fft);
    auto magResultado = magnitudes(res_fft);

    std::cout << "✓ Espectros calculados" << std::endl;

    // ─── paso 5: exportar ─────────────────────────────────────────────────
    std::ofstream file("pshift-espectro.csv");
    file << "frecuencia_hz,original,shifted" << std::endl;
    for (int k = 0; k < N_fft / 2; k++) {
        float freq = k * sampleRate / N_fft;
        file << freq << "," << magOriginal[k] << "," << magResultado[k] << std::endl;
    }
    file.close();

    std::cout << "✓ Exportado a pshift-espectro.csv" << std::endl;
    std::cout << std::endl;
    std::cout << "Qué esperar en el espectro:" << std::endl;
    std::cout << "  Original:  pico en " << f0 << " Hz" << std::endl;
    std::cout << "  Resultado: pico en " << f0 * L / M << " Hz" << std::endl;
    std::cout << std::endl;
    std::cout << "Probá:" << std::endl;
    std::cout << "  M=1, L=2 → octava arriba  (880 Hz), señal mitad de larga" << std::endl;
    std::cout << "  M=2, L=1 → octava abajo   (220 Hz), señal doble de larga" << std::endl;
    std::cout << "  M=2, L=3 → quinta arriba  (660 Hz)" << std::endl;

    return 0;
}