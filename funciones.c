#include <stdio.h>
#include <string.h>
#include "funciones.h"

int datosDisponibles(int generados, int cargados) {
    if (!generados && !cargados) {
        printf("Debe generar o cargar datos antes de realizar esta acción.\n");
        return 0;
    }
    return 1;
}
void generarDatosHistoricos(Zona* zonas, int cantidad) {
    const char* nombresZonas[5] = {
        "Tumbaco", "La Carolina", "Calderón", "Valle de los chillos", "Centro historico"
    };

    for (int i = 0; i < cantidad; i++) {
        strncpy(zonas[i].nombre, nombresZonas[i], sizeof(zonas[i].nombre) - 1);
        zonas[i].nombre[sizeof(zonas[i].nombre) - 1] = '\0';
        zonas[i].temperatura = 25 + i;
        zonas[i].humedad = 60 + i;
        zonas[i].viento = 5 + i;
        for (int d = 0; d < 29; d++) {
            for (int j = 0; j < 4; j++) {
                zonas[i].contaminantes[d][j] = 80 + (i + j + d) % 20;
            }
        }
        for (int j = 0; j < 4; j++) {
            zonas[i].contaminantes[29][j] = 0;
            zonas[i].alerta[j] = 0;
            zonas[i].prediccion[j] = 0;
            zonas[i].promedio[j] = 0;
        }
        zonas[i].recomendacion[0] = '\0';
    }

    guardarHistoricoSimulado("historico.txt", zonas, cantidad);
    printf("\nDatos históricos generados y guardados.\n");
}

void cargarDesdeArchivo(const char* nombreArchivo, Zona* zonas, int cantidad) {
    FILE* f = fopen(nombreArchivo, "r");
    if (!f) {
        printf("No se pudo abrir %s.\n", nombreArchivo);
        return;
    }
    for (int i = 0; i < cantidad; i++) {
        if (!fgets(zonas[i].nombre, sizeof(zonas[i].nombre), f)) {
            printf("Error leyendo nombre de zona en línea %d.\n", i + 1);
            fclose(f);
            return;
        }
        zonas[i].nombre[strcspn(zonas[i].nombre, "\n")] = '\0';

        for (int d = 0; d < 30; d++) {
            for (int j = 0; j < 4; j++) {
                if (fscanf(f, "%f", &zonas[i].contaminantes[d][j]) != 1) {
                    printf("Error leyendo contaminantes para zona %s día %d.\n", zonas[i].nombre, d + 1);
                    fclose(f);
                    return;
                }
            }
        }
        if (fscanf(f, "%f %f %f", &zonas[i].temperatura, &zonas[i].humedad, &zonas[i].viento) != 3) {
            printf("Error leyendo clima para zona %s.\n", zonas[i].nombre);
            fclose(f);
            return;
        }
    }
    fclose(f);
    printf("Datos cargados desde archivo.\n");
}

void monitorearContaminacion(Zona* zonas, int cantidad, float* limites) {
    for (int i = 0; i < cantidad; i++) {
        printf("\nZona: %s\n", zonas[i].nombre);

        // Ingreso de contaminantes con alertas inmediatas
        for (int j = 0; j < 4; j++) {
            const char* nombreCont = (j == 0) ? "CO2" : (j == 1) ? "SO2" : (j == 2) ? "NO2" : "PM2.5";
            const char* unidad = (j == 0) ? "ppm" : "ug/m3";
            float valor;
            while (1) {
                printf("  Ingrese nivel actual de %s (%s): ", nombreCont, unidad);
                if (scanf("%f", &valor) != 1 || valor < 0 || valor > 9999) {
                    printf("  Valor inválido. Debe estar entre 0 y 9999. Intente de nuevo.\n");
                    while (getchar() != '\n');
                } else {
                    zonas[i].contaminantes[29][j] = valor;
                    zonas[i].alerta[j] = valor > limites[j];
                    if (zonas[i].alerta[j]) {
                        printf(" ALERTA: %s excede el límite (%.2f > %.2f)\n", nombreCont, valor, limites[j]);
                    } else {
                        printf(" %s dentro del rango permitido.\n", nombreCont);
                    }
                    break;
                }
            }
        }

        // Ingreso de temperatura
        while (1) {
            printf("  Ingrese temperatura actual (°C): ");
            if (scanf("%f", &zonas[i].temperatura) != 1 || zonas[i].temperatura < -50 || zonas[i].temperatura > 60) {
                printf("  Temperatura inválida. Rango permitido: -50 a 60 °C.\n");
                while (getchar() != '\n');
            } else {
                break;
            }
        }

        // Ingreso de humedad
        while (1) {
            printf("  Ingrese humedad actual (%%): ");
            if (scanf("%f", &zonas[i].humedad) != 1 || zonas[i].humedad < 0 || zonas[i].humedad > 100) {
                printf("Humedad inválida. Rango permitido: 0 a 100.\n");
                while (getchar() != '\n');
            } else {
                break;
            }
        }

        // Ingreso de viento
        while (1) {
            printf("  Ingrese velocidad del viento (km/h): ");
            if (scanf("%f", &zonas[i].viento) != 1 || zonas[i].viento < 0 || zonas[i].viento > 300) {
                printf("  Velocidad del viento inválida. Rango permitido: 0 a 300 km/h.\n");
                while (getchar() != '\n');
            } else {
                break;
            }
        }

        // Limpiar buffer
        while (getchar() != '\n');
    }
}


