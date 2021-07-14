//#include "SerialClass.h" //ESTO SERIA PARA CONECTRALO CON ARDUINO
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>


#define SI 1
#define NO 0
#define N 10
#define LONGCAD 50

typedef struct
{
	char Ingrediente1[LONGCAD];
	char Ingrediente2[LONGCAD];
	char codigo[LONGCAD];
	char Nombre[LONGCAD];
	int kilocalorias;
}Datos;


void configura(void);
int menu_principal(void);


int alta_bebidas(Datos[], int*, int);
void listado_bebidas(Datos*, int);
int baja_bebidas(Datos*, int*);
int consulta_bebidas(Datos*, int);
int posicion_bebidas(Datos*, int, char*);
int modificacion_bebidas(Datos*, int);
void leer_fichero_bebidas(Datos*, int*, int);
int escribir_fichero_bebidas(Datos*, int);




Datos leer_datos_bebida(char);
void mostrar_titulos_bebidas(void);
void mostrar_datos_bebidas(Datos);


int main(void)
{
	Datos bebida[N];
	int n = 0;
	int longitud = N;

	// Variables comunes
	int resultado;
	int opc, opc1, res;  // Opci�n del men� principal seleccionada
	configura(); // Tareas de configuraci�n

	leer_fichero_bebidas(bebida, &n, longitud);

	do // Bucle principal de la aplicaci�n
	{
		opc = menu_principal();
		resultado = -1;

		switch (opc)
		{
		case 1:	resultado = alta_bebidas(bebida, &n, N);	break;
		case 2: resultado = baja_bebidas(bebida, &n);break;
		case 3: resultado = modificacion_bebidas(bebida, n); break;
		case 4: consulta_bebidas(bebida, n);break;
		case 5: listado_bebidas(bebida, n);break;
		case 6:	break;
		case 7: 
			while (1)
			{
				printf("Introduce una bebida:\n 1-Bebida1.\n 2-Bebida2.\n 3-Bebida3");
				scanf_s("%d", &opc1);
				switch (opc1)
				{
				case 1: printf("Has seleccionado la bebida1\n Espere un momento...\n");
					//funcion llamar arduino
					//llamararduino1();
					system("Pause");
					break;
				case 2: printf("Has seleccionado la bebida2\n Espere un momento...\n");
					//llamararduino2();
					system("pause");
					break;
				case 3: printf("Has seleccionado la bebida3\n Espere un momento...\n");
					//llamararduino3();
					system("Pause");
					break;
				}
			}
		}
		if (resultado == 0)
			escribir_fichero_bebidas(bebida, n);


		printf("\n\n");
	} while (opc != 6);
	return 0;
}

void configura(void)
{
	// Establece juego de caracteres castellano
	// Para que funcione hay que partir de un proyecto vac�o
	// No utilice la plantilla Aplicaci�n de consola C++
	setlocale(LC_ALL, "spanish");
}

int menu_principal(void)
{
	int opcion;
	char intro;
	do
	{
		printf("MENU PRINCIPAL\n");
		printf("==============\n");
		printf("1 - Alta bebida\n");
		printf("2 - Baja de bebida\n");
		printf("3 - Modificacion bebida\n");
		printf("4 - Consulta bebida\n");
		printf("5 - Listado bebidas\n");
		printf("6 - Cerrar Aplicacion\n");
		printf("7 - Seleccionar bebida\n");
		printf("Seleccione opcion:");
		scanf_s("%d", &opcion);
		intro = getchar();
		if (opcion < 1 || opcion>7)
			printf("\nOpcion inexistente.\n\n");
	} while (opcion < 1 || opcion>6);
	printf("\n");
	return opcion;
}

Datos leer_datos_bebida(char operacion)
{
	Datos bebida;
	char intro, kilocalorias[LONGCAD];

	if (operacion == 'A')
	{
		printf("Alta de una nueva bebida\n");
		printf("=======================\n");
	}
	else
	{
		printf("Modificar bebida (intro mantiene dato previo)\n");
		printf("==============================================\n");
	}
	printf("Ingrediente1:");
	gets_s(bebida.Ingrediente1, LONGCAD);
	printf("Ingrediente2:");
	gets_s(bebida.Ingrediente2, LONGCAD);
	if (operacion == 'A') // En el alta lee el login. En modificaci�n mantiene el anterior.
	{
		printf("Codigo:");
		gets_s(bebida.codigo, LONGCAD);
	}
	printf("Nombre:");
	gets_s(bebida.Nombre, LONGCAD);
	printf("Kilocalorias:");
	if (operacion == 'A')
	{
		scanf_s("%d", &bebida.kilocalorias);
		intro = getchar(); // Evitamos que se quede un '\n' en el buffer del teclado.
	}
	else // Si se trata de la modificaci�n un intro lo almacena como un cero para conservar anterior.
	{
		gets_s(kilocalorias, LONGCAD);
		if (strcmp(kilocalorias, "") == 0)
			bebida.kilocalorias = 0;
		else
			bebida.kilocalorias = atoi(kilocalorias);
	}
	return bebida;
}

