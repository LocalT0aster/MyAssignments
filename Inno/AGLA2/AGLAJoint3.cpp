// Code by @NeRaG0n7 (Danil Nesterov d.nesterov@innopolis.university)
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
// GNU plot
#ifdef WIN32
#define GNUPLOT_NAME "C:\\gnuplot\\bin\\gnuplot -persist"
#else
#define GNUPLOT_NAME "gnuplot -persist"
#endif

using namespace std;

void
predator_prey_model(double v0, double k0, double alpha1, double beta1, double alpha2, double beta2, double T, int N, FILE* pipe = nullptr) {
    vector<double> t(N + 1), v(N + 1), k(N + 1);
    double dt = T / N;

    // Initialize
    t[0] = 0;
    v[0] = v0 - (alpha2 / beta2);
    k[0] = k0 - (alpha1 / beta1);

    // Time stepping
    for (int i = 1; i <= N; ++i) {
        t[i] = t[i - 1] + dt;
        v[i] = v[0] * ::cos(::sqrt(alpha1 * alpha2) * t[i]) -
               (k[0] * ::sqrt(alpha2) * beta1 * ::sin(::sqrt(alpha1 * alpha2) * t[i])) /
               (beta2 * ::sqrt(alpha1));

        k[i] = (v[0] * ::sqrt(alpha1) * beta2 * ::sin(::sqrt(alpha1 * alpha2) * t[i])) /
                (beta1 * ::sqrt(alpha2)) +
                k[0] * ::cos(::sqrt(alpha1 * alpha2) * t[i]);
    }

    // Output
    cout << fixed << setprecision(2);
    cout << "t:" << endl;
    for (const auto &ti: t) {
        cout << ti << " ";
    }
    cout << endl << "v:" << endl;
    for (const auto &vi: v) {
        cout << vi + (alpha2 / beta2) << " ";
    }
    cout << endl << "k:" << endl;
    for (const auto &ki: k) {
        cout << ki + (alpha1 / beta1) << " ";
    }
    cout << endl;

    if (pipe == nullptr) return;

    fprintf(pipe, "plot '-' using 1:2 t 'data1' with lines, '' using 1:2 t 'data2' with lines\n");
    for(int i = 0; i < N; i++){
        fprintf(pipe, "%f\t%f\n", t[i], v[i] + (alpha2 / beta2));
    }
    fprintf(pipe, "e\n");
    for(int i = 0; i < N; i++){
        fprintf(pipe, "%f\t%f\n", t[i], k[i] + (alpha1 / beta1));
    }
    fprintf(pipe, "e\n");
    fflush(pipe);
}

int main() {
#ifdef WIN32
    FILE* pipe = _popen(GNUPLOT_NAME, "w");
#else
    FILE* pipe = popen(GNUPLOT_NAME, "w");
#endif
    double v0, k0, alpha1, beta1, alpha2, beta2, T;
    int N;
    cin >> v0 >> k0 >> alpha1 >> beta1 >> alpha2 >> beta2 >> T >> N;

    predator_prey_model(v0, k0, alpha1, beta1, alpha2, beta2, T, N, pipe);

#ifdef WIN32
    _pclose(pipe);
#else
    pclose(pipe);
#endif
    return 0;
}

