#include <stdio.h>
#include <string.h>
#include "funciones.h"

#define NUM_ZONAS 5

int main() {
    Zona zonas[NUM_ZONAS];
    float limites[4] = {100, 40, 25, 15}; // CO2, SO2, NO2, PM2.5

    int opcion;
    int datosGenerados = 0;
    int datosCargados = 0;

    do {
        printf("\n======== MENÚ DE OPCIONES ========\n");
        printf("1. Generar datos históricos simulados\n");
        printf("2. Monitorear niveles actuales\n");
        printf("3. Predecir contaminación\n");
        printf("4. Generar recomendaciones\n");
        printf("5. Exportar informe\n");
        printf("6. Guardar datos en archivo binario\n");
        printf("7. Cargar datos desde archivo binario\n");
        printf("8. Guardar monitoreo actual en archivo de texto\n");
        printf("0. Salir\n");
        printf("Seleccione una opción: ");
        if (scanf("%d", &opcion) != 1) {
            printf("Entrada inválida. Intente de nuevo.\n");
            while(getchar() != '\n');
            continue;
        }

        switch (opcion) {
            case 1:
                generarDatosHistoricos(zonas, NUM_ZONAS);
                datosGenerados = 1;
                datosCargados = 0;
                break;
            case 2:
                if (!datosDisponibles(datosGenerados, datosCargados)) break;
                monitorearContaminacion(zonas, NUM_ZONAS, limites);
                actualizarHistoricoConMonitoreo(zonas, NUM_ZONAS);
                break;
            case 3:
                if (!datosDisponibles(datosGenerados, datosCargados)) break;
                predecirContaminacion(zonas, NUM_ZONAS);
                emitirAlertas(zonas, NUM_ZONAS, limites);
                break;
            case 4:
                if (!datosDisponibles(datosGenerados, datosCargados)) break;
                generarRecomendaciones(zonas, NUM_ZONAS);
                printf("Recomendaciones generadas.\n");
                break;
            case 5:
                if (!datosDisponibles(datosGenerados, datosCargados)) break;
                exportarInforme("informe.txt", zonas, NUM_ZONAS);
                break;
            case 6:
                if (!datosDisponibles(datosGenerados, datosCargados)) break;
                guardarDatos("datos.bin", zonas, NUM_ZONAS);
                break;
            case 7: {
                int cargados = cargarDatos("datos.bin", zonas, NUM_ZONAS);
                if (cargados > 0) {
                    datosCargados = 1;
                    datosGenerados = 0;
                }
                break;
            }
            case 8:
                if (!datosDisponibles(datosGenerados, datosCargados)) break;
                guardarMonitoreo("monitoreo.txt", zonas, NUM_ZONAS);
                break;
            case 0:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opción inválida. Intente de nuevo.\n");
                break;
        }
    } while (opcion != 0);

    return 0;
}
