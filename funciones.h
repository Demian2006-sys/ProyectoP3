#include <stdio.h>
typedef struct {
    char nombre[30];
    float contaminantes[30][4];
    float prediccion[4];
    float promedio[4];
    int alerta[4];
    char recomendacion[100];
    float temperatura;
    float humedad;
    float viento;
} Zona;

void generarDatosHistoricos(Zona* zonas, int cantidad);
void cargarDesdeArchivo(const char* nombreArchivo, Zona* zonas, int cantidad);
void monitorearContaminacion(Zona* zonas, int cantidad, float* limites);
void predecirContaminacion(Zona* zonas, int cantidad);
void generarRecomendaciones(Zona* zonas, int cantidad);
void exportarInforme(const char* nombreArchivo, Zona* zonas, int cantidad);
void guardarDatos(const char* nombreArchivo, Zona* zonas, int cantidad);
int cargarDatos(const char* nombreArchivo, Zona* zonas, int max_zonas);
void calcularPromediosHistoricos(Zona* zonas, int cantidad);
void emitirAlertas(Zona* zonas, int cantidad, float* limites);
void guardarMonitoreo(const char* nombreArchivo, Zona* zonas, int cantidad);
void guardarHistoricoSimulado(const char* archivo, Zona* zonas, int cantidad);
void cargarHistoricoDesdeArchivo(const char* archivo, Zona* zonas, int cantidad);
void imprimirInforme(FILE* f, Zona* zonas, int cantidad);
void actualizarHistoricoConMonitoreo(Zona* zonas, int cantidad);
int datosDisponibles(int generados, int cargados);
