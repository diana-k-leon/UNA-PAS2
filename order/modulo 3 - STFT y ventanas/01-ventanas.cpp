#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <fstream>

const float PI = M_PI;

// ─── ventanas ─────────────────────────────────────────────────────────────────
std::vector<float> hamming(int N) {
    std::vector<float> w(N);
    for (int n = 0; n < N; n++)
        w[n] = 0.54f - 0.46f * std::cos(2.0f * PI * n / (N - 1));
    return w;
}

std::vector<float> hann(int N) {
    std::vector<float> w(N);
    for (int n = 0; n < N; n++)
        w[n] = 0.5f - 0.5f * std::cos(2.0f * PI * n / (N - 1));
    return w;
}

std::vector<float> bartlett(int N) {
    std::vector<float> w(N);
    for (int n = 0; n < N; n++)
        w[n] = 1.0f - std::abs(2.0f * n / (N - 1) - 1.0f);
    return w;
}

std::vector<float> blackman(int N) {
    std::vector<float> w(N);
    for (int n = 0; n < N; n++)
        w[n] = 0.42f
             - 0.5f  * std::cos(2.0f * PI * n / (N - 1))
             + 0.08f * std::cos(4.0f * PI * n / (N - 1));
    return w;
}

// ─── aplicar ventana ──────────────────────────────────────────────────────────
std::vector<float> aplicarVentana(const std::vector<float>& signal,
                                   const std::vector<float>& ventana) {
    std::vector<float> resultado(signal.size());
    for (int n = 0; n < (int)signal.size(); n++)
        resultado[n] = signal[n] * ventana[n];
    return resultado;
}

// ─── FFT ──────────────────────────────────────────────────────────────────────
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

// ─── calcular magnitudes ──────────────────────────────────────────────────────
std::vector<float> magnitudes(const std::vector<float>& signal) {
    int N = signal.size();
    std::vector<std::complex<float>> compleja(N);
    for (int n = 0; n < N; n++)
        compleja[n] = std::complex<float>(signal[n], 0.0f);
    auto espectro = fft(compleja);
    std::vector<float> mag(N / 2);
    for (int k = 0; k < N / 2; k++)
        mag[k] = std::abs(espectro[k]) / N;
    return mag;
}

int main() {
    // ─── parámetros ───────────────────────────────────────────────────────────
    int N            = 1024;
    float sampleRate = 44100.0f;
    float f0         = 440.0f;      // probá con 441.0 para ver leakage

    // ─── generar señal ────────────────────────────────────────────────────────
    std::vector<float> signal(N);
    for (int n = 0; n < N; n++)
        signal[n] = std::sin(2.0f * PI * f0 * n / sampleRate);

    // ─── calcular espectros con cada ventana ──────────────────────────────────
    auto sinVentana      = magnitudes(signal);
    auto conHamming      = magnitudes(aplicarVentana(signal, hamming(N)));
    auto conHann         = magnitudes(aplicarVentana(signal, hann(N)));
    auto conBartlett     = magnitudes(aplicarVentana(signal, bartlett(N)));
    auto conBlackman     = magnitudes(aplicarVentana(signal, blackman(N)));

    // ─── exportar CSV ─────────────────────────────────────────────────────────
    std::ofstream file("ventanas.csv");
    file << "bin,frecuencia_hz,sin_ventana,hamming,hann,bartlett,blackman" << std::endl;
    for (int k = 0; k < N / 2; k++) {
        float freq = k * sampleRate / N;
        file << k << ","
             << freq        << ","
             << sinVentana[k] << ","
             << conHamming[k] << ","
             << conHann[k]    << ","
             << conBartlett[k]<< ","
             << conBlackman[k]<< std::endl;
    }
    file.close();

    std::cout << "Exportado a ventanas.csv" << std::endl;
    std::cout << "Tip: cambiá f0 a 441.0 Hz para ver el efecto del leakage" << std::endl;

    return 0;
}
