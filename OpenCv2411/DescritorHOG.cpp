#include "DescritorHOG.h"
#include "ArquivoCSV.h"
#include <math.h>

using namespace std;

//gera duas matriz contendo os gradientes decompostos, uma para x e outa para y
void DescritorHOG::gerarDescritor(Mat& frame,SubBlocoHOG& blocosHOG)
{

	//inicia structs dos blocos menores
	blocosHOG;
	blocosHOG.bloco = new DadoHOG*[frame.rows];   //aloca vetor dinamicamente
	for(int i = 0; i < frame.rows; ++i)
		blocosHOG.bloco[i] = new DadoHOG[frame.cols];

	/*for(int y =0; y<(frame.rows);y++) //desconciderar as bordas
	{
		for(int x =0; x<(frame.cols);x++)
		{
			blocosHOG.bloco[y][x].x=0;
			blocosHOG.bloco[y][x].y=0;
			blocosHOG.bloco[y][x].magnitude =0;
			blocosHOG.bloco[y][x].graus =0;
		}
	}*/

#pragma region conta do descritor
	for(int y =1; y<(frame.rows)-1;y++) //desconciderar as bordas
	{
		for(int x =1; x<(frame.cols)-1;x++)
		{
			blocosHOG.bloco[y][x].x=frame.at<uchar>(y+1,x) - frame.at<uchar>(y-1,x);
			blocosHOG.bloco[y][x].y=frame.at<uchar>(y,x+1) - frame.at<uchar>(y,x-1);
			blocosHOG.bloco[y][x].magnitude = sqrt((double)(blocosHOG.bloco[y][x].y*blocosHOG.bloco[y][x].y + blocosHOG.bloco[y][x].x*blocosHOG.bloco[y][x].x));
			double cos = blocosHOG.bloco[y][x].x/(blocosHOG.bloco[y][x].magnitude+0.1);//evitar divisão por zero
			blocosHOG.bloco[y][x].graus = acos(cos)*180/PI;
			if(blocosHOG.bloco[y][x].graus<0)
				blocosHOG.bloco[y][x].graus = blocosHOG.bloco[y][x].graus +180;
			//else if(blocosHOG.bloco[y][x].graus>180)
			//	blocosHOG.bloco[y][x].graus = blocosHOG.bloco[y][x].graus -180;
		}
	}
#pragma endregion
}

void DescritorHOG::gerarDescritorIntegral(Mat& edges,BlocoHOGintegtal** descritor)
{
	Mat frame;
	cvtColor(edges, frame, CV_BGR2GRAY);

	for(int y =1; y<(frame.rows)-1;y++) //desconciderar as bordas
	{
		for(int x =1; x<(frame.cols)-1;x++)
		{
			int magnitudex = frame.at<uchar>(x+1,y) - frame.at<uchar>(x-1,y);
			int magnitudey=frame.at<uchar>(x,y+1) - frame.at<uchar>(x,y-1);
			float magnitude = sqrt((double)(magnitudey*magnitudey + magnitudex*magnitudex ));
			float cos = magnitudex/(magnitude+0.000000001);//evitar divisão por zero
			float graus = acos(cos)*180/PI;
			if(graus<0)
				graus = graus +180;

			for(int intInflu=1;intInflu<=9;intInflu++)
			{
				float wk =0;	
				float Distanciak = fabsf(graus-(intInflu*20-10));
				if(Distanciak<20)
					wk =fabsf(1 -Distanciak/20.0);
				descritor[x][y].intervalo[intInflu-1]+=magnitude*wk;
			}

			for(int k =0 ;k<x;k++)
			{
				for(int angulo =0;angulo<9;angulo++)
					descritor[x][y].intervalo[angulo] +=descritor[k][y].intervalo[angulo];
			}
			for(int k =0 ;k<y;k++)
			{
				for(int angulo =0;angulo<9;angulo++)
					descritor[x][y].intervalo[angulo] +=descritor[x][k].intervalo[angulo];
			}
		}
	}
}