void mostrar_titulos_bebidas(void)
{
	const char titulos[][LONGCAD] = { "Ingrediente1","Ingrediente2","Codigo","Nombre","Kilocalorias" };
	int i;

	for (i = 0;i < 5;i++)
		printf("%-15s", titulos[i]);
	printf("\n");
	for (i = 0;i < 15 * 5;i++)
		printf("=");
	printf("\n");
}

void mostrar_datos_bebidas(Datos u)
{
	printf("%-15s%-15s", u.Ingrediente1, u.Ingrediente2);
	printf("%-15s%-15s", u.codigo, u.Nombre);
	printf("%-15d\n", u.kilocalorias);
}

int alta_bebidas(Datos u[], int* pn, int longitud)
{
	int cod_error, n = *pn;

	if (n == longitud)
	{
		printf("La lista est� completa\n");
		cod_error = -1;
	}
	else
	{
		u[n] = leer_datos_bebida('A');
		n++; // Incrementamos el n�mero actual de usuarios
		*pn = n; // Actualizamos el argumento por referencia
		cod_error = 0;
	}
	return cod_error;
}

void leer_fichero_bebidas(Datos p[], int* pnumero, int longitud)
{
	FILE* fichero; // Puntero para manipular el fichero
	int num = 0; // Variable auxiliar para numero de usuarios le�dos
	int i, pos; // Variable bucle y posicion final cadena
	errno_t cod_error; // C�digo de error tras el proceso de apertura.
	char intro[2];

	cod_error = fopen_s(&fichero, "Usuarios.txt", "rt"); // Se intenta abrir el fichero de texto
	if (cod_error != 0)  // Si el fichero no se ha podido abrir
		*pnumero = 0; // La lista estar� vac�a
	else  // Si el fichero ha podido abrirse 
	{
		fscanf_s(fichero, "%d", &num); // Se lee la cantidad de registros
		if (num == 0) // Si esa cantidad es cero
			*pnumero = 0; // La lista estar� vac�a
		else  // Si hay registros para leer (seg�n el entero le�do)
		{
			if (num > longitud) // Si no hay memoria suficiente
			{
				printf("Memoria insuficiente para almacenar los datos del fichero\n");
				*pnumero = 0;
			}
			else // Si hay memoria suficiente
			{
				fgets(intro, 2, fichero); // Saltamos el intro que hay tras el n�mero (Ascii 10 y 13)
				for (i = 0;i < num;i++)  // Se leen los registros uno por uno
				{
					fgets((p + i)->Ingrediente1, LONGCAD, fichero);
					pos = strlen((p + i)->Ingrediente1);  // Calcula la longitud del nombre para ubicar el \n.
					(p + i)->Ingrediente1[pos - 1] = '\0';
					fgets((p + i)->Ingrediente2, LONGCAD, fichero);
					pos = strlen((p + i)->Ingrediente2);  // Calcula la longitud del nombre para ubicar el \n.
					(p + i)->Ingrediente2[pos - 1] = '\0';
					fgets((p + i)->codigo, LONGCAD, fichero);
					pos = strlen((p + i)->codigo);  // Calcula la longitud del nombre para ubicar el \n.
					(p + i)->codigo[pos - 1] = '\0';
					fgets((p + i)->Nombre, LONGCAD, fichero);
					pos = strlen((p + i)->Nombre);  // Calcula la longitud del nombre para ubicar el \n.
					(p + i)->Nombre[pos - 1] = '\0';
					fscanf_s(fichero, "%d", &(p + i)->kilocalorias);
					fgets(intro, 2, fichero); // Saltamos el intro que hay tras el n�mero (Ascii 10 y 13)
				}
				*pnumero = num;
			}
		}
		fclose(fichero); // Se cierra el fichero
	}
}