void predecirContaminacion(Zona* zonas, int cantidad) {
    printf("\nRESULTADOS DE LA PREDICCIÓN\n");
    for (int i = 0; i < cantidad; i++) {
        printf("\nZona: %s\n", zonas[i].nombre);
        for (int j = 0; j < 4; j++) {
            float suma = 0.0f;
            float peso = 0.0f;

            // Promedio ponderado del contaminante
            for (int d = 0; d < 30; d++) {
                float w = (float)(d + 1) / 30;
                suma += zonas[i].contaminantes[d][j] * w;
                peso += w;
            }

            float base = (peso != 0) ? suma / peso : 0;

            // Ajuste basado en clima actual
            float ajuste = 1.0f
                + (zonas[i].temperatura - 25.0f) * 0.01f     // más calor, más contaminación
                - (zonas[i].humedad - 50.0f) * 0.005f        // más humedad, menos contaminación
                - (zonas[i].viento * 0.002f);                // más viento, dispersa más

            if (ajuste < 0.8f) ajuste = 0.8f;
            if (ajuste > 1.2f) ajuste = 1.2f;

            zonas[i].prediccion[j] = base * ajuste;

            printf("  %s → %.2f\n",
                   (j == 0) ? "CO2" : (j == 1) ? "SO2" : (j == 2) ? "NO2" : "PM2.5",
                   zonas[i].prediccion[j]);
        }
    }
}

void generarRecomendaciones(Zona* zonas, int cantidad) {
    for (int i = 0; i < cantidad; i++) {
        zonas[i].recomendacion[0] = '\0';
        int alerta = 0;

        for (int j = 0; j < 4; j++) {
            if (zonas[i].alerta[j]) {
                alerta = 1;
                float exceso = zonas[i].contaminantes[29][j];  // monitoreo actual
                const char* cont = (j == 0) ? "CO2" : (j == 1) ? "SO2" : (j == 2) ? "NO2" : "PM2.5";

                // Recomendación según intensidad
                const char* medida;
                if (exceso < 1.2 * 25 && j == 0) medida = "Promover uso de bicicleta y restringir autos privados.";
                else if (exceso < 1.5 * 25) medida = "Reducir actividad industrial ligera.";
                else if (exceso < 2.0 * 25) medida = "Activar control de emisiones y alertas públicas.";
                else medida = "Evacuar zonas críticas y suspender actividades urbanas.";

                char buffer[150];
                snprintf(buffer, sizeof(buffer), "%s: %s ", cont, medida);

                strncat(zonas[i].recomendacion, buffer, sizeof(zonas[i].recomendacion) - strlen(zonas[i].recomendacion) - 1);
            }
        }

        if (!alerta) {
            strncpy(zonas[i].recomendacion, "Sin alerta. Mantener buenas prácticas ambientales.",
                    sizeof(zonas[i].recomendacion));
        }
    }

    printf("\nRECOMENDACIONES GENERADAS:\n");
    for (int i = 0; i < cantidad; i++) {
        printf("\nZona: %s\n%s\n", zonas[i].nombre, zonas[i].recomendacion);
    }
}

