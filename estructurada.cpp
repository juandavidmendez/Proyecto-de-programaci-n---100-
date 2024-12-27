#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>

using namespace std;

// Estructura para almacenar datos de un artefacto
struct Artefacto {
    string nombre;
    double consumo_diario_watts;
};

// Función para calcular el consumo semanal, quincenal y mensual en Watts
double calcularConsumoWatts(double consumo_diario, int dias) {
    return consumo_diario * dias;
}

// Función para convertir Watts a kilovatios-hora
double convertirAKilovatiosHora(double consumo_watts) {
    return consumo_watts / 1000;
}

// Función para mostrar el consumo de un artefacto
void mostrarConsumo(const Artefacto& artefacto) {
    cout << fixed << setprecision(2);
    cout << "Artefacto: " << artefacto.nombre << "\n";
    cout << "Consumo diario: " << artefacto.consumo_diario_watts << " Watts (" 
         << convertirAKilovatiosHora(artefacto.consumo_diario_watts) << " kWh)\n";
    cout << "Consumo semanal: " << calcularConsumoWatts(artefacto.consumo_diario_watts, 7) 
         << " Watts (" << convertirAKilovatiosHora(calcularConsumoWatts(artefacto.consumo_diario_watts, 7)) << " kWh)\n";
    cout << "Consumo quincenal: " << calcularConsumoWatts(artefacto.consumo_diario_watts, 14) 
         << " Watts (" << convertirAKilovatiosHora(calcularConsumoWatts(artefacto.consumo_diario_watts, 14)) << " kWh)\n";
    cout << "Consumo mensual: " << calcularConsumoWatts(artefacto.consumo_diario_watts, 30) 
         << " Watts (" << convertirAKilovatiosHora(calcularConsumoWatts(artefacto.consumo_diario_watts, 30)) << " kWh)\n\n";
}

// Función para calcular el consumo total mensual de todos los artefactos en kilovatios-hora
double calcularConsumoTotalMensual(const vector<Artefacto>& artefactos) {
    double total = 0;
    for (const auto& artefacto : artefactos) {
        total += convertirAKilovatiosHora(calcularConsumoWatts(artefacto.consumo_diario_watts, 30));
    }
    return total;
}

// Función para mostrar recomendaciones de ahorro energético
void mostrarRecomendaciones() {
    cout << "\nRecomendaciones para reducir el consumo de energía:\n";
    cout << "- Apague luces y dispositivos cuando no se usen.\n";
    cout << "- Desconecte artefactos de la corriente al no utilizarlos.\n";
    cout << "- Utilice dispositivos de bajo consumo energético.\n";
    cout << "- Ajuste el termostato de calefacción o aire acondicionado.\n";
    cout << "- Lave la ropa con agua fría y use la secadora solo cuando sea necesario.\n\n";
}

// Función para encontrar el artefacto con mayor consumo mensual
Artefacto encontrarArtefactoMayorConsumo(const vector<Artefacto>& artefactos) {
    Artefacto mayor_consumo = artefactos[0];
    for (const auto& artefacto : artefactos) {
        if (calcularConsumoWatts(artefacto.consumo_diario_watts, 30) > calcularConsumoWatts(mayor_consumo.consumo_diario_watts, 30)) {
            mayor_consumo = artefacto;
        }
    }
    return mayor_consumo;
}

// Función para encontrar el artefacto con menor consumo mensual
Artefacto encontrarArtefactoMenorConsumo(const vector<Artefacto>& artefactos) {
    Artefacto menor_consumo = artefactos[0];
    for (const auto& artefacto : artefactos) {
        if (calcularConsumoWatts(artefacto.consumo_diario_watts, 30) < calcularConsumoWatts(menor_consumo.consumo_diario_watts, 30)) {
            menor_consumo = artefacto;
        }
    }
    return menor_consumo;
}

// Función para mostrar el menú principal
void mostrarMenu() {
    cout << "\n======= Menú Principal =======\n";
    cout << "1. Agregar artefacto\n";
    cout << "2. Mostrar consumo de todos los artefactos\n";
    cout << "3. Mostrar recomendaciones de ahorro energético\n";
    cout << "4. Mostrar artefacto con mayor consumo\n";
    cout << "5. Mostrar artefacto con menor consumo\n";
    cout << "6. Salir\n";
    cout << "Seleccione una opción: ";
}

// Función para solicitar entrada numérica válida
double solicitarEntradaNumerica(const string& mensaje) {
    double valor;
    while (true) {
        cout << mensaje;
        cin >> valor;
        if (cin.fail() || valor <= 0) {
            cout << "Entrada inválida. Por favor, ingrese un número positivo.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            return valor;
        }
    }
}

// Función para agregar artefactos a la lista
void agregarArtefactos(vector<Artefacto>& artefactos) {
    int cantidad;
    cout << "\nIngrese la cantidad de artefactos que desea agregar: ";
    cin >> cantidad;

    for (int i = 0; i < cantidad; ++i) {
        Artefacto artefacto;
        cout << "\nIngrese el nombre del artefacto " << (i + 1) << ": ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Limpiar el buffer antes de getline
        getline(cin, artefacto.nombre);

        artefacto.consumo_diario_watts = solicitarEntradaNumerica("Consumo diario en Watts: ");
        artefactos.push_back(artefacto);
        cout << "Artefacto agregado correctamente.\n";
    }
}

int main() {
    vector<Artefacto> artefactos;
    double limite_consumo_mensual = solicitarEntradaNumerica("Ingrese el límite de consumo mensual en kWh: ");
    
    int opcion;
    do {
        mostrarMenu();
        cin >> opcion;

        switch (opcion) {
            case 1:
                agregarArtefactos(artefactos);
                break;
            case 2:
                if (artefactos.empty()) {
                    cout << "No hay artefactos registrados.\n";
                } else {
                    for (const auto& artefacto : artefactos) {
                        mostrarConsumo(artefacto);
                    }
                    double consumo_total = calcularConsumoTotalMensual(artefactos);
                    cout << "Consumo total mensual de todos los artefactos: " << consumo_total << " kWh\n";
                    if (consumo_total > limite_consumo_mensual) {
                        cout << "\n** Se ha excedido el límite de consumo mensual **\n";
                        mostrarRecomendaciones();
                    } else {
                        cout << "\nEl consumo mensual de energía es sostenible.\n";
                    }
                }
                break;
            case 3:
                mostrarRecomendaciones();
                break;
            case 4:
                if (!artefactos.empty()) {
                    Artefacto mayor_consumo = encontrarArtefactoMayorConsumo(artefactos);
                    cout << "\nArtefacto con mayor consumo mensual:\n";
                    mostrarConsumo(mayor_consumo);
                } else {
                    cout << "No hay artefactos registrados para evaluar.\n";
                }
                break;
            case 5:
                if (!artefactos.empty()) {
                    Artefacto menor_consumo = encontrarArtefactoMenorConsumo(artefactos);
                    cout << "\nArtefacto con menor consumo mensual:\n";
                    mostrarConsumo(menor_consumo);
                } else {
                    cout << "No hay artefactos registrados para evaluar.\n";
                }
                break;
            case 6:
                cout << "\nGracias por utilizar el programa. ¡Hasta pronto!\n";
                break;
            default:
                cout << "Opción inválida. Intente de nuevo.\n";
        }
    } while (opcion != 6);

return 0;
}