#pragma once

#include <iostream>
#include <string>
#include<stdio.h>
#include<math.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class ArquivoCSV
{
public:

	void Criar(long int histograma[],int tam);
	void Criar(float histograma[],int tam);

	void ListarCaracteristicas(float histograma[],int tam,int etiqueta);

	void ListarCustos(float histograma[],int tam);

	void ListarDistanciaMinima(float histograma[],int tam);

	ArquivoCSV(void);
	~ArquivoCSV(void);
};