void exportarInforme(const char* nombreArchivo, Zona* zonas, int cantidad) {
    FILE* f = fopen(nombreArchivo, "a");
    if (!f) {
        printf("No se pudo crear el informe.\n");
        return;
    }

    fprintf(f, "\n\n==================== NUEVO INFORME ====================\n");
    printf("\n\n==================== NUEVO INFORME ====================\n");

    imprimirInforme(f, zonas, cantidad);
    imprimirInforme(stdout, zonas, cantidad);  // <- imprime en consola

    fclose(f);
    printf("Informe exportado en %s\n", nombreArchivo);
}


void guardarDatos(const char* nombreArchivo, Zona* zonas, int cantidad) {
    FILE* f = fopen(nombreArchivo, "wb");
    if (!f) {
        printf("Error al guardar archivo binario.\n");
        return;
    }
    size_t escritos = fwrite(zonas, sizeof(Zona), cantidad, f);
    if (escritos != (size_t)cantidad) {
        printf("Error al escribir todos los datos en archivo binario.\n");
    }
    fclose(f);
    printf("Datos guardados (binario).\n");
}

int cargarDatos(const char* nombreArchivo, Zona* zonas, int max_zonas) {
    FILE* f = fopen(nombreArchivo, "rb");
    if (!f) {
        printf("Archivo binario no encontrado.\n");
        return 0;
    }
    int n = fread(zonas, sizeof(Zona), max_zonas, f);
    if (n <= 0) {
        printf("Error leyendo datos del archivo binario.\n");
        fclose(f);
        return 0;
    }
    fclose(f);
    printf("Datos cargados (binario).\n");
    return n;
}

void calcularPromediosHistoricos(Zona* zonas, int cantidad) {
    for (int i = 0; i < cantidad; i++) {
        for (int j = 0; j < 4; j++) {
            float suma = 0.0f;
            int dias = 0;
            for (int d = 0; d < 30; d++) {
                if (zonas[i].contaminantes[d][j] >= 0) {
                    suma += zonas[i].contaminantes[d][j];
                    dias++;
                }
            }
            zonas[i].promedio[j] = (dias > 0) ? suma / dias : -1.0f;
        }
    }
}

void emitirAlertas(Zona* zonas, int cantidad, float* limites) {
    for (int i = 0; i < cantidad; i++) {
        for (int j = 0; j < 4; j++) {
            zonas[i].alerta[j] = zonas[i].prediccion[j] > limites[j];
        }
    }
}

void guardarMonitoreo(const char* nombreArchivo, Zona* zonas, int cantidad) {
    FILE* f = fopen(nombreArchivo, "a"); // Append mode
    if (!f) {
        printf("No se pudo abrir el archivo de monitoreo.\n");
        return;
    }

    fprintf(f, "+------------------------+------------+------------+------------+------------+--------+\n");
    fprintf(f, "| %-22s | %-10s | %-10s | %-10s | %-10s | %-6s |\n", 
            "Zona", "CO2", "SO2", "NO2", "PM2.5", "Alerta");
    fprintf(f, "+------------------------+------------+------------+------------+------------+--------+\n");

    for (int i = 0; i < cantidad; i++) {
        int alerta = 0;
        for (int j = 0; j < 4; j++) {
            if (zonas[i].alerta[j]) {
                alerta = 1;
                break;
            }
        }

        fprintf(f, "| %-22s | %-10.2f | %-10.2f | %-10.2f | %-10.2f | %-6s |\n",
                zonas[i].nombre,
                zonas[i].contaminantes[29][0],
                zonas[i].contaminantes[29][1],
                zonas[i].contaminantes[29][2],
                zonas[i].contaminantes[29][3],
                alerta ? "SI" : "NO");
    }

    fprintf(f, "+------------------------+------------+------------+------------+------------+--------+\n");
    fclose(f);
    printf("Monitoreo guardado en texto (%s)\n", nombreArchivo);
}

void guardarHistoricoSimulado(const char* archivo, Zona* zonas, int cantidad) {
    FILE* f = fopen(archivo, "w");
    if (!f) {
        printf("No se pudo guardar el archivo histórico.\n");
        return;
    }

    for (int i = 0; i < cantidad; i++) {
        for (int d = 0; d < 29; d++) {
            // Guardar nombre con comillas para nombres con espacios
            fprintf(f, "\"%s\" %d %.2f %.2f %.2f %.2f\n",
                zonas[i].nombre, d + 1,
                zonas[i].contaminantes[d][0],
                zonas[i].contaminantes[d][1],
                zonas[i].contaminantes[d][2],
                zonas[i].contaminantes[d][3]);
        }
    }

    fclose(f);
    printf("Datos históricos simulados guardados en %s\n", archivo);
}

