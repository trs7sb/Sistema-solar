#include <stdio.h>
#include <math.h>
#include <time.h> 
#include <stdbool.h>
#include <omp.h> // OpenMP para paralelización

// Constantes físicas
#define G 6.67430e-11 // Constante gravitacional (m^3 kg^-1 s^-2)
#define AU 1.496e11   // Unidad astronómica (m)
#define DAY 86400     // Un día en segundos
#define YEAR 365.25   // Un año en días
#define NUM_PLANETS 40 // Número de planetas actualizado (15 originales + 25 nuevos)
#define MASA_SOLAR 1.989e30 // Masa del Sol en kg
#define PI 3.14159265358979323846 // Definición de PI

// Datos de los planetas (masas en kg, distancias iniciales en m, velocidades iniciales en la dirección "y" en m/s)
// typedef permite crear objetos de tipo struct sin tener que escribir la palabra struct cada vez 
typedef struct {
    // Se define un tipo de dato llamado Planet que acceder a los atributos de cada planeta 
    char name[20]; // Nombre del planeta
    double mass;       // Masa del planeta
    double position[2]; // Posición (x, y) en m
    double velocity[2]; // Velocidad (vx, vy) en m/s

} Planet;       

// Inicializar datos reales de los planetas
void inicializarPlanetas(Planet planets[]) { 
    Planet temp[NUM_PLANETS] = {
        {"Sol", MASA_SOLAR, {0, 0}, {0, 0}}, 
        {"Mercurio", 3.3011e23, {0.39 * AU, 0}, {0, 47400}},
        {"Venus", 4.8675e24, {0.72 * AU, 0}, {0, 35020}},
        {"Tierra", 5.97237e24, {1.0 * AU, 0}, {0, 29780}},
        {"Luna", 7.34767309e22, 
            {1.0 * AU + 384400000}, 
            {0, 29780 + 1022}}, // Luna de la Tierra
        {"Marte", 6.4171e23, {1.52 * AU, 0}, {0, 24070}},
        {"Júpiter", 1.8982e27, {5.2 * AU, 0}, {0, 13070}},
        {"Ío", 8.9319e22, 
            {5.2 * AU + 421700000 * cos(0), 421700000 * sin(0)}, 
            { - sqrt(G * 1.8982e27 / 421700000) * sin(0), 
             sqrt(G * 1.8982e27 / 421700000) * cos(0) + 13070 }}, // Luna Ío
        {"Europa", 4.7998e22, 
            {5.2 * AU + 671100000 * cos(PI/ 2), 671100000 * sin(PI / 2)}, 
            { - sqrt(G * 1.8982e27 / 671100000) * sin(PI/ 2), 
             sqrt(G * 1.8982e27 / 671100000) * cos(PI / 2) + 13070}}, // Luna Europa
        {"Ganímedes", 1.4819e23, 
            {5.2 * AU + 1070400000 * cos(PI), 1070400000 * sin(PI)}, 
            {- sqrt(G * 1.8982e27 / 1070400000) * sin(PI), 
             sqrt(G * 1.8982e27 / 1070400000) * cos(PI)+ 13070}}, // Luna Ganímedes
        {"Calisto", 1.0759e23, 
            {5.2 * AU + 1882700000 * cos(3 * PI / 2), 1882700000 * sin(3 * PI / 2)}, 
            { - sqrt(G * 1.8982e27 / 1882700000) * sin(3 * PI/ 2), 
             sqrt(G * 1.8982e27 / 1882700000) * cos(3 * PI/ 2) + 13070}}, // Luna Calisto
        {"Saturno", 5.6834e26, {9.58 * AU, 0}, {0, 9680}},
        {"Urano", 8.6810e25, {19.22 * AU, 0}, {0, 6800}},
        {"Neptuno", 1.02413e26, {30.05 * AU, 0}, {0, 5430}},
        {"Plutón", 1.30900e22, {39.48 * AU, 0}, {0, 4748}},
        // Nuevos planetas ficticios
        {"Planeta1", 2.5e24, {45.0 * AU, 0}, {0, 4000}},
        {"Planeta2", 3.0e24, {50.0 * AU, 0}, {0, 3500}},
        {"Planeta3", 1.5e24, {55.0 * AU, 0}, {0, 3000}},
        {"Planeta4", 2.0e24, {60.0 * AU, 0}, {0, 2500}},
        {"Planeta5", 1.8e24, {65.0 * AU, 0}, {0, 2000}},
        {"Planeta6", 2.2e24, {70.0 * AU, 0}, {0, 1500}},
        {"Planeta7", 1.9e24, {75.0 * AU, 0}, {0, 1000}},
        {"Planeta8", 2.1e24, {80.0 * AU, 0}, {0, 900}},
        {"Planeta9", 2.3e24, {85.0 * AU, 0}, {0, 800}},
        {"Planeta10", 2.4e24, {90.0 * AU, 0}, {0, 700}},
        {"Planeta11", 2.6e24, {95.0 * AU, 0}, {0, 600}},
        {"Planeta12", 2.7e24, {100.0 * AU, 0}, {0, 500}},
        {"Planeta13", 2.8e24, {105.0 * AU, 0}, {0, 400}},
        {"Planeta14", 2.9e24, {110.0 * AU, 0}, {0, 300}},
        {"Planeta15", 3.1e24, {115.0 * AU, 0}, {0, 200}},
        {"Planeta16", 3.2e24, {120.0 * AU, 0}, {0, 100}},
        {"Planeta17", 3.3e24, {125.0 * AU, 0}, {0, 50}},
        {"Planeta18", 3.4e24, {130.0 * AU, 0}, {0, 25}},
        {"Planeta19", 3.5e24, {135.0 * AU, 0}, {0, 10}},
        {"Planeta20", 3.6e24, {140.0 * AU, 0}, {0, 5}},
        {"Planeta21", 3.7e24, {145.0 * AU, 0}, {0, 2}},
        {"Planeta22", 3.8e24, {150.0 * AU, 0}, {0, 1}},
        {"Planeta23", 3.9e24, {155.0 * AU, 0}, {0, 0.5}},
        {"Planeta24", 4.0e24, {160.0 * AU, 0}, {0, 0.2}},
        {"Planeta25", 4.1e24, {165.0 * AU, 0}, {0, 0.1}}
    };

    int i;
    for (i = 0; i < NUM_PLANETS; i++) {
        planets[i] = temp[i];
    }
}

