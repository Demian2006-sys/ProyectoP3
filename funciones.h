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

/**
 * Genera datos históricos simulados y los guarda en archivo.
 */
void generarDatosHistoricos(Zona* zonas, int cantidad);

/**
 * Carga datos desde archivo de texto normalizado con nombres entre comillas.
 */
void cargarDesdeArchivo(const char* nombreArchivo, Zona* zonas, int cantidad);

/**
 * Ingresa niveles actuales de contaminación para cada zona.
 */
void monitorearContaminacion(Zona* zonas, int cantidad, float* limites);

/**
 * Predice niveles futuros de contaminación usando ponderación lineal.
 */
void predecirContaminacion(Zona* zonas, int cantidad);

/**
 * Genera recomendaciones basadas en alertas.
 */
void generarRecomendaciones(Zona* zonas, int cantidad);

/**
 * Exporta informe detallado a archivo y también lo imprime en consola.
 */
void exportarInforme(const char* nombreArchivo, Zona* zonas, int cantidad);

/**
 * Guarda datos en archivo binario, con manejo de errores.
 */
void guardarDatos(const char* nombreArchivo, Zona* zonas, int cantidad);

/**
 * Carga datos desde archivo binario, retorna número de zonas cargadas.
 */
int cargarDatos(const char* nombreArchivo, Zona* zonas, int max_zonas);

/**
 * Calcula promedios históricos de contaminantes.
 */
void calcularPromediosHistoricos(Zona* zonas, int cantidad);

/**
 * Actualiza alertas según predicciones y límites.
 */
void emitirAlertas(Zona* zonas, int cantidad, float* limites);

/**
 * Guarda monitoreo actual en archivo de texto (formato tabla simple).
 */
void guardarMonitoreo(const char* nombreArchivo, Zona* zonas, int cantidad);

/**
 * Guarda datos históricos simulados en archivo de texto con nombres entre comillas.
 */
void guardarHistoricoSimulado(const char* archivo, Zona* zonas, int cantidad);

/**
 * Carga histórico desde archivo de texto con nombres entre comillas.
 */
void cargarHistoricoDesdeArchivo(const char* archivo, Zona* zonas, int cantidad);

/**
 * Imprime informe en archivo o consola.
 */
void imprimirInforme(FILE* f, Zona* zonas, int cantidad);

/**
 * Actualiza el histórico con los datos del monitoreo actual.
 */
void actualizarHistoricoConMonitoreo(Zona* zonas, int cantidad);
int datosDisponibles(int generados, int cargados);