#include "Candidato.h"
#include "ClassificadorRegressaoLogistica.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "DescritorLBP.h"
#include "ArquivoCSV.h"

using namespace std;

static int nomeFigura =1;

void Candidato::split(const string& s, char c,vector<string>& v) {
	string::size_type i = 0;
	string::size_type j = s.find(c);

	while (j != string::npos) {
		v.push_back(s.substr(i, j-i));
		i = ++j;
		j = s.find(c, j);

		if (j == string::npos)
			v.push_back(s.substr(i, s.length()));
	}
}

Candidato::Candidato(int TamX,int TamY)
{
	tamX=TamX;
	tamY=TamY;
}

void Candidato::MarcarImagemLBP(Mat& frame,int x, int y,int passos)
{
	Vec3b & color = frame.at<Vec3b>(y+tamY/2,(x+tamX/2));
								color[0] = 255;
								color[1] = 0;
								color[2] = 0;


								rectangle(frame,
							cvPoint(x,y),                     // South-West point 
							cvPoint(tamX+x, tamY+y), // North-East point
							cvScalar(255, 0, 0, 0), 
							1, 8, 0);


	/*for(int i = 0; i<passos ; i++)
						{
							for(int j = 0; j<passos ; j++)
							{
								Vec3b & color = frame.at<Vec3b>(y+tamY/2+j,(x+tamX/2+i));
								color[0] = 255;
								color[1] = 0;
								color[2] = 0;
							}
						}
						*/
}

void Candidato::VarrerFrame(Mat& frame,TipoDescritor Tipo,int passos,float limiar1,float limiar2,TipoClassificador classificador)
{
	Mat frameCopia;
	frame.copyTo(frameCopia);
	//cvtColor(frame, edges, CV_BGR2GRAY);

	const int posX = tamX;
	const int posY = tamY;

	//namedWindow( "Display window", WINDOW_AUTOSIZE );

	//cvtColor(frame,frame, CV_BGR2GRAY);
	for(int y = 0;y<frame.rows-tamY;y+=passos)
	{

		for(int x = 0;x<frame.cols-tamX;x+=passos)//cria matriz dos descritores
		{

			Mat subBlock=frame(Rect(x, y, tamX, tamY));//pega uma parte da imagem 6x6pixel
#pragma region regressao
			if(classificador==TipoClassificador::regressaoLogistica)
			{
				float probabilidade = DetectarRegressao(subBlock,Tipo);

				if(probabilidade >= limiar1 && probabilidade <= limiar2)
					{
						rectangle(frameCopia,
							cvPoint(x,y),                     // South-West point 
							cvPoint(tamX+x, tamY+y), // North-East point
							cvScalar(255, 0, 0, 0), 
							1, 8, 0);
					/*	if(!ok)
						{
							probabilidadeok = probabilidade;
							xok =x;
							yok=y;
							ok=true;
							x+=passos;
						}
						else if(probabilidadeok<probabilidade)
						{
							probabilidadeok = probabilidade;
							xok =x;
							yok=y;
							x+=passos;
						}
						else
						{
						
						x+=passos;
						ok=false;
						}*/
					}
				

				//if(Tipo == TipoDescritor::LBPdesc || Tipo == TipoDescritor::LBPUNIFORMEdesc)
				//{// será atribuido a detecção ao pixel central
				//	if (probabilidade > limiar)
				//		MarcarImagemLBP(frameCopia,x,y,passos);
				//}
				//else
				//{
				//	
				//}

				//imwrite( "C:\\Users\\Lucas\\Documents\\Visual Studio 2010\\Projects\\\OpenCv2411\\Debug\\Gray_Image.jpg", frameCopia );
#pragma endregion
			}
			else if (classificador==TipoClassificador::distanciaMinima)
			{
				float probabilidade = DetectarDistanciaMinima(subBlock,Tipo);
				if(Tipo == TipoDescritor::LBPdesc || Tipo == TipoDescritor::LBPUNIFORMEdesc)
				{// será atribuido a detecção ao pixel central
					if (probabilidade >= 0)
						MarcarImagemLBP(frameCopia,x,y,passos);
				}
				else
				{
					if(probabilidade >= 0)
					{
						rectangle(frameCopia,
							cvPoint(x,y),                     // South-West point 
							cvPoint(tamX+x, tamY+y), // North-East point
							cvScalar(255, 0, 0, 0), 
							1, 8, 0);

					}
				}
			}
			else
			{
				bool probabilidade = DetectarDistanciaMinimaPositiva(subBlock,Tipo);
				if (probabilidade)
					MarcarImagemLBP(frameCopia,x,y,passos);
			}


			//imshow( "Display window", frameCopia );  
			//extrai as caracteristicas do pedaço da imagem 
		}
	}
	imwrite( "C:\\Users\\Lucas\\Documents\\Visual Studio 2010\\Projects\\\OpenCv2411\\Debug\\Gray_Image.jpg", frameCopia );
}