// Función para dividir la masa de los planetas entre la masa solar
void normalizarMasa(Planet planets[]) {
    int i;
    for (i = 0; i < NUM_PLANETS; i++) {
        planets[i].mass /= MASA_SOLAR;
    }
}

// Función para convertir distancias y velocidades a unidades astronómicas (UA y UA/s)
void convertirUnidadesAU(Planet planets[]) {
    int i;
    for (i = 0; i < NUM_PLANETS; i++) {
        // Convertir posición de metros a UA
        planets[i].position[0] /= AU;
        planets[i].position[1] /= AU;

        // Convertir velocidad de m/s a UA/s
        planets[i].velocity[0] /= AU;
        planets[i].velocity[1] /= AU;
    }
}

// Función para convertir de unidades rescaladas a unidades originales
void convertirAUnidadesOriginales(Planet planets[]) {
    int i; 
    for (i = 0; i < NUM_PLANETS; i++) {
        // Convertir masa de masas solares a kilogramos
        planets[i].mass *= MASA_SOLAR;

        // Convertir posición de UA a metros
        planets[i].position[0] *= AU;
        planets[i].position[1] *= AU;

        // Convertir velocidad de UA/s a m/s
        planets[i].velocity[0] *= AU;
        planets[i].velocity[1] *= AU;
    }
}

// Función para reescalar las velocidades según el factor de tiempo
void reescalarVelocidades(Planet planets[], double factor_tiempo) {
    int i;
    for (i = 0; i < NUM_PLANETS; i++) {
        planets[i].velocity[0] /= factor_tiempo;
        planets[i].velocity[1] /= factor_tiempo;
    }
}

// Función para deshacer el reescalado de las velocidades
void deshacerReescaladoVelocidades(Planet planets[], double factor_tiempo) {
    int i; 
    for (i = 0; i < NUM_PLANETS; i++) {
        planets[i].velocity[0] *= factor_tiempo;
        planets[i].velocity[1] *= factor_tiempo;
    }
}

// Calcular la fuerza gravitacional entre dos planetas
void calcularFuerza(Planet *a, Planet *b, double *fx, double *fy) {
    double dx,dy,distancia,fuerza; 
     dx = b->position[0] - a->position[0];
     dy = b->position[1] - a->position[1];
    // dx y dy son las diferencias de posición entre los planetas a y b

    distancia = sqrt(dx * dx + dy * dy);
    fuerza = (1*a->mass * b->mass) / (distancia * distancia); //G es la unidad debido al rescalamiento
    *fx = fuerza * dx / distancia; //Componente x del vector fuerza 
    *fy = fuerza * dy / distancia; //Componente y del vector fuerza
}

