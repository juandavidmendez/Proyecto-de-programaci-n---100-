#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>
#include <sstream>

using namespace std;

// Excepción personalizada
class ExcepcionConsumo : public exception {
private:
    string mensaje;

public:
    explicit ExcepcionConsumo(const string& mensaje) : mensaje(mensaje) {}

    const char* what() const noexcept override {
        return mensaje.c_str();
    }
};

// Clase base abstracta: ArtefactoBase
class ArtefactoBase {
protected:
    string nombre;
    double consumo_por_minuto_watts;

public:
    ArtefactoBase(const string& nombre, double consumo_por_minuto_watts)
        : nombre(nombre), consumo_por_minuto_watts(consumo_por_minuto_watts) {}

    virtual ~ArtefactoBase() = default;

    string getNombre() const { return nombre; }
    double getConsumoPorMinuto() const { return consumo_por_minuto_watts; }

    virtual double calcularConsumoMensualKW(double minutos, int dias) const = 0;
    virtual double calcularCosto(double minutos, int dias, double tarifa) const = 0;
    virtual void mostrarConsumo(double minutos, int dias, double tarifa) const = 0;

    friend class ReporteConsumo;
};

// Clase derivada: Artefacto
class Artefacto : public ArtefactoBase {
public:
    Artefacto(const string& nombre, double consumo_por_minuto_watts)
        : ArtefactoBase(nombre, consumo_por_minuto_watts) {}

    double calcularConsumoMensualKW(double minutos, int dias) const override {
        if (minutos < 0 || dias < 0) {
            throw ExcepcionConsumo("Tiempo de uso o días no pueden ser negativos.");
        }
        return consumo_por_minuto_watts * minutos * dias / 1000; // En kWh
    }

    double calcularCosto(double minutos, int dias, double tarifa) const override {
        return calcularConsumoMensualKW(minutos, dias) * tarifa;
    }

    void mostrarConsumo(double minutos, int dias, double tarifa) const override {
        double consumo_kw = calcularConsumoMensualKW(minutos, dias);
        double costo = calcularCosto(minutos, dias, tarifa);

        cout << fixed << setprecision(2);
        cout << "Artefacto: " << nombre << "\n";
        cout << "Consumo mensual: " << consumo_kw << " kWh\n";
        cout << "Costo mensual: " << costo << " $\n";
    }
};

// Clase derivada: ArtefactoInteligente
class ArtefactoInteligente : public ArtefactoBase {
private:
    double eficiencia; // Factor de eficiencia

public:
    ArtefactoInteligente(const string& nombre, double consumo_por_minuto_watts, double eficiencia)
        : ArtefactoBase(nombre, consumo_por_minuto_watts), eficiencia(eficiencia) {}

    double calcularConsumoMensualKW(double minutos, int dias) const override {
        return (consumo_por_minuto_watts * minutos * dias * eficiencia) / 1000;
    }

    double calcularCosto(double minutos, int dias, double tarifa) const override {
        return calcularConsumoMensualKW(minutos, dias) * tarifa;
    }

    void mostrarConsumo(double minutos, int dias, double tarifa) const override {
        double consumo_kw = calcularConsumoMensualKW(minutos, dias);
        double costo = calcularCosto(minutos, dias, tarifa);

        cout << fixed << setprecision(2);
        cout << "Artefacto Inteligente: " << nombre << "\n";
        cout << "Consumo mensual (eficiencia aplicada): " << consumo_kw << " kWh\n";
        cout << "Costo mensual: " << costo << " $\n";
    }

    friend void ajustarEficiencia(ArtefactoInteligente& artefacto, double nuevaEficiencia);
};

// Función amiga para ajustar la eficiencia
void ajustarEficiencia(ArtefactoInteligente& artefacto, double nuevaEficiencia) {
    artefacto.eficiencia = nuevaEficiencia;
}

// Clase amiga: ReporteConsumo
class ReporteConsumo {
public:
    // Generar un reporte detallado de los artefactos
    static void generarReporte(const vector<ArtefactoBase*>& artefactos, 
                               const vector<double>& minutos_uso, const vector<int>& dias_uso, double tarifa) {
        double total_kw = 0, total_costo = 0;

        cout << "\n--- Reporte de Consumos ---\n";
        for (size_t i = 0; i < artefactos.size(); ++i) {
            double consumo_kw = artefactos[i]->calcularConsumoMensualKW(minutos_uso[i], dias_uso[i]);
            double costo = artefactos[i]->calcularCosto(minutos_uso[i], dias_uso[i], tarifa);

            cout << "Artefacto: " << artefactos[i]->getNombre() << "\n";
            cout << "Consumo mensual: " << consumo_kw << " kWh\n";
            cout << "Costo mensual: " << costo << " $\n\n";

            total_kw += consumo_kw;
            total_costo += costo;
        }

        cout << "Consumo total mensual: " << total_kw << " kWh\n";
        cout << "Costo total mensual: " << total_costo << " $\n";
    }
};