void Candidato::VarrerFrameVariasClasses(Mat& frame,TipoDescritor Tipo,int passos,float limiar1,float limiar2,int numeroVistorias,TipoClassificador classificador)
{
	Mat frameCopia;
	frame.copyTo(frameCopia);
	const int posX = tamX;
	const int posY = tamY;

	for(int y = 0;y<frame.rows-tamY;y+=passos)
	{
		for(int x = 0;x<frame.cols-tamX;x+=passos)//cria matriz dos descritores
		{
				Mat subBlock=frame(Rect(x, y, tamX, tamY));//pega uma parte da imagem de acordo com o candidato
				if(DetectarRegressao(subBlock,Tipo,limiar1,limiar2)>= numeroVistorias)
					{
						rectangle(frameCopia,
							cvPoint(x,y),                     // South-West point 
							cvPoint(tamX+x, tamY+y), // North-East point
							cvScalar(255, 0, 0, 0), 
							1, 8, 0);
			}
		}
	}
	string Result;          // string which will contain the result
	ostringstream convert;   // stream used for the conversion
	convert << nomeFigura;      // insert the textual representation of 'Number' in the characters in the stream
	Result = convert.str();
	string diretorio = "C:\\Users\\Lucas\\Documents\\Visual Studio 2010\\Projects\\\OpenCv2411\\Debug\\TesteImagens\\";
	diretorio =diretorio+(Result+".jpg");
	imwrite(diretorio, frameCopia );
	nomeFigura++;
}

int Candidato::Histograma(Mat& frame,TipoDescritor Tipo,int etiqueta,float *vetor,boolean salvar)
{
	int tam =0 ;
	DescritorLBP LBP;
	DescritorHOG HOG;
	switch(Tipo)
	{
	case LBPdesc:
		//LBP.histograma(frame,LBPnormal,etiqueta,vetor,salvar);
		LBP.histogramaOtimizado(frame,LBPnormal,etiqueta,vetor,salvar);
		break;
	case LBPUNIFORMEdesc:
		LBP.histogramaOtimizado(frame,LBPuniforme,etiqueta,vetor,salvar);
		break;
	case HOGdesc:
		tam = HOG.histograma(frame,HOGnormal,vetor,etiqueta,salvar);
		break;
	case HOGNormalizadodesc:
		HOG.histograma(frame,HOGnormalizado,vetor,etiqueta,salvar);
		break;
	case HOGIntegraldesc:
		HOG.histograma(frame,HOGintegral,vetor,etiqueta,salvar);
		break;
	case HAARdesc:
		break;
	}

	return tam;
}