void listado_bebidas(Datos* usuario, int n)
{
	int i;

	if (n == 0)
		printf("En este momento no hay usuarios en la aplicaci�n\n");
	else
	{
		mostrar_titulos_bebidas();
		for (i = 0;i < n;i++)
			mostrar_datos_bebidas(usuario[i]);
	}
}
int consulta_bebidas(Datos* u, int n)
{
	char codigo[LONGCAD];
	int posicion = -1;
	int cod_error = 0;

	printf("Introduzca codigo a buscar:");
	gets_s(codigo, LONGCAD);
	posicion = posicion_bebidas(u, n, codigo);
	if (posicion == -1)
	{
		printf("No existe ninguna bebida con ese codigo\n");
		cod_error = -1;
	}
	else
	{
		mostrar_titulos_bebidas();
		mostrar_datos_bebidas(u[posicion]);
	}
	return cod_error;
}

int posicion_bebidas(Datos* u, int n, char* login)
{
	int i, posicion = -1;

	for (i = 0; i < n && posicion == -1; i++)
		if (strcmp(login, u[i].codigo) == 0)
			posicion = i;
	return posicion;
}

int modificacion_bebidas(Datos* u, int n)
{
	char codigo[LONGCAD];
	int posicion = -1;
	Datos nuevos_datos;
	int cod_error = 0;

	printf("Introduzca codigo a buscar:");
	gets_s(codigo, LONGCAD);
	posicion = posicion_bebidas(u, n, codigo);
	if (posicion == -1)
	{
		printf("No existe ninguna bebida con ese codigo\n");
		cod_error = -1;
	}
	else
	{
		mostrar_titulos_bebidas();
		mostrar_datos_bebidas(u[posicion]);
		printf("\n");
		nuevos_datos = leer_datos_bebida('M');
		if (strcmp(nuevos_datos.Ingrediente1, "") != 0)
			strcpy_s(u[posicion].Ingrediente1, LONGCAD, nuevos_datos.Ingrediente1);
		if (strcmp(nuevos_datos.Ingrediente2, "") != 0)
			strcpy_s(u[posicion].Ingrediente2, LONGCAD, nuevos_datos.Ingrediente2);
		if (strcmp(nuevos_datos.Nombre, "") != 0)
			strcpy_s(u[posicion].Nombre, LONGCAD, nuevos_datos.Nombre);
		if (nuevos_datos.kilocalorias != 0)
			u[posicion].kilocalorias = nuevos_datos.kilocalorias;
	}
	return cod_error;
}

int baja_bebidas(Datos* u, int* pn)
{
	char codigo[LONGCAD], respuesta[2];
	int encontrado = NO;
	int i, j;
	int n = *pn;
	int cod_error = 0;

	if (n == 0)
	{
		printf("La lista de bebidas esta vacia\n");
		cod_error = -1;
	}
	else
	{
		printf("Introduzca codigo a eliminar:");
		gets_s(codigo, LONGCAD);
		for (i = 0;i < n && encontrado == NO;i++)
			if (strcmp(codigo, u[i].codigo) == 0)
			{
				printf("Datos de la bebida encontrado:\n");
				mostrar_titulos_bebidas();
				mostrar_datos_bebidas(u[i]);
				printf("Desea borra esta bebida (S/N):");
				gets_s(respuesta, 2);
				if (strcmp(respuesta, "S") == 0 || strcmp(respuesta, "s") == 0)
				{
					encontrado = SI;
					for (j = i;j < n - 1;j++)
						u[j] = u[j + 1];
					*pn = n - 1;
				}
				else
					cod_error = -1;
			}
		if (encontrado == NO)
		{
			printf("No se ha encontrado la bebida\n");
			cod_error = -1;
		}
	}
	return cod_error;
}

int escribir_fichero_bebidas(Datos* lista, int numero)
{
	int i;
	FILE* fichero;
	errno_t err;

	err = fopen_s(&fichero, "Usuarios.txt", "w");
	if (err == 0) // Si el fichero se ha podido crear
	{
		fprintf(fichero, "%d\n", numero); // Se graba en el fichero el n�mero de usuarios
		for (i = 0;i < numero;i++)
		{

			fprintf(fichero, "%s\n", (lista + i)->Ingrediente1);
			fprintf(fichero, "%s\n", (lista + i)->Ingrediente2);
			fprintf(fichero, "%s\n", (lista + i)->codigo);
			fprintf(fichero, "%s\n", (lista + i)->Nombre);
			fprintf(fichero, "%d\n", (lista + i)->kilocalorias);
		}
		fclose(fichero);
	}
	else
		printf("Se ha producido un problema a la hora de grabar el fichero de usuarios\n");
	return err;
}