// Clase GestorArtefactos
class GestorArtefactos {
private:
    vector<ArtefactoBase*> disponibles;

public:
    GestorArtefactos() {
        disponibles = {
            new Artefacto("Bombilla LED", 0.15),
            new Artefacto("Ventilador", 0.8),
            new Artefacto("Televisor LED", 1.6),
            new Artefacto("Refrigerador", 8.5),
            new Artefacto("Microondas", 10.0),
            new Artefacto("Licuadora", 5.0),
            new Artefacto("Cafetera", 3.5),
            new Artefacto("Plancha", 12.0),
            new ArtefactoInteligente("Termostato Inteligente", 0.25, 0.85),
            new ArtefactoInteligente("Aspiradora Robot", 1.2, 0.9),
            new ArtefactoInteligente("Lavadora Inteligente", 4.5, 0.8),
            new ArtefactoInteligente("Iluminación Inteligente", 0.2, 0.75),
            new ArtefactoInteligente("Altavoz Inteligente", 0.05, 0.9)
        };
    }

    ~GestorArtefactos() {
        for (auto artefacto : disponibles) {
            delete artefacto;
        }
    }

    void mostrarMenu() const {
        cout << "\n--- Selección de Artefactos ---\n";
        for (size_t i = 0; i < disponibles.size(); ++i) {
            cout << i + 1 << ". " << setw(20) << left << disponibles[i]->getNombre()
                 << " (" << disponibles[i]->getConsumoPorMinuto() << " W/min)\n";
        }
    }

    ArtefactoBase* seleccionarArtefacto(int indice) const {
        return disponibles[indice];
    }

    size_t size() const {
        return disponibles.size();
    }
};

// Clase GestiónConsumo
class GestionConsumo {
private:
    vector<ArtefactoBase*> artefactos;
    vector<double> minutos_uso;
    vector<int> dias_uso;
    double tarifa_kwh;

public:
    GestionConsumo(double tarifa)
        : tarifa_kwh(tarifa) {}

    void agregarArtefacto(ArtefactoBase* artefacto, double minutos, int dias) {
        artefactos.push_back(artefacto);
        minutos_uso.push_back(minutos);
        dias_uso.push_back(dias);
    }

    void mostrarConsumos() const {
        if (artefactos.empty()) {
            cout << "\nNo hay artefactos registrados.\n";
            return;
        }

        ReporteConsumo::generarReporte(artefactos, minutos_uso, dias_uso, tarifa_kwh);
    }

    void guardarEnArchivo(const string& nombreArchivo) const {
        ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            cerr << "Error al abrir el archivo para escritura.\n";
            return;
        }

        archivo << "--- Reporte de Consumos ---\n";
        archivo << fixed << setprecision(2);

        double total_kw = 0, total_costo = 0;

        for (size_t i = 0; i < artefactos.size(); ++i) {
            archivo << "Artefacto: " << artefactos[i]->getNombre() << "\n";
            double consumo_kw = artefactos[i]->calcularConsumoMensualKW(minutos_uso[i], dias_uso[i]);
            double costo = artefactos[i]->calcularCosto(minutos_uso[i], dias_uso[i], tarifa_kwh);
            archivo << "Consumo mensual: " << consumo_kw << " kWh\n";
            archivo << "Costo mensual: " << costo << " $\n\n";

            total_kw += consumo_kw;
            total_costo += costo;
        }

        archivo << "Consumo total mensual: " << total_kw << " kWh\n";
        archivo << "Costo total mensual: " << total_costo << " $\n";
        archivo.close();
        cout << "Reporte guardado exitosamente en " << nombreArchivo << ".\n";
    }
};

// Función principal
int main() {
    cout << "=========================================\n";
    cout << "   SISTEMA DE GESTIÓN DE CONSUMO ENERGÉTICO\n";
    cout << "=========================================\n";

    string nombreUsuario, numeroUsuario;

    cout << "\nPor favor, regístrese.\n";
    cout << "Nombre: ";
    getline(cin, nombreUsuario);
    cout << "Número de contacto: ";
    getline(cin, numeroUsuario);

    cout << "\nBienvenido(a), " << nombreUsuario << " (" << numeroUsuario << ").\n";

    GestorArtefactos gestor;
    GestionConsumo gestion(0.15); // Tarifa de $0.15 por kWh

    int opcion;
    do {
        cout << "\n=====================\n";
        cout << "         MENÚ\n";
        cout << "=====================\n";
        cout << "1. Agregar artefactos\n";
        cout << "2. Mostrar consumo total\n";
        cout << "3. Guardar reporte en archivo\n";
        cout << "4. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcion) {
            case 1: {
                gestor.mostrarMenu();
                cout << "Seleccione los artefactos que desea agregar (0 para terminar): ";
                string linea;
                getline(cin, linea);
                istringstream iss(linea);
                int indice;

                while (iss >> indice && indice != 0) {
                    if (indice > 0 && indice <= (int)gestor.size()) {
                        ArtefactoBase* artefacto = gestor.seleccionarArtefacto(indice - 1);
                        cout << "Ingrese los minutos diarios de uso para " << artefacto->getNombre() << ": ";
                        double minutos;
                        cin >> minutos;
                        cout << "Ingrese los días de uso por mes para " << artefacto->getNombre() << ": ";
                        int dias;
                        cin >> dias;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        gestion.agregarArtefacto(artefacto, minutos, dias);
                    } else {
                        cout << "Opción inválida. Intente nuevamente.\n";
                    }
                }
                break;
            }
            case 2:
                gestion.mostrarConsumos();
                break;
            case 3:
                gestion.guardarEnArchivo("consumos.txt");
                break;
            case 4:
                cout << "Saliendo del programa... ¡Hasta pronto!\n";
                break;
            default:
                cout << "Opción inválida. Intente nuevamente.\n";
                break;
        }
    } while (opcion != 4);

return 0;
}