//Cria um histograma e grava em um arquivo CSV
int DescritorHOG::histograma(Mat& edges,HOGmodo modo,float *histograma,int etiqueta,boolean salvar)
{

	const int divisorDeBlocos =25;

	Mat frame;
	cvtColor(edges, frame, CV_BGR2GRAY);

	int quantColunas=frame.cols/divisorDeBlocos;//x
	int quantLinhas=frame.rows/divisorDeBlocos; //y

	int offSet = 0;//offset para o histograma

	if(modo != HOGintegral) //Se não for o modo integral
	{
		if(salvar)
		{
			histograma = new float[(quantColunas-1)*(quantLinhas-1)*2*2*9];
			memset(histograma,0,(quantColunas-1)*(quantLinhas-1)*2*2*9*sizeof(float)); //limpa o vetor
		}
		

#pragma region gerar matriz do descritor
		SubBlocoHOG** vetorDescritor;
		vetorDescritor = new SubBlocoHOG*[quantLinhas];   //aloca vetor dinamicamente
		for(int i = 0; i < quantLinhas; i++) 
			vetorDescritor[i] = new SubBlocoHOG[quantColunas];

		
		SubBlocoHOG blocoHOG;
		for(int y = 0;y<quantLinhas;y++)
		{
			for(int x = 0;x<quantColunas;x++)//cria matriz dos descritores
			{
				Mat subBlock=frame(Rect(x*divisorDeBlocos, y*divisorDeBlocos, divisorDeBlocos, divisorDeBlocos));//pega uma parte da imagem 6x6pixel
				gerarDescritor(subBlock,blocoHOG);
				vetorDescritor[y][x].bloco=blocoHOG.bloco;
			}
		}
#pragma endregion
#pragma region gerar histograma normal

		for(int iy=0;iy<quantLinhas-1;iy++)
		{
			for(int ix=0;ix<quantColunas-1;ix++)
			{
				for(int solapamentox = 0; solapamentox<2;solapamentox++)
				{
					for(int solapamentoy = 0; solapamentoy<2;solapamentoy++)
					{
						for(int x=0;x<divisorDeBlocos-1;x++)
						{
							for(int y=0;y<divisorDeBlocos-1;y++)
							{
								if(vetorDescritor[iy+solapamentoy][ix+solapamentox ].bloco[y][x].magnitude ==0) //não inicia a conta pois o resutado será zero
									continue;

								float wx =0;
								float wy =0;

								//if(ix>0) //descartar os subblocos das bordas
								{
									wx=fabsf((x)-((divisorDeBlocos/2)));
									wx=wx/(divisorDeBlocos/2);
									wx=1-wx;
								}
								//if(iy>0) //descartar os subblocos das bordas
								{
									wy=fabsf((y)-((divisorDeBlocos/2)));
									wy=wx/(divisorDeBlocos/2);
									wy=1-wy;
								}
								for(int intInflu=1;intInflu<=9;intInflu++)
								{
									float wk =0;	
									float Distanciak = fabsf(vetorDescritor[iy+solapamentoy][ix+solapamentox].bloco[x][y].graus-(intInflu*20-10));
									if(Distanciak<20)
									{
										wk =fabsf(1 -Distanciak/20.0);
										histograma[offSet+intInflu-1]=+vetorDescritor[iy+solapamentoy][ix+solapamentox].bloco[x][y].magnitude*wk*wx*wy;
									}
								}
							}
						}
						offSet+=9;
					}
				}
			}
		}

#pragma endregion
	}
	else
	{
#pragma region histograma integral

		histograma = new float[(quantColunas-1)*(quantLinhas-1)*2*2*9];
		memset(histograma,0,(quantColunas-1)*(quantLinhas-1)*2*2*9*sizeof(float)); //limpa o vetor

		BlocoHOGintegtal** descritor;
		descritor = new BlocoHOGintegtal*[frame.rows]; //linhaxcoluna
		for(int i = 0; i < frame.rows; i++)
			descritor[i] = new BlocoHOGintegtal[frame.cols];

		for(int x=0;x<frame.rows;x++)
		{
			for(int y=0;y<frame.cols;y++)
				memset(descritor[x][y].intervalo,0,sizeof(float)*9); 	
		}
		gerarDescritorIntegral(frame,descritor);
		for(int x =0;x<frame.rows-6;x +=12)
		{
			for(int y =0;y<frame.cols-6;y+=12)
			{
				for(int graus =0;graus<9;graus++)
				{
					histograma[offSet + graus]=descritor[x+6][y+6].intervalo[graus] - descritor[x][y+6].intervalo[graus]- descritor[x+6][y].intervalo[graus] +descritor[x][y].intervalo[graus];
				}
				offSet+=9;
				for(int graus =0;graus<9;graus++)
				{
					histograma[offSet + graus]=descritor[x+6+6][y+6].intervalo[graus] - descritor[x+6][y+6].intervalo[graus]- descritor[x+6+6][y].intervalo[graus] +descritor[x+6][y].intervalo[graus];
				}
				offSet+=9;
				for(int graus =0;graus<9;graus++)
				{
					histograma[offSet + graus]=descritor[x+6][y+6+6].intervalo[graus] - descritor[x][y+6+6].intervalo[graus]- descritor[x+6][y+6].intervalo[graus] +descritor[x][y+6].intervalo[graus];
				}
				offSet+=9;
				for(int graus =0;graus<9;graus++)
				{
					histograma[offSet + graus]=descritor[x+6+6][y+6+6].intervalo[graus] - descritor[x+6][y+6+6].intervalo[graus]- descritor[x+6+6][y+6].intervalo[graus] +descritor[x+6][y+6].intervalo[graus];
				}
				offSet+=9;
			}

		}


#pragma endregion
	}

	if(modo == HOGmodo::HOGnormalizado)
		normalizar(histograma);
	
	if(salvar)
	{
		ArquivoCSV arquivoCSV;
		arquivoCSV.Criar(histograma,(quantColunas-1)*(quantLinhas-1)*2*2*9);
		arquivoCSV.ListarCaracteristicas(histograma,(quantColunas-1)*(quantLinhas-1)*2*2*9,etiqueta);
	}
	return (quantColunas-1)*(quantLinhas-1)*2*2*9;
}

//normaliza o vetorHOG(norma euclidiana)
void DescritorHOG::normalizar(float vetor[])
{
	float divisor =0;
	for(int i =0;i<9;i++)
		divisor +=vetor[i]*vetor[i];
	divisor = sqrt(divisor);
	divisor = sqrt((divisor*divisor)+0.00001);

	for(int i =0;i<9;i++)
		vetor[i] = vetor[i]/divisor;
}

DescritorHOG::DescritorHOG(void)
{
}

DescritorHOG::~DescritorHOG(void)
{
}