//ANOTACIÓN SOBRE EL OPERADOR ->
/*
b es un puntero de tipo Planet, apunta a una dirección de memoria de una variable de tipo Planet.
-> es un operador de acceso a una propiedad del struct Planet equivalente a
 por ejemplo, (*b).position[0], que accede a la posición en x del planeta b. 
 a->mass accede a la masa del planeta a.
*/

void calcularAceleraciones(Planet planets[], double a[NUM_PLANETS][2]) {

    double fuerzas[NUM_PLANETS][2] = {0};
    int i, j, k;
    // Paralelizar el cálculo de las fuerzas gravitacionales

    //dynamic apropiado cuando las iteraciones tienen distintos costes computacionales
    //distribuye las iteraciones de manera dinámica entre los hilos sin orden considerando su tiempo de ejecución
    #pragma omp parallel for schedule(dynamic)

 
    for (i = 0; i < NUM_PLANETS; i++) {
        for (j = i + 1; j < NUM_PLANETS; j++) {
            double fx, fy;
            calcularFuerza(&planets[i], &planets[j], &fx, &fy);
            //omp atomic evita conflictos al utilizar una misma variable compartida
            //sincronizar entre hilos el acceso a una posición de memoria común, fx y fy
            //#pragma omp atomic
            fuerzas[i][0] += fx;
            //#pragma omp atomic
            fuerzas[i][1] += fy;
            //#pragma omp atomic
            fuerzas[j][0] -= fx;
            //#pragma omp atomic
            fuerzas[j][1] -= fy;
        }
    }

    // Calcular aceleraciones a partir de las fuerzas
    #pragma omp parallel for
    for (k = 0; k < NUM_PLANETS; k++) {
        a[k][0] = fuerzas[k][0] / planets[k].mass;
        a[k][1] = fuerzas[k][1] / planets[k].mass;
    }

    /* Creemos que calcular las aceleraciones a partir de la fuerzas disminuye el tiempo de ejecución porque 
    se calcula sólo la fuerza una vez por cada par de planetas,*/
}
 
//Calcula el módulo de la velocidad 
void calcularModulosVelocidad(Planet planets[], double modulosVelocidad[]) {
    int i; 
    for (i = 0; i < NUM_PLANETS; i++) {
        modulosVelocidad[i] = sqrt(planets[i].velocity[0] * planets[i].velocity[0] +
                                   planets[i].velocity[1] * planets[i].velocity[1]);
    }
}


// Calcular las energías del sistema (SIN RESCALAMIENTO)
void calcularEnergias(Planet planets[], double *energiaCinetica, double *energiaPotencial) {
    energiaCinetica[0] = 0;
    energiaPotencial[0] = 0;
    double modulosVelocidad[NUM_PLANETS] ={0};
    double energiaCineticaLocal = 0;
    double velocidad2;
    int i;

    calcularModulosVelocidad(planets, modulosVelocidad);

    // Energía cinética del sistema
   // #pragma omp parallel for //divide la iteraciones del for entre los hilos disponibles
   // reduction(+:energiaCinetica) cada hilo tiene su propia copia privada de energiaCinetica y al final se suman todas las copias
    #pragma omp parallel for reduction(+:energiaCineticaLocal) 

 
    for (i = 0; i < NUM_PLANETS; i++) {
        energiaCineticaLocal += 0.5 * planets[i].mass * modulosVelocidad[i]*modulosVelocidad[i]; // Energía cinética
    }

    /*Cada hilo calcula el valor de velocidad2 para las iteraciones del bucle que le han sido asignadas. 
    Cada hilo utiliza su copia privada de *energiaCinetica (creada automáticamente por la cláusula reduction) para acumular las contribuciones de las iteraciones que le corresponden.
    Una vez que todos los hilos han terminado de procesar sus iteraciones, OpenMP combina las copias privadas de *energiaCinetica en una única copia global. 
    En este caso, las copias privadas se suman para obtener el valor final de *energiaCinetica
    */

    //Usamos una variable de tipo double porque no se pueden utilizar punteros en reduction.

    *energiaCinetica = energiaCineticaLocal;
    
     // Energía potencial del sistema
     double energiaPotencialLocal = 0; // Variable local para la reducción
     int j;
     double dx, dy, distancia;
     #pragma omp parallel for reduction(+:energiaPotencialLocal)
     for (i = 0; i < NUM_PLANETS; i++) {
         for (j = i + 1; j < NUM_PLANETS; j++) {
             dx = planets[j].position[0] - planets[i].position[0];
             dy = planets[j].position[1] - planets[i].position[1];
             distancia = sqrt(dx * dx + dy * dy);
             energiaPotencialLocal -= (G * planets[i].mass * planets[j].mass) / distancia;
         }
     }
     *energiaPotencial = energiaPotencialLocal; 
}


