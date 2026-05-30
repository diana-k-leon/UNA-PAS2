// ============================================================
// 01-filtro-idft.cpp
// Programación Aplicada al Sonido II — UNA
// Módulo 4 — Filtro pasa-bajos con DFT/IDFT
// ============================================================
//
// Tu primer procesamiento de audio en frecuencia:
//   1. Generar señal con dos sinusoides: 440 Hz + 4000 Hz
//   2. FFT → espectro
//   3. Borrar todos los bins por encima de 2000 Hz
//   4. IFFT → señal filtrada
//   5. Exportar ambas a CSV y comparar
//
// Qué vas a ver: la señal original tiene dos frecuencias.
// La señal filtrada tiene solo 440 Hz — los 4000 Hz desaparecieron.
//
// Compilar:  g++ -std=c++17 01-filtro-idft.cpp dsp_utils.cpp -o filtro-idft
// Ejecutar:  ./filtro-idft
//
// Graficar señales (tiempo):
// gnuplot -e "set datafile separator ','; set title 'Original vs Filtrada'; set xrange [0:200]; plot 'filtro-idft.csv' using 1:2 with lines lc rgb '#FF0000' lw 1 title 'original (440+4000 Hz)', 'filtro-idft.csv' using 1:3 with lines lc rgb '#00AA00' lw 2 title 'filtrada (solo 440 Hz)'; pause -1"
//
// Graficar espectros:
// gnuplot -e "set datafile separator ','; set title 'Espectro antes y después'; set xrange [0:6000]; plot 'filtro-idft-espectro.csv' using 1:2 with lines lw 2 title 'original', 'filtro-idft-espectro.csv' using 1:3 with lines lw 2 title 'filtrada'; pause -1"
//
// ============================================================

#include "dsp_utils.h"
#include <iostream>
#include <cmath>
#include <fstream>

int main() {
    // ─── parámetros ───────────────────────────────────────────────────────
    int N            = 2048;        // potencia de 2
    float sampleRate = 44100.0f;
    float f1         = 440.0f;      // LA — frecuencia que queremos mantener
    float f2         = 4000.0f;     // frecuencia que queremos borrar
    float fCorte     = 2000.0f;     // frecuencia de corte del filtro

    std::cout << "=== Filtro pasa-bajos con DFT/IDFT ===" << std::endl;
    std::cout << "Señal: " << f1 << " Hz + " << f2 << " Hz" << std::endl;
    std::cout << "Corte: " << fCorte << " Hz (todo lo de arriba se borra)" << std::endl;
    std::cout << std::endl;

    // ─── paso 1: generar señal con dos frecuencias ────────────────────────
    std::vector<float> signal(N);
    for (int n = 0; n < N; n++)
        signal[n] = std::sin(2.0f * PI * f1 * n / sampleRate)
                  + std::sin(2.0f * PI * f2 * n / sampleRate);

    std::cout << "✓ Señal generada: " << f1 << " Hz + " << f2 << " Hz" << std::endl;

    // ─── paso 2: DFT (FFT) ───────────────────────────────────────────────
    // convertir a complejo
    std::vector<std::complex<float>> compleja(N);
    for (int n = 0; n < N; n++)
        compleja[n] = std::complex<float>(signal[n], 0.0f);

    // calcular FFT
    auto espectro = fft(compleja);

    std::cout << "✓ FFT calculada" << std::endl;

    // ─── paso 3: modificar espectro — borrar frecuencias altas ────────────
    // ¿qué bin corresponde a 2000 Hz?
    int binCorte = (int)(fCorte * N / sampleRate);

    std::cout << "  Bin de corte: " << binCorte << " (corresponde a " << fCorte << " Hz)" << std::endl;

    // guardar espectro original para comparar
    std::vector<float> magOriginal(N / 2);
    for (int k = 0; k < N / 2; k++)
        // cada bin es un número complejo (magnitud + fase). 
        // abs sobre un complejo devuelve la magnitud: √(real² + imaginario²). 
        // "cuánta energía hay en esa frecuencia"
        magOriginal[k] = std::abs(espectro[k]) / N; //dividido N — normalización

    // borrar bins por encima del corte (frecuencias positivas)
    for (int k = binCorte; k < N - binCorte; k++)
        espectro[k] = std::complex<float>(0.0f, 0.0f);

    // espectro filtrado para comparar
    std::vector<float> magFiltrada(N / 2);
    for (int k = 0; k < N / 2; k++)
        magFiltrada[k] = std::abs(espectro[k]) / N;

    std::cout << "✓ Espectro modificado: bins " << binCorte << " a " << (N - binCorte) << " en cero" << std::endl;

    // ─── paso 4: IDFT (IFFT) ─────────────────────────────────────────────
    auto resultado = ifft(espectro);

    // extraer parte real (la señal reconstruida)
    std::vector<float> filtrada(N);
    for (int n = 0; n < N; n++)
        filtrada[n] = resultado[n].real();

    std::cout << "✓ IFFT calculada — señal reconstruida" << std::endl;

    // ─── paso 5: exportar ─────────────────────────────────────────────────

    // señales en el tiempo
    std::ofstream file1("filtro-idft.csv");
    file1 << "sample,original,filtrada" << std::endl;
    for (int n = 0; n < N; n++)
        file1 << n << "," << signal[n] << "," << filtrada[n] << std::endl;
    file1.close();

    // espectros
    std::ofstream file2("filtro-idft-espectro.csv");
    file2 << "frecuencia_hz,original,filtrada" << std::endl;
    for (int k = 0; k < N / 2; k++) {
        float freq = k * sampleRate / N;
        file2 << freq << "," << magOriginal[k] << "," << magFiltrada[k] << std::endl;
    }
    file2.close();

    std::cout << "✓ Exportado a filtro-idft.csv y filtro-idft-espectro.csv" << std::endl;
    std::cout << std::endl;
    std::cout << "Qué esperar:" << std::endl;
    std::cout << "- Original: onda compleja (440 + 4000 Hz mezclados)" << std::endl;
    std::cout << "- Filtrada: sinusoide limpia de 440 Hz (los 4000 Hz desaparecieron)" << std::endl;

    return 0;
}
