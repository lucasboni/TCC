#include "DescritorLBP.h"
#include "ArquivoCSV.h"
#include <iostream>

using namespace std;

//Valores para o LBP uniforme
const char lookup[256] = {
	0, 1, 2, 3, 4, 58, 5, 6, 7, 58, 58, 58, 8, 58, 9, 10,
	11, 58, 58, 58, 58, 58, 58, 58, 12, 58, 58, 58, 13, 58, 14, 15,
	16, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58,
	17, 58, 58, 58, 58, 58, 58, 58, 18, 58, 58, 58, 19, 58, 20, 21,
	22, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58,
	58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58,
	23, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58,
	24, 58, 58, 58, 58, 58, 58, 58, 25, 58, 58, 58, 26, 58, 27, 28,
	29, 30, 58, 31, 58, 58, 58, 32, 58, 58, 58, 58, 58, 58, 58, 33,
	58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 34,
	58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58,
	58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 35,
	36, 37, 58, 38, 58, 58, 58, 39, 58, 58, 58, 58, 58, 58, 58, 40,
	58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 41,
	42, 43, 58, 44, 58, 58, 58, 45, 58, 58, 58, 58, 58, 58, 58, 46,
	47, 48, 58, 49, 58, 58, 58, 50, 51, 52, 58, 53, 54, 55, 56, 57 };

	//cria o descritor para o LBP
	void DescritorLBP::gerarDescritor(Mat& frame,Mat& LBP, LBPmodo modo,int etiqueta)
	{
		const int pulos =1;

		for(int x =1; x<frame.rows-1;x+=pulos) //desconciderar as bordas
		{
			for(int y =1; y<frame.cols-1;y+=pulos)
			{
				uchar pixel = 0;
				if(frame.at<uchar>(x,y)<frame.at<uchar>(x-1,y))
					pixel |=0x01;
				if(frame.at<uchar>(x,y)<frame.at<uchar>(x-1,y-1))
					pixel |=0x02;
				if(frame.at<uchar>(x,y)<frame.at<uchar>(x,y-1))
					pixel |=0x04;
				if(frame.at<uchar>(x,y)<frame.at<uchar>(x+1,y-1))
					pixel |=0x8;
				if(frame.at<uchar>(x,y)<frame.at<uchar>(x+1,y))
					pixel |=0x10;
				if(frame.at<uchar>(x,y)<frame.at<uchar>(x+1,y+1))
					pixel |=0x20;
				if(frame.at<uchar>(x,y)<frame.at<uchar>(x,y+1))
					pixel |=0x40;
				if(frame.at<uchar>(x,y)<frame.at<uchar>(x-1,y+1))
					pixel |=0x80;

				if(modo == LBPnormal)
					LBP.at<uchar>(x,y)= pixel;
				else
					LBP.at<uchar>(x,y)= lookup[pixel];
			}
		}
	}

	//gera um csv com valores de histograma da imagem em LBP
	void DescritorLBP::histograma(Mat& frame, LBPmodo modo,int etiqueta,float *vetor,boolean salvar)
	{
		Mat edge;
		cvtColor(frame,edge, CV_BGR2GRAY);
		int tam = 59; //tamanho do LBP uniforme
		if(modo == LBPnormal)
			tam = 256;
		Mat LBP(frame.rows,frame.cols,CV_8UC1);
		gerarDescritor(edge,LBP,modo,etiqueta);
		memset(vetor,0,tam*sizeof(float));
		for(int x =1; x<LBP.rows-1;x++)
		{
			for(int y =1; y<LBP.cols-1;y++)
				vetor[LBP.at<uchar>(x,y)]++;
		}
		if(salvar)
		{
			ArquivoCSV csv;
			csv.Criar(vetor,tam);
			csv.ListarCaracteristicas(vetor,tam,etiqueta);
		}
	}

	void DescritorLBP::histogramaOtimizado(Mat& frame, LBPmodo modo,int etiqueta,float *vetor,boolean salvar)
	{
		Mat edge;
		cvtColor(frame,edge, CV_BGR2GRAY);
		int tam = 59; //tamanho do LBP uniforme
		if(modo == LBPnormal)
			tam = 256;

		memset(vetor,0,tam*sizeof(float));


		const int pulos =1;
		uchar pixel;
		uchar  pixelAtual;

		for(int x =1; x<edge.rows-1;x+=pulos) //desconciderar as bordas
		{
			for(int y =1; y<edge.cols-1;y+=pulos)
			{
				pixel = 0;
				pixelAtual =  edge.at<uchar>(x,y);


				pixel |= (edge.at<uchar>(x-1,y-1) > pixelAtual) << 7;
				pixel |= (edge.at<uchar>(x-1,y) > pixelAtual) << 6;
				pixel |= (edge.at<uchar>(x-1,y+1) > pixelAtual) << 5;
				pixel |= (edge.at<uchar>(x,y+1) > pixelAtual) << 4;
				pixel |= (edge.at<uchar>(x+1,y+1) > pixelAtual) << 3;
				pixel |= (edge.at<uchar>(x+1,y) > pixelAtual) << 2;
				pixel |= (edge.at<uchar>(x+1,y-1) > pixelAtual) << 1;
				pixel |= (edge.at<uchar>(x,y-1) > pixelAtual) << 0;


				if(modo == LBPuniforme)
					pixel= lookup[pixel];

				vetor[pixel]++;		
			}
		}

		if(salvar)
		{
			ArquivoCSV csv;
			csv.Criar(vetor,tam);
			csv.ListarCaracteristicas(vetor,tam,etiqueta);
		}




	}

	void DescritorLBP::histogramaDetecao(Mat& frame, LBPmodo modo,int etiqueta,float *vetor)
	{
		int tam = 59; //tamanho do LBP uniforme
		if(modo == LBPnormal)
			tam = 256;
		Mat LBP(frame.rows,frame.cols,CV_8UC1);
		gerarDescritor(frame,LBP,modo,etiqueta);
		memset(vetor,0,tam*sizeof(float));
		for(int x =1; x<LBP.rows-1;x++)
		{
			for(int y =1; y<LBP.cols-1;y++)
				vetor[LBP.at<uchar>(x,y)]++;
		}
	}

	//destaca a imagem de acordo com o intervalo indicados usando a tecnica LBP
	void DescritorLBP::DetectarObjto(Mat& frame,int intervalos[][2], int quantidadeIntervalos, LBPmodo modo,int etiqueta)
	{
		Mat LBP(frame.rows,frame.cols,CV_8UC1);
		gerarDescritor(frame,LBP,modo,etiqueta);

		for(int x =1; x<LBP.rows-1;x++) //desconciderar as bordas
		{
			for(int y =1; y<LBP.cols-1;y++)
			{
				bool  dentroDoIntervalo =false;
				for(int j =0; j < quantidadeIntervalos;j++)
				{
					if((LBP.at<uchar>(x,y)>=intervalos[j][0] && LBP.at<uchar>(x,y)<=intervalos[j][1]))
					{
						dentroDoIntervalo = true;
						break;
					}
				} 
				if(!dentroDoIntervalo)
				{
					Vec3b & color = frame.at<Vec3b>(x,y);
					color[0] = 255;
					color[1] = 0;
					color[2] = 0;
				}
			}
		}
	}

	DescritorLBP::DescritorLBP(void)
	{

	}

	DescritorLBP::~DescritorLBP(void)
	{
	}