//faz a conta do descritor e faz a conta de custo
//retorna a probabilidade
float Candidato::DetectarRegressao(Mat& frame,TipoDescritor Tipo)
{
	//carregar em um vetor o custo
	//calcular descritor
	//fazer a função logistica de probabilidade

	long float tempsum =0;

#pragma region carregar custos
	string line;
	vector<string>  custos;

	ifstream myfile ("C:\\Users\\Lucas\\Documents\\Visual Studio 2010\\Projects\\OpenCv2411\\Debug\\Histogramas\\custos.csv"); // ifstream = padrão ios:in
	if (myfile.is_open())
	{
		while (! myfile.eof() ) //enquanto end of file for false continua
		{
			getline (myfile,line); // como foi aberto em modo texto(padrão)

			split(line, ';', custos);
			break;
		}
		myfile.close();
	}

	float *VetorCustos = new float[custos.size()-1];
	for(int i =0;i<custos.size()-1;i++)
	{
		std::string::size_type sz;     // alias of size_t
		float mars = std::stof (custos[i],&sz);//pega as etiquetas
		VetorCustos[i] = mars;
	}

#pragma endregion
#pragma region calcular descritor
	float *vetor = new float[custos.size()-2];
	Histograma(frame,Tipo,1,vetor,false);
	float teste;
	for(int i =0;i<custos.size()-2;i++)
	{
		if(vetor[i] !=0)
			tempsum+=vetor[i];
		teste=vetor[i];
	}


#pragma endregion

#pragma region função logistica
	float soma = VetorCustos[0];
	float probabilidade;
	if(tempsum !=0)
	{
		
	for(int i =0;i<custos.size()-2;i++)
	{
		if(VetorCustos[i+1] !=0 && vetor[i] !=0)
			soma+=((vetor[i]*VetorCustos[i+1]));
	}
	ClassificadorRegressaoLogistica RegLog;
	probabilidade = RegLog.funcaoLogistica(soma/tempsum);
	}
	else
	{
		probabilidade = 0;
	}
	
#pragma endregion
	//cout << soma<<endl;
	cout << probabilidade<<endl;

	return probabilidade;
}

int Candidato::DetectarRegressao(Mat& frame,TipoDescritor Tipo,float limiar1,float limiar2)
{

#pragma region carregar classificadores
	string line;
	vector<string>  custo;
	vector<float*> custos;

	int tamCusto=0;

	ifstream myfile ("C:\\Users\\Lucas\\Documents\\Visual Studio 2010\\Projects\\OpenCv2411\\Debug\\Histogramas\\custos.csv"); // ifstream = padrão ios:in
	if (myfile.is_open())
	{
		while (! myfile.eof() ) //enquanto end of file for false continua
		{
			getline (myfile,line); // como foi aberto em modo texto(padrão)
			custo.clear();
			split(line, ';', custo);


			if(tamCusto==0)
				tamCusto = custo.size()-1;

			if(custo.size()==0)
				break;

			float *VetorCustos = new float[tamCusto];

			for(int i =0;i<custo.size()-1;i++)
			{
				std::string::size_type sz;     // alias of size_t
				float mars = std::stof (custo[i],&sz);//pega as etiquetas
				VetorCustos[i] = mars;
			}
			custos.push_back(VetorCustos);

		}
		myfile.close();
	}
#pragma endregion

	int vitorias=0;

	long float tempsum =0;

#pragma region calcular descritor
	float *vetor = new float[tamCusto-1];
	Histograma(frame,Tipo,1,vetor,false);
	float teste;
	for(int i =0;i<tamCusto-1;i++)
	{
		if(vetor[i] !=0)
			tempsum+=vetor[i];
		teste=vetor[i];
	}
#pragma endregion

#pragma region função logistica

	for(int p =0;p<custos.size();p++)
	{

	float soma = custos[p][0];
	float probabilidade;
	if(tempsum !=0)
	{	
		for(int i =0;i<tamCusto-1;i++)
		{
			if(custos[p][i+1] !=0)
				soma+=((vetor[i]*custos[p][i+1]));
		}
		ClassificadorRegressaoLogistica RegLog;
		probabilidade = RegLog.funcaoLogistica(soma/tempsum);
	}
	else
		probabilidade = 0;

	if(probabilidade>=limiar1 && probabilidade <=limiar2)
		vitorias++;
	
	}

#pragma endregion
	//if(vitorias>0)
	//	cout << vitorias<<endl;

	//if(probabilidade>=limiar1 && probabilidade <=limiar2)


		return vitorias;
}

