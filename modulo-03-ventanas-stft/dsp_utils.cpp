// ============================================================
// dsp_utils.cpp
// Programación Aplicada al Sonido II — UNA
// Implementación de utilidades compartidas
// ============================================================

#include "dsp_utils.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <algorithm>

const float PI = M_PI;

// ─── FFT (Cooley-Tukey) ──────────────────────────────────────────────────────

std::vector<std::complex<float>> fft(std::vector<std::complex<float>> signal) {
    int N = signal.size();
    if (N <= 1) return signal;
    std::vector<std::complex<float>> pares(N / 2), impares(N / 2);
    for (int i = 0; i < N / 2; i++) {
        pares[i]   = signal[2 * i];
        impares[i] = signal[2 * i + 1];
    }
    auto FFT_pares   = fft(pares);
    auto FFT_impares = fft(impares);
    std::vector<std::complex<float>> resultado(N);
    for (int k = 0; k < N / 2; k++) {
        std::complex<float> twiddle = std::polar(1.0f, -2.0f * PI * k / N) * FFT_impares[k];
        resultado[k]         = FFT_pares[k] + twiddle;
        resultado[k + N / 2] = FFT_pares[k] - twiddle;
    }
    return resultado;
}

// ─── aplicar ventana ──────────────────────────────────────────────────────────
std::vector<float> aplicarVentana(const std::vector<float>& signal,
                                   const std::vector<float>& ventana) {
    std::vector<float> resultado(signal.size());
    for (int n = 0; n < (int)signal.size(); n++)
        resultado[n] = signal[n] * ventana[n];
    return resultado;
}

// ─── magnitudes ───────────────────────────────────────────────────────────────
// Recibe una señal (un array de floats, como una sinusoide de 440 Hz) 
// y te devuelve otro array donde cada posición te dice cuánta energía hay en cada frecuencia. 
// Eso es lo que graficás.
std::vector<float> magnitudes(const std::vector<float>& signal) {
    
    int N = signal.size();
    
    // Paso 1: convertir señal real → compleja
    // La FFT necesita números complejos. Nuestra señal es solo floats,
    // así que ponemos cada sample como parte real y 0 como imaginaria.
    std::vector<std::complex<float>> compleja(N);
    for (int n = 0; n < N; n++)
        compleja[n] = std::complex<float>(signal[n], 0.0f);

    
    // Paso 2: calcular FFT
    // Devuelve N números complejos — cada uno es un bin de frecuencia
    auto espectro = fft(compleja);

    // Paso 3: extraer magnitudes (solo primera mitad — hasta Nyquist)
    // La segstd::vector<float> mag(N / 2);una mitad es espejo, no aporta info nueva. (1) 
    std::vector<float> mag(N / 2);
    for (int k = 0; k < N / 2; k++)
        // std::abs() en un complejo = sqrt(real² + imag²) = cuánta energía hay
        // Dividimos por N para normalizar
        mag[k] = std::abs(espectro[k]) / N;
    return mag;
}
/* (1) - ¿Por qué N/2 y no N? 
Porque el espectro de una señal real es simétrico — 
la segunda mitad es espejo de la primera. 
Solo la primera mitad (hasta Nyquist) tiene información útil. 
Si N = 1024, te quedás con 512 bins de frecuencia. 
*/


// ─── espectro de una ventana en dB ────────────────────────────────────────────

WindowSpectrum windowSpectrum(const std::vector<float>& ventana, int fftSize) {
    int M = ventana.size();
    int halfM = M / 2;

    // zero-phase windowing
    std::vector<std::complex<float>> buffer(fftSize, {0.0f, 0.0f});
    for (int i = 0; i < M - halfM; i++)
        buffer[i] = std::complex<float>(ventana[halfM + i], 0.0f);
    for (int i = 0; i < halfM; i++)
        buffer[fftSize - halfM + i] = std::complex<float>(ventana[i], 0.0f);

    // FFT
    auto espectro = fft(buffer);

    // magnitud en dB
    float epsilon = 1e-10f;
    std::vector<float> magDB(fftSize);
    for (int k = 0; k < fftSize; k++) {
        float mag = std::abs(espectro[k]);
        magDB[k] = 20.0f * std::log10(std::max(mag, epsilon));
    }

    // normalizar a 0 dB
    float maxDB = *std::max_element(magDB.begin(), magDB.end());
    for (int k = 0; k < fftSize; k++)
        magDB[k] -= maxDB;

    // fftshift (centrar)
    std::vector<float> centered(fftSize);
    for (int k = 0; k < fftSize; k++)
        centered[k] = magDB[(k + fftSize / 2) % fftSize];

    // medir lóbulo principal
    int center = fftSize / 2;
    int firstNull = center;
    for (int k = center + 1; k < fftSize - 1; k++) {
        if (centered[k] > centered[k - 1]) {
            firstNull = k - 1;
            break;
        }
    }
    int halfWidth = firstNull - center;
    int mainLobeWidth = (int)std::round(2.0f * halfWidth * (float)M / fftSize);

    // medir lóbulo lateral más alto
    float highestSidelobe = -200.0f;
    for (int k = firstNull + 1; k < fftSize; k++) {
        if (centered[k] > highestSidelobe)
            highestSidelobe = centered[k];
    }
    int firstNullLeft = center - (firstNull - center);
    for (int k = 0; k < firstNullLeft; k++) {
        if (centered[k] > highestSidelobe)
            highestSidelobe = centered[k];
    }

    return { centered, mainLobeWidth, highestSidelobe };
}

// ─── exportar espectro de ventana a CSV ───────────────────────────────────────

void exportarWindowSpectrum(const std::string& filename,
                            const std::string& columnName,
                            const WindowSpectrum& ws,
                            int M, int fftSize, int rangeBins) {
    std::ofstream file(filename);
    file << "bin," << columnName << "_dB" << std::endl;
    int center = fftSize / 2;
    int range  = (int)((float)rangeBins * fftSize / M);
    for (int k = center - range; k <= center + range; k++) {
        float bin = (float)(k - center) * M / fftSize;
        file << bin << "," << ws.dB[k] << std::endl;
    }
    file.close();
}