void cargarHistoricoDesdeArchivo(const char* archivo, Zona* zonas, int cantidad) {
    FILE* f = fopen(archivo, "r");
    if (!f) {
        printf("No se pudo cargar el histórico desde %s\n", archivo);
        return;
    }

    char nombreZona[30];
    int dia;
    float co2, so2, no2, pm25;

    while (fscanf(f, " \"%29[^\"]\" %d %f %f %f %f", nombreZona, &dia, &co2, &so2, &no2, &pm25) == 6) {
        int encontrado = 0;
        for (int i = 0; i < cantidad; i++) {
            if (strcmp(nombreZona, zonas[i].nombre) == 0) {
                if (dia >= 1 && dia <= 30) {
                    zonas[i].contaminantes[dia - 1][0] = co2;
                    zonas[i].contaminantes[dia - 1][1] = so2;
                    zonas[i].contaminantes[dia - 1][2] = no2;
                    zonas[i].contaminantes[dia - 1][3] = pm25;
                    encontrado = 1;
                }
                break;
            }
        }
        if (!encontrado) {
            printf("Advertencia: Zona '%s' no encontrada en datos.\n", nombreZona);
        }
    }

    fclose(f);
    printf("Histórico cargado desde %s\n", archivo);
}

void imprimirInforme(FILE* f, Zona* zonas, int cantidad) {
    fprintf(f, "+------------------------+------------+------------+------------+------------+--------+-----------------------------------------------+\n");
    fprintf(f, "| %-22s | %-10s | %-10s | %-10s | %-10s | %-6s | %-45s |\n",
            "Zona", "CO2 (ppm)", "SO2 (ug/m3)", "NO2 (ug/m3)", "PM2.5 (ug/m3)", "Alerta", "Recomendación");
    fprintf(f, "+------------------------+------------+------------+------------+------------+--------+-----------------------------------------------+\n");

    for (int i = 0; i < cantidad; i++) {
        int alerta = 0;
        for (int j = 0; j < 4; j++) {
            if (zonas[i].alerta[j]) {
                alerta = 1;
                break;
            }
        }

        fprintf(f, "| %-22s | %-10.2f | %-10.2f | %-10.2f | %-10.2f | %-6s | %-45s |\n",
                zonas[i].nombre,
                zonas[i].prediccion[0],
                zonas[i].prediccion[1],
                zonas[i].prediccion[2],
                zonas[i].prediccion[3],
                alerta ? "SI" : "NO",
                zonas[i].recomendacion);
    }

    fprintf(f, "+------------------------+------------+------------+------------+------------+--------+-----------------------------------------------+\n");
}

void actualizarHistoricoConMonitoreo(Zona* zonas, int cantidad) {
    // 1. Cargar histórico existente
    cargarHistoricoDesdeArchivo("historico.txt", zonas, cantidad);

    for (int i = 0; i < cantidad; i++) {
        // 2. Desplazar días: del día 1 al día 29
        for (int d = 0; d < 29; d++) {
            for (int j = 0; j < 4; j++) {
                zonas[i].contaminantes[d][j] = zonas[i].contaminantes[d + 1][j];
            }
        }

        // 3. Copiar los valores monitoreados (día actual) al último día del histórico
        for (int j = 0; j < 4; j++) {
            zonas[i].contaminantes[29][j] = zonas[i].contaminantes[29][j]; // Este valor ya está ahí tras el monitoreo
            // No hace falta reasignar, pero puedes dejarlo si quieres que sea explícito
        }
    }

    // 4. Guardar todo el arreglo actualizado en el archivo
    FILE* f = fopen("historico.txt", "w");
    if (!f) {
        printf("Error al actualizar el archivo histórico.\n");
        return;
    }

    for (int i = 0; i < cantidad; i++) {
        for (int d = 0; d < 30; d++) {
            fprintf(f, "\"%s\" %d %.2f %.2f %.2f %.2f\n",
                    zonas[i].nombre, d + 1,
                    zonas[i].contaminantes[d][0],
                    zonas[i].contaminantes[d][1],
                    zonas[i].contaminantes[d][2],
                    zonas[i].contaminantes[d][3]);
        }
    }

    fclose(f);
    printf("Histórico actualizado con monitoreo actual.\n");
}