// Actualizar posiciones y velocidades usando el método de Verlet
void actualizarPlanetas(Planet planets[], double dt) {
    double a[NUM_PLANETS][2] = {0};

    //Calcula las aceleraciones en el tiempo t a partir de las fuerzas con las posiciones en el tiempo t
    calcularAceleraciones(planets, a); 

    double w[NUM_PLANETS][2] = {0};

    //Almacena en un array w las velocidades y aceleraciones en el tiempo t 
    int i; 
    for (i = 1; i < NUM_PLANETS; i++) {
        w[i][0] = planets[i].velocity[0] + 0.5 * dt * a[i][0];
        w[i][1] = planets[i].velocity[1] + 0.5 * dt * a[i][1];
    }

    // Actualizar posiciones al tiempo t+dt
    int j;
    for (j= 0; j < NUM_PLANETS; j++) {
        planets[j].position[0] += planets[j].velocity[0] * dt + 0.5 * a[j][0] * dt * dt;
        planets[j].position[1] += planets[j].velocity[1] * dt + 0.5 * a[j][1] * dt * dt;
    }

    // Calcular la aceleración con las posiciones actualizadas
    calcularAceleraciones(planets, a);

    //Calcular las nuevas velocidades al tiempo t+dt a partir de las aceleraciones en el tiempo t+dt y el array w
    int k; 
    for ( k = 0; k < NUM_PLANETS; k++) {
        planets[k].velocity[0] = w[k][0] + 0.5 * a[k][0] * dt;
        planets[k].velocity[1] = w[k][1] + 0.5 * a[k][1] * dt;
    }
}

// Imprimir las posiciones de los planetas en un instante de tiempo
//void imprimirPosiciones(Planet planets[], double tiempo) {
   // printf("Tiempo: %.2f días\n", tiempo / DAY);
    //int i;
   // for ( i = 0; i < NUM_PLANETS; i++) {
   //     printf("%s: x = %.2e, y = %.2e\n", planets[i].name, planets[i].position[0], planets[i].position[1]);
   // }
    //printf("\n"); //salto de línea
//}

// Función para guardar las posiciones de los planetas en un archivo
void guardarPosiciones(Planet planets[], FILE *archivo_posiciones) {
    int i;
    for ( i = 0; i < NUM_PLANETS; i++) {
        fprintf(archivo_posiciones, "%.6e, %.6e\n", planets[i].position[0], planets[i].position[1]);
    }
    fprintf(archivo_posiciones, "\n"); // Línea en blanco para separar instantes de tiempo
}

// Función para calcular los períodos de los planetas usando la Tercera Ley de Kepler
void calcularPeriodos(Planet planets[], double periodos[], double t) {
    int i; 
    for (i = 0; i < NUM_PLANETS; i++) {
            if((planets[i].position[1] < 0) && (periodos[i]==0))
            {
                periodos[i] = 2*t;
            }
        }
    }

 
double calcularMomentoAngularTotal(Planet planets[]) {
    double momento_angular_total = 0.0;
    int i;
    double r, momento_angular; 
    double modulosVelocidad[NUM_PLANETS] = {0};

    for (i = 0; i < NUM_PLANETS; i++) {
        // Módulo de la posición
        double r = sqrt(planets[i].position[0] * planets[i].position[0] +
                        planets[i].position[1] * planets[i].position[1]);

        // Módulo de la velocidad
      calcularModulosVelocidad(planets, modulosVelocidad);

        // Momento angular del planeta
        double momento_angular = planets[i].mass * r * modulosVelocidad[i];

        // Sumar al momento angular total
        momento_angular_total += momento_angular;
    }

    return momento_angular_total;
}


int main() {

    omp_set_num_threads(2); 
    // Establecer el número de hilos 

    time_t inicio = time(NULL); // Guardar el tiempo de inicio de la simulación

    int vueltas[NUM_PLANETS];
    Planet planets[NUM_PLANETS];
    double periodos[NUM_PLANETS] = {0}; 


    inicializarPlanetas(planets);



    // Rescalar masas (a masa solar) y posiciones y velocidades a UA y UA/s
    normalizarMasa(planets);
    convertirUnidadesAU(planets);

    // Calcular el factor de reescalado del tiempo
    double factor_tiempo = sqrt(G * MASA_SOLAR / pow(AU, 3));

    // Reescalar las velocidades según el factor de tiempo
    reescalarVelocidades(planets, factor_tiempo);

    // Reescalar el tiempo
    double dt = 0.1*DAY * factor_tiempo; 
    double tiempo_total = 50*YEAR * DAY * factor_tiempo; 

    FILE *archivo = fopen("energias.txt", "w");
    if (!archivo) {
        perror("Error al abrir el archivo");
        return 1;
    }

    // Abrir archivo para guardar las posiciones
    FILE *archivo_posiciones = fopen("posiciones_planetas.txt", "w");
     if (!archivo_posiciones) {
        perror("Error al abrir el archivo de posiciones");
        return 1;
    }
    
    // Abrir archivo para guardar el momento angular total
    FILE *archivo_momento_total = fopen("momento_angular_total.txt", "w");
    if (!archivo_momento_total) {
    perror("Error al abrir el archivo de momento angular total");
    return 1;
    }

    //Inicializar el número de vueltas completas de cada planeta
    int i; 
    for (i = 0; i < NUM_PLANETS; i++) {
        vueltas[i] = 0;
    }

    bool cruzo_eje[NUM_PLANETS]; // Indica si el planeta ha cruzado el eje y
    for (i = 0; i < NUM_PLANETS; i++) {
        cruzo_eje[i] = false; // Inicialmente, ningún planeta ha cruzado el eje
    }


    //CON EL TIEMPO Y LAS CONDICIONES INICIALES RESCALADAS
    for (double t = 0; t < tiempo_total; t += dt) {

        //Calcular posiciones y velocidades en el tiempo t+dt
        actualizarPlanetas(planets, dt);
        // Guardar las posiciones de los planetas para cada tiempo.
        guardarPosiciones(planets, archivo_posiciones);
        
        // Convertir a unidades originales antes de calcular las energías
        convertirAUnidadesOriginales(planets);
        // Deshacer el reescalado de las velocidades por el factor tiempo 
        deshacerReescaladoVelocidades(planets, factor_tiempo);
        

        double energiaCinetica, energiaPotencial;
        //Devuelve la energía cinética y potencial del sistema en el tiempo t + dt  en (m, kg, s)
        calcularEnergias(planets, &energiaCinetica, &energiaPotencial);
        double energiaMecanica = energiaCinetica + energiaPotencial;

        //Guarda las energías en el archivo. El tiempo en días se tiene en cuenta en el código de python 
        fprintf(archivo, "%.6e %.6e %.6e\n", energiaCinetica, energiaPotencial, energiaMecanica);

        // Calcular el momento angular total y guardarlo en el archivo
        double momento_angular_total = calcularMomentoAngularTotal(planets);
         fprintf(archivo_momento_total, "%.6e\n", momento_angular_total);

        //if ((int)(t / dt) % 30 == 0) { // Imprimir cada 30 días
        //    imprimirPosiciones(planets, t / factor_tiempo); // Tiempo en unidades originales
        //}

        // Volver a normalizar las unidades para continuar la simulación
        normalizarMasa(planets);
        convertirUnidadesAU(planets);

        // Reescalar las velocidades nuevamente para continuar la simulación
        reescalarVelocidades(planets, factor_tiempo);

        
            calcularPeriodos(planets, periodos, t);
    }

    fclose(archivo);
    fclose(archivo_posiciones);
    fclose(archivo_momento_total);
    time_t fin = time(NULL); // Guardar el tiempo de finalización de la simulación

    // Imprimir los períodos de cada planeta
   // for (int i = 0; i < NUM_PLANETS; i++) {
     //   printf("%s: %.2f años\n", planets[i].name, periodos[i]/( factor_tiempo*DAY* YEAR)); 
     //}


    printf("Tiempo de inicio: %s", ctime(&inicio)); // Imprimir el tiempo de inicio
    printf("Tiempo de finalización: %s", ctime(&fin)); // Imprimir el tiempo de finalización
    double tiempo_total_simulacion = difftime(fin, inicio); // Calcular el tiempo total de la simulación
    printf("Tiempo total de simulación: %.8f segundos\n", tiempo_total_simulacion); // Imprimir el tiempo total de la simulación
    return 0;
}