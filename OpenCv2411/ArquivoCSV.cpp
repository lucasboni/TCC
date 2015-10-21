#include "ArquivoCSV.h"


//Cria um arquivo CSV com o histograma de algum descritor
void ArquivoCSV::Criar(long int histograma[],int tam)
{
	FILE *arquivo = fopen("C:\\Users\\Lucas\\Documents\\Visual Studio 2010\\Projects\\OpenCv2411\\Debug\\arquivo.csv", "w");
	for(int i =0;i<tam;i++)
	{
		char buffer[50];
		itoa(i, buffer, 10);
		fprintf(arquivo,buffer);
		fputc(';', arquivo);
		itoa(histograma[i], buffer, 10);
		fprintf(arquivo,buffer);
		//fputc('\r', arquivo);
		fputc('\n', arquivo);
	}
	fclose(arquivo);
}



void ArquivoCSV::Criar(float histograma[],int tam)
{
	float divisor =0; //transforma para valor de 0 até 1
	for(int i =0;i<tam;i++)
		divisor +=histograma[i];

	FILE *arquivo = fopen("C:\\Users\\Lucas\\Documents\\Visual Studio 2010\\Projects\\OpenCv2411\\Debug\\Histogramas\\arquivo.csv", "w");
	fprintf(arquivo,"%s\n","Intervalo;Magnitude");
	for(int i =0;i<tam;i++)
	{
		char buffer[50];
		itoa(i, buffer, 10);
		fprintf(arquivo,buffer);
		fputc(';', arquivo);
		fprintf(arquivo,"%f",histograma[i]/divisor);
		//fputc('\r', arquivo);
		fputc('\n', arquivo);
	}
	fclose(arquivo);
}

void ArquivoCSV::ListarCaracteristicas(float histograma[],int tam,int etiqueta)
{
	float divisor =0; //transforma para valor de 0 até 1
	for(int i =0;i<tam;i++)
		divisor +=histograma[i];

	if(divisor == 0)
		divisor=1;

	//divisor=1;


	FILE *arquivo = fopen("C:\\Users\\Lucas\\Documents\\Visual Studio 2010\\Projects\\OpenCv2411\\Debug\\Histogramas\\descritores.csv", "a");
	fputc('1', arquivo);
	for(int i =0;i<tam;i++)
	{
		char buffer[50];
		fputc(';', arquivo);
		fprintf(arquivo,"%lf",histograma[i]/divisor);
		//fputc('\r', arquivo);
		
	}
	fputc(';', arquivo);
	char buffer[50];
	itoa(etiqueta, buffer, 10);
	fprintf(arquivo,buffer);
	fputc('\n', arquivo);
	fclose(arquivo);
}

void ArquivoCSV::ListarCustos(float histograma[],int tam)
{
	FILE *arquivo = fopen("C:\\Users\\Lucas\\Documents\\Visual Studio 2010\\Projects\\OpenCv2411\\Debug\\Histogramas\\custos.csv", "a");
	for(int i =0;i<tam;i++)
	{
		char buffer[50];
		/*
		if(histograma[i] == 0)
			histograma[i]=histograma[i];
		else if(histograma[i]<0)           //arredondar pois esses valores ficam zerados no arquivo
		{
			if(-histograma[i]<0.000001)
				histograma[i] =-0.000001;
		}
		else if(histograma[i]<0.000001)
			histograma[i] =0.000001;
			*/
		fprintf(arquivo,"%f",histograma[i]);
		fputc(';', arquivo);
	}
	fputc('\n', arquivo);
	fclose(arquivo);
}

void ArquivoCSV::ListarDistanciaMinima(float histograma[],int tam)
{
	FILE *arquivo = fopen("C:\\Users\\Lucas\\Documents\\Visual Studio 2010\\Projects\\OpenCv2411\\Debug\\Histogramas\\menorDistancia.csv", "w");
	for(int i =0;i<tam;i++)
	{
		char buffer[50];
		fprintf(arquivo,"%f",histograma[i]);
		fputc(';', arquivo);
	}
	fclose(arquivo);
}

ArquivoCSV::ArquivoCSV(void)
{
}


ArquivoCSV::~ArquivoCSV(void)
{
}