float Candidato::DetectarDistanciaMinima(Mat& frame,TipoDescritor Tipo)
{
	float tempsum = 0;

#pragma region carregar custos
	string line;
	vector<string>  custos;
	ifstream myfile ("C:\\Users\\Lucas\\Documents\\Visual Studio 2010\\Projects\\OpenCv2411\\Debug\\Histogramas\\menorDistancia.csv"); // ifstream = padrão ios:in
	if (myfile.is_open())
	{
		while (! myfile.eof() ) //enquanto end of file for false continua
		{
			getline (myfile,line); // como foi aberto em modo texto(padrão)

			split(line, ';', custos);
			break;
		}
		myfile.close();
	}

	float *VetorCustos = new float[custos.size()-1];
	for(int i =0;i<custos.size()-1;i++)
	{
		std::string::size_type sz;     // alias of size_t
		float mars = std::stof (custos[i],&sz);//pega as etiquetas
		VetorCustos[i] = mars;
	}

#pragma endregion

#pragma region calcular descritor
	float *vetor = new float[custos.size()-2];
	Histograma(frame,Tipo,1,vetor,false);
	for(int i =0;i<custos.size()-2;i++)
		tempsum+=vetor[i];

#pragma endregion

#pragma region detectar objeto

	float classificacao = 0;
	for(int i = 0 ; i<custos.size()-2;i++)
		classificacao +=(vetor[i]*VetorCustos[i]/tempsum);

	classificacao+=VetorCustos[custos.size()-2];
#pragma endregion

	cout << classificacao << endl;
	return classificacao;
}

bool Candidato::DetectarDistanciaMinimaPositiva(Mat& frame,TipoDescritor Tipo)
{
	float tempsum = 0;

#pragma region carregar custos
	string line;
	vector<string>  custos;
	ifstream myfile ("C:\\Users\\Lucas\\Documents\\Visual Studio 2010\\Projects\\OpenCv2411\\Debug\\Histogramas\\menorDistancia.csv"); // ifstream = padrão ios:in
	if (myfile.is_open())
	{
		while (! myfile.eof() ) //enquanto end of file for false continua
		{
			getline (myfile,line); // como foi aberto em modo texto(padrão)

			split(line, ';', custos);
			break;
		}
		myfile.close();
	}

	float *VetorCustos = new float[custos.size()-3];
	for(int i =0;i<custos.size()-3;i++)
	{
		std::string::size_type sz;     // alias of size_t
		float mars = std::stof (custos[i],&sz);//pega as etiquetas
		VetorCustos[i] = mars;
	}

	std::string::size_type sz;     // alias of size_t
	float mars1 = std::stof (custos[custos.size()-3],&sz);//pega as etiquetas
	float mars2 = std::stof (custos[custos.size()-2],&sz);//pega as etiquetas


	float limiar1 = mars1;
	float limiar2 = mars2;

#pragma endregion

#pragma region calcular descritor
	float *vetor = new float[custos.size()-4];
	Histograma(frame,Tipo,1,vetor,false);
	for(int i =0;i<custos.size()-4;i++)
		tempsum+=vetor[i];

#pragma endregion

#pragma region detectar objeto

	float classificacao = 0;
	for(int i = 0 ; i<custos.size()-4;i++)
		classificacao +=(vetor[i]*VetorCustos[i]/tempsum);

	classificacao+=VetorCustos[custos.size()-4];
#pragma endregion

	cout << classificacao << endl;

	classificacao = sqrt(classificacao*classificacao);

	if(classificacao>=limiar1 && classificacao<=limiar2)
		return true;

	return false;
}

Candidato::Candidato(void)
{
}

Candidato::~Candidato(void)
{
}
