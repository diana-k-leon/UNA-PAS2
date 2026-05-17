// ============================================================
// 01-sinusoide-real.cpp
// Programación Aplicada al Sonido II — UNA
// Unidad 1 - sinusoides
// ============================================================
// Genera una onda senoidal y la guarda en un archivo CSV
// para poder visualizarla en Google Sheets o Excel.
// Compilar:  g++ -std=c++17  01-sinusoide-real.cpp -o sinusoide-real
// Ejecutar:  ./sinusoide-real
// ============================================================
// x[n] = A · cos(2π · f₀/SR · n + φ)
// Esta sinusoide real es la señal de entrada que vamos a analizar con la DFT.
// ----------------------------------------------------------
// EXPORTAR A CSV
// El CSV — útil para debugging en JUCE. Cuando no sabés si tu buffer tiene los valores correctos, 
// ----------------------------------------------------------
// Exportar a CSV nos permite visualizar la señal en cualquier
// herramienta externa — Google Sheets, Excel, Python, MATLAB.
// Es una técnica útil para debugging: cuando trabajés con
// audio en JUCE, podés exportar buffers y verificar que
// los valores son los que esperás antes de mandarlos al speaker.
// Graficar:  gnuplot -e "set datafile separator ','; plot 'sinusoide.csv' using 1:2 with lines title 'señal'; pause -1"
// ----------------------------------------------------------

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

int main() {

    // ----------------------------------------------------------
    // PARÁMETROS DE LA SINUSOIDE
    // ----------------------------------------------------------
    float A   = 0.8f;           // amplitud
    float f0  = 1000.0f;        // frecuencia en Hz
    float phi = M_PI / 2;       // fase
    float fs  = 44100.0f;       // sample rate

    // ----------------------------------------------------------
    // GENERAR LA SEÑAL
    // ----------------------------------------------------------
    std::vector<float> t;
    std::vector<float> x;

    for (float ti = -0.002f; ti < 0.002f; ti += 1.0f / fs) {
        t.push_back(ti);
        x.push_back(A * std::cos(2 * M_PI * f0 * ti + phi));
    }

    // ----------------------------------------------------------
    // EXPORTAR A CSV
    // ----------------------------------------------------------
    std::ofstream file("sinusoide.csv");
    //file << "tiempo,amplitud" << std::endl;
    for (int i = 0; i < (int)x.size(); i++) {
        file << t[i] << "," << x[i] << std::endl;
    }
    file.close();

    std::cout << "Generadas " << x.size() << " muestras" << std::endl;
    std::cout << "Guardado en sinusoide.csv" << std::endl;

    return 0;
}
