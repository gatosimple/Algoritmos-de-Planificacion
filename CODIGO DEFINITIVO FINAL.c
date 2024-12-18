#include <stdio.h>
#include <stdlib.h>

struct Procesos {
    int TiempoEspera;
    int TiempoLlegada;
    int TiempoCPU;
    int TiempoRetorno;
    int Prioridad;
};

void ImprimirCelda(FILE *file, char c, int esVerde) {
    if (esVerde) {
    printf("\033[30;42m"); // Código de color de fondo verde
    } else {
    printf("\033[30;43m");  // Código de color de fondo amarillo
    }
    fprintf(file,"%2c ", c);

    printf("%2c \033[0m", c); // Restablecer el color a su valor predeterminado después de imprimir la celda
}

void ImprimirInfo(FILE *file, char letras [], struct Procesos Proceso[], int n){
    char b[] = "Proceso";
    char c[] = "Te";
    char d[] = "Tr";
    printf("\n\n%9s%8s%8s\n",b,c,d);
    printf("+-----------+-------+-------+\n");
    fprintf(file,"\n\n%9s%8s%8s\n",b,c,d);
    fprintf(file,"+-----------+-------+-------+\n");

    for (int i=0 ; i < n; i++)
    {
        printf("|\033[104;37m Proceso %c \033[0m|\033[30;48;5;230m%4d   \033[0m|\033[30;48;5;230m%4d   \033[0m|\n",
               letras[i] , Proceso[i].TiempoEspera, Proceso[i].TiempoRetorno);
        printf("+-----------+-------+-------+\n");
        fprintf(file,"| Proceso %c |%4d   |%4d   |\n", letras[i] , Proceso[i].TiempoEspera, Proceso[i].TiempoRetorno);
        fprintf(file,"+-----------+-------+-------+\n");
    }
}

float CalculoPromedios(FILE *file, float totalTiempoEspera, float totalTiempoRetorno, int n){
    float PromedioTiempoEspera = totalTiempoEspera / n;
    float PromedioTiempoRetorno = totalTiempoRetorno / n;
    float PromedioTiempoServicio = PromedioTiempoRetorno - PromedioTiempoEspera;
    float PromedioTiempoRetornoNormalizado = PromedioTiempoRetorno / PromedioTiempoServicio;

    printf("\nTiempo promedio de espera: %.2f\n", PromedioTiempoEspera);
    printf("Tiempo promedio de retorno: %.2f\n", PromedioTiempoRetorno);
    printf("Tiempo promedio de servicio: %.2f\n", PromedioTiempoServicio);
    printf("Tiempo promedio de retorno normalizado: %.2f\n", PromedioTiempoRetornoNormalizado);

    fprintf(file,"\nTiempo promedio de espera: %.2f\n", PromedioTiempoEspera);
    fprintf(file,"Tiempo promedio de retorno: %.2f\n", PromedioTiempoRetorno);
    fprintf(file,"Tiempo promedio de servicio: %.2f\n", PromedioTiempoServicio);
    fprintf(file,"Tiempo promedio de retorno normalizado: %.2f\n", PromedioTiempoRetornoNormalizado);

    return PromedioTiempoRetornoNormalizado;
}

float Prioridad(FILE *file, struct Procesos Proceso[], int n)
{
    int ciclo = 0;
    int contador = 0;
    int procesoElegido;
    int TiempoCOPIACPU[5];
    int i;
    int columnas=0;
    float totalTiempoEspera = 0, totalTiempoRetorno = 0;
    for (i = 0; i < n; i++)
    {
        TiempoCOPIACPU[i] = Proceso[i].TiempoCPU;
    }

    int ultimoTiempoLlegada = 0;
    for (i = 0; i < n; i++) {
      if (Proceso[i].TiempoLlegada > ultimoTiempoLlegada) {
        ultimoTiempoLlegada = Proceso[i].TiempoLlegada;
      }
    }

    for (i = 0; i < n; i++)
    {
        columnas = columnas + Proceso[i].TiempoCPU;
    }
    columnas= columnas + ultimoTiempoLlegada;

    char arreglo[i][columnas];

 while (contador != n) {
    procesoElegido = -1;

   for (i = 0; i < n; i++)
    {
        arreglo[i][ciclo]= ' ';
        if (Proceso[i].TiempoLlegada <= ciclo && Proceso[i].TiempoCPU > 0)
        {
            arreglo[i][ciclo]= 'W';
            if (procesoElegido == -1){
                procesoElegido = i;
            }
            else if (Proceso[i].Prioridad < Proceso[procesoElegido].Prioridad){
                procesoElegido = i;
            }
            else if(Proceso[i].Prioridad == Proceso[procesoElegido].Prioridad && Proceso[i].TiempoLlegada < Proceso[procesoElegido].TiempoLlegada){
                procesoElegido = i;
            }
        }
    }

    arreglo[procesoElegido][ciclo] = 'X';

    if (procesoElegido != -1) {
        Proceso[procesoElegido].TiempoCPU--;

        if (Proceso[procesoElegido].TiempoCPU == 0) {
            contador++;

            Proceso[procesoElegido].TiempoRetorno = ciclo + 1 - Proceso[procesoElegido].TiempoLlegada;
            Proceso[procesoElegido].TiempoEspera = Proceso[procesoElegido].TiempoRetorno - TiempoCOPIACPU[procesoElegido];

            totalTiempoEspera += Proceso[procesoElegido].TiempoEspera;
            totalTiempoRetorno += Proceso[procesoElegido].TiempoRetorno;
        }
    }
    ciclo++;
}

    char letras[] = {'A','B','C','D','E'};
    char a[] = "============================ALGORITMO PRIORIDAD============================";
    printf("\n%22s\n\n",a);
    fprintf(file,"\n%22s\n\n",a);

    printf("Tabla\n");
    fprintf(file,"Tabla\n");

    char tiempo = 'T';
    printf("%5c",tiempo);
    printf("%4d",0);
    fprintf(file,"%5c",tiempo);
    fprintf(file,"%4d",0);
    for(i = 1; i <= ciclo ; i++){
        printf("%4d",i);
        fprintf(file,"%4d",i);
    }
    printf("\n");
    fprintf(file,"\n");
    printf("+-------");
    fprintf(file,"+-------");
    for (int j = 0; j < ciclo ; j++) {
        printf("+---");
       fprintf(file,"+---");
    }
    printf("+");
    printf("\n");
    fprintf(file,"+");
    fprintf(file,"\n");

    for (int i = 0; i < 5; i++)
    {
        printf("|%4c   ", letras[i]);
        fprintf(file,"|%4c   ", letras[i]);
        for (int j = 0; j < ciclo; j++) {
          if (arreglo[i][j] == 'X') {
            printf("|");
            fprintf(file,"|");
            ImprimirCelda(file,'X', 1);
          } else if (arreglo[i][j] == 'W') {
            printf("|");
            fprintf(file,"|");
            ImprimirCelda(file,'W', 0);
          } else {
            printf("|%2c ", arreglo[i][j]);
            fprintf(file,"|%2c ", arreglo[i][j]);
          }
        }

        printf("|");
        printf("\n");

        fprintf(file,"|");
        fprintf(file,"\n");
        printf("+-------");
        fprintf(file,"+-------");
        for (int j = 0; j < ciclo ; j++) {
        printf("+---");
        fprintf(file,"+---");
        }
        printf("+");
        printf("\n");
        fprintf(file,"+");
        fprintf(file,"\n");
    }

    // Imprimir tiempo de espera y retorno de cada proceso en una tabla
    ImprimirInfo(file,letras, Proceso, n);

    // Calculamos los promedios de tiempo de espera, retorno, servicio, retorno normalizado
    float PromedioTiempoRetornoNormalizado = CalculoPromedios(file,totalTiempoEspera, totalTiempoRetorno, n);
}

float FCFS(FILE *file, struct Procesos Proceso[], int n){
    int ciclo = 0;
    int contador = 0;
    int procesoElegido;
    int TiempoCOPIACPU[5];
    int i;
    int columnas = 0;

    float totalTiempoEspera = 0, totalTiempoRetorno = 0;

    for (i = 0; i < n; i++)
    {
        TiempoCOPIACPU[i] = Proceso[i].TiempoCPU;
    }
    int ultimoTiempoLlegada = 0;
    for (i = 0; i < n; i++) {
      if (Proceso[i].TiempoLlegada > ultimoTiempoLlegada) {
        ultimoTiempoLlegada = Proceso[i].TiempoLlegada;
      }
    }

    for (i = 0; i < n; i++)
    {
        columnas = columnas + Proceso[i].TiempoCPU;
    }
    columnas= columnas + ultimoTiempoLlegada;

    char arreglo[i][columnas];

    while (contador != n) {
        procesoElegido = -1;

    for (i = 0; i < n; i++)
    {
        arreglo[i][ciclo]= ' ';
        if (Proceso[i].TiempoLlegada <= ciclo && Proceso[i].TiempoCPU > 0)
        {
            arreglo[i][ciclo]= 'W';
            if (procesoElegido == -1){
                procesoElegido = i;
            }
            else if (Proceso[i].TiempoLlegada < Proceso[procesoElegido].TiempoLlegada){
                procesoElegido = i;
            }
            else if(Proceso[i].TiempoLlegada == Proceso[procesoElegido].TiempoLlegada && Proceso[i].Prioridad < Proceso[procesoElegido].Prioridad){
                procesoElegido = i;
            }
        }
    }
        arreglo[procesoElegido][ciclo]= 'X';
        if (procesoElegido != -1) {

           Proceso[procesoElegido].TiempoCPU--;

            if (Proceso[procesoElegido].TiempoCPU == 0) {

                contador++;

                Proceso[procesoElegido].TiempoRetorno = ciclo + 1 - Proceso[procesoElegido].TiempoLlegada;
                Proceso[procesoElegido].TiempoEspera = Proceso[procesoElegido].TiempoRetorno - TiempoCOPIACPU[procesoElegido];

                totalTiempoEspera += Proceso[procesoElegido].TiempoEspera;
                totalTiempoRetorno += Proceso[procesoElegido].TiempoRetorno;
            }
        }
        ciclo++;
        columnas++;
    }
    char letras[] = {'A','B','C','D','E'};
    char a[] = "============================ALGORITMO FCFS============================";
    printf("\n%22s\n\n",a);
    fprintf(file,"\n%22s\n\n",a);

    printf("Tabla\n");
    fprintf(file,"Tabla\n");

    char tiempo = 'T';
    printf("%5c",tiempo);
    printf("%4d",0);
    fprintf(file,"%5c",tiempo);
    fprintf(file,"%4d",0);
    for(i = 1; i <= ciclo ; i++){
        printf("%4d",i);
        fprintf(file,"%4d",i);
    }
    printf("\n");
    fprintf(file,"\n");
    printf("+-------");
    fprintf(file,"+-------");
    for (int j = 0; j < ciclo ; j++) {
        printf("+---");
       fprintf(file,"+---");
    }
    printf("+");
    printf("\n");
    fprintf(file,"+");
    fprintf(file,"\n");

    for (int i = 0; i < 5; i++)
    {
        printf("|%4c   ", letras[i]);
        fprintf(file,"|%4c   ", letras[i]);
        for (int j = 0; j < ciclo; j++) {
          if (arreglo[i][j] == 'X') {
            printf("|");
            fprintf(file,"|");
            ImprimirCelda(file,'X', 1);
          } else if (arreglo[i][j] == 'W') {
            printf("|");
            fprintf(file,"|");
            ImprimirCelda(file,'W', 0);
          } else {
            printf("|%2c ", arreglo[i][j]);
            fprintf(file,"|%2c ", arreglo[i][j]);
          }
        }

        printf("|");
        printf("\n");

        fprintf(file,"|");
        fprintf(file,"\n");
        printf("+-------");
        fprintf(file,"+-------");
        for (int j = 0; j < ciclo ; j++) {
        printf("+---");
        fprintf(file,"+---");
        }
        printf("+");
        printf("\n");
        fprintf(file,"+");
        fprintf(file,"\n");
    }

    // Imprimir tiempo de espera y retorno de cada proceso en una tabla
    ImprimirInfo(file,letras, Proceso, n);

    // Calculamos los promedios de tiempo de espera, retorno, servicio, retorno normalizado
    float PromedioTiempoRetornoNormalizado = CalculoPromedios(file,totalTiempoEspera, totalTiempoRetorno, n);
}

int main()
{
    struct Procesos Proceso[5];
    int n = 5;
    int i;

    float PromedioTiempoRetornoNormalizadoPrioridad;
    float PromedioTiempoRetornoNormalizadoFCFS;
    struct Procesos ProcesoCopia[5];
    FILE *file = fopen("output.txt", "w");

    char letras[] = {'A', 'B', 'C', 'D', 'E'};

    // Pedir datos de entrada
    printf("Ingrese el tiempo de llegada, tiempo de CPU y prioridad de cada proceso:\n");
    for (i = 0; i < n; i++)
    {
        printf("\nProceso %c:\n", letras[i]);
        printf("Tiempo de llegada: ");
        scanf("%d", &Proceso[i].TiempoLlegada);
        printf("Tiempo de CPU: ");
        scanf("%d", &Proceso[i].TiempoCPU);
        printf("Prioridad: ");
        scanf("%d", &Proceso[i].Prioridad);

        ProcesoCopia[i]= Proceso[i];
    }

    system("cls"); // o clear

    // imprimir una tabla con los datos de entrada
    char m[] = "Proceso";
    char x[] = "TLlegada";
    char y[] = "TCPU";
    char z[] = "Prioridad";
    printf("\n============================DATOS DE ENTRADA============================");
    fprintf(file,"\n============================DATOS DE ENTRADA============================");
    printf("\n\n%9s%11s%6s%11s\n",m,x,y,z);
    printf("+-----------+-------+-------+-------+\n");
    fprintf(file,"\n\n%9s%11s%6s%11s\n",m,x,y,z);
    fprintf(file,"+-----------+-------+-------+-------+\n");

    for (int i=0 ; i < n; i++)
    {
        printf("|\033[41;37m Proceso %c \033[0m|\033[30;48;5;230m%4d   \033[0m|\033[30;48;5;230m%4d   \033[0m|\033[30;48;5;230m%4d   \033[0m|\n",
            letras[i] , Proceso[i].TiempoLlegada, Proceso[i].TiempoCPU, Proceso[i].Prioridad);
        printf("+-----------+-------+-------+-------+\n");
        fprintf(file,"| Proceso %c |%4d   |%4d   |%4d   |\n", letras[i] , Proceso[i].TiempoLlegada, Proceso[i].TiempoCPU, Proceso[i].Prioridad);
        fprintf(file,"+-----------+-------+-------+-------+\n");
    }

    printf("\n\n");
    fprintf(file,"\n\n");

    // Uso de Algoritmos
    PromedioTiempoRetornoNormalizadoPrioridad = Prioridad(file,Proceso, n);
    for ( int i =0 ; i < n ; i++)
    {
        Proceso[i]= ProcesoCopia[i];
    }

    printf("\n\n");
    fprintf(file,"\n\n");

    PromedioTiempoRetornoNormalizadoFCFS = FCFS(file,Proceso, n);

    printf("\n\n=======================COMPARACION ENTRE ALGORITMOS=======================\n");
    fprintf(file,"\n\n=======================COMPARACION ENTRE ALGORITMOS=======================\n");

    if (PromedioTiempoRetornoNormalizadoPrioridad < PromedioTiempoRetornoNormalizadoFCFS){
        printf("\nEl Algoritmo de Prioridad es mejor\n");
        fprintf(file,"\nEl Algoritmo de Prioridad es mejor\n");
    }else if (PromedioTiempoRetornoNormalizadoPrioridad > PromedioTiempoRetornoNormalizadoFCFS){
        printf("\nEl Algoritmo de FCFS es mejor\n");
        fprintf(file,"\nEl Algoritmo de FCFS es mejor\n");
    }else{
        printf("\nTanto el Algoritmo de FCFS como el Algoritmo de Prioridad son buenos\n");
        fprintf(file,"\nTanto el Algoritmo de FCFS como el Algoritmo de Prioridad son buenos\n");
    }
    return 0;
}
