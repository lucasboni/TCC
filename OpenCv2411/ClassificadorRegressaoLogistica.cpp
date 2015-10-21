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
#include <exception>


void ClassificadorRegressaoLogistica::split(const string& s, char c,vector<string>& v) {
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

float ClassificadorRegressaoLogistica::funcaoLogistica(float valor)
{
	return (1/(1+exp(-valor)));
}

float ClassificadorRegressaoLogistica::funcaoCusto(float caracteristicas[],float etiquetas[],float w,int quantidade)
{
	float j=0;
	for(int i =0;i <quantidade;i++)
	{
		float h =funcaoLogistica(w*caracteristicas[i]);
		float constPositivo = etiquetas[i]*log(h);
		float constNegativo = (1-etiquetas[i])*log(1-h);
		j +=(constPositivo+constNegativo);
	}
	return (j/-quantidade);
}

//usado para descobrir o melhor w
float ClassificadorRegressaoLogistica::funcaoOtimizacao(float caracteristicas[],float etiquetas[],float tolerancia,int quantidade)
{
	float menorCusto = 9999;
	float menorW = 0;

	for(int i=-40;i<=40;i+=5)
	{

		const double alfa=0.001;
		float Wanterior;
		float Watual;
		float gradiente=40;

		Watual = Wanterior = i;

		while(gradiente < -tolerancia || gradiente > +tolerancia)
		{
			Wanterior=Watual;
			float gradienteAcumulado = 0;
			for(int i =0;i <quantidade;i++)
			{	
				gradienteAcumulado+= ((funcaoLogistica(caracteristicas[i]*Wanterior) - etiquetas[i])*caracteristicas[i]);
			}
			gradiente = gradienteAcumulado/quantidade;
			Watual = Wanterior-alfa*gradiente;
		}

		float custoAtual =funcaoCusto(caracteristicas,etiquetas,Watual,quantidade);
		if(custoAtual<menorCusto)
		{

		menorCusto = custoAtual;
		menorW = Watual;
		}

	}

	/*
	const float valorInicial = 40;

	const double alfa=0.001;
	float Wanterior = 40;
	float Watual = Wanterior;
	float gradiente = 40;

	float w1,w2,w3;

	Wanterior = valorInicial;
	Watual = Wanterior;
	gradiente = 40;
	while(gradiente < -tolerancia || gradiente > +tolerancia)
	{
		Wanterior=Watual;
		float gradienteAcumulado = 0;
		for(int i =0;i <quantidade;i++)
		{	
			gradienteAcumulado+= ((funcaoLogistica(caracteristicas[i]*Wanterior) - etiquetas[i])*caracteristicas[i]);
		}
		gradiente = gradienteAcumulado/quantidade;
		Watual = Wanterior-alfa*gradiente;
	}
	w1=Watual;


	Wanterior = 0;
	Watual = Wanterior;
	gradiente = 40;
	while(gradiente < -tolerancia || gradiente > +tolerancia)
	{
		Wanterior=Watual;
		float gradienteAcumulado = 0;
		for(int i =0;i <quantidade;i++)
		{	
			gradienteAcumulado+= ((funcaoLogistica(caracteristicas[i]*Wanterior) - etiquetas[i])*caracteristicas[i]);
		}
		gradiente = gradienteAcumulado/quantidade;
		Watual = Wanterior-alfa*gradiente;
	}
	w2=Watual;

	Wanterior = -valorInicial;
	Watual = Wanterior;
	gradiente = 40;
	while(gradiente < -tolerancia || gradiente > +tolerancia)
	{
		Wanterior=Watual;
		float gradienteAcumulado = 0;
		for(int i =0;i <quantidade;i++)
		{	
			gradienteAcumulado+= ((funcaoLogistica(caracteristicas[i]*Wanterior) - etiquetas[i])*caracteristicas[i]);
		}
		gradiente = gradienteAcumulado/quantidade;
		Watual = Wanterior-alfa*gradiente;
	}
	w3=Watual;

	float menorCusto = w1;
	
	
	if(funcaoCusto(caracteristicas,etiquetas,w3,quantidade)<funcaoCusto(caracteristicas,etiquetas,menorCusto,quantidade))
		menorCusto = w3;

	if(funcaoCusto(caracteristicas,etiquetas,w2,quantidade)<=funcaoCusto(caracteristicas,etiquetas,menorCusto,quantidade))
		menorCusto = w2;

	//if(menorCusto == valorInicial || menorCusto == -valorInicial)
	//	menorCusto = w2;

	*/
	cout <<  menorW << endl;
	return menorW;
}

void getDir(string d, vector<string> & f)
{
	FILE* pipe =  NULL;
	string pCmd = "dir /B /S \"" + d+"\"";
	char buf[256];

	if( NULL == (pipe = _popen(pCmd.c_str(),"rt")))
	{
		cout<<"Shit"<<endl;
		return;
	}

	while (!feof(pipe))
	{
		if(fgets(buf,256,pipe) != NULL)
		{
			f.push_back(string(buf));
		}

	}
	_pclose(pipe);
}

int ClassificadorRegressaoLogistica::gerarClassificador(TipoDescritor descritor,int numero,float *vetor)
{

	vector<string> dat;
	
	getDir("C:\\Users\\Lucas\\Documents\\Visual Studio 2010\\Projects\\OpenCv2411\\Debug\\pos\\", dat);

	string aaa=dat[0];
	Mat frame  = imread(aaa);

	try
  {
	char *path=NULL;
	size_t size =0;
	path=getcwd(path,size);
	string diretorioPositivo(path);
	string diretorioNegativo(path);
	diretorioPositivo ="C:\\Users\\Lucas\\Documents\\Visual Studio 2010\\Projects\\OpenCv2411\\Debug\\pos\\";
	diretorioNegativo ="C:\\Users\\Lucas\\Documents\\Visual Studio 2010\\Projects\\OpenCv2411\\Debug\\neg\\";
	
	for(int i =1;i<=numero;i++) //positivo
	{
		string Result;          // string which will contain the result
		ostringstream convert;   // stream used for the conversion
		convert << i;      // insert the textual representation of 'Number' in the characters in the stream
		Result = convert.str();
		std::stringstream ss;
		string temp =diretorioPositivo + Result +".png";		
		Mat frame  = imread(temp);
		Candidato candidato(10,10);
		candidato.Histograma(frame,descritor,1,vetor,true);
	}

	for(int i =1;i<=numero;i++) //negativo
	{
		string Result;          // string which will contain the result
		ostringstream convert;   // stream used for the conversion
		convert << i;      // insert the textual representation of 'Number' in the characters in the stream
		Result = convert.str();
		std::stringstream ss;
		string temp =diretorioNegativo + Result +".png";		
		Mat frame  = imread(temp);
		Candidato candidato(10,10);
		candidato.Histograma(frame,descritor,0,vetor,true);
	}

	vector<vector<string>> caracteristicas;

	string line;
	ifstream myfile ("C:\\Users\\Lucas\\Documents\\Visual Studio 2010\\Projects\\OpenCv2411\\Debug\\Histogramas\\descritores.csv"); // ifstream = padrão ios:in
	if (myfile.is_open())
	{
		while (! myfile.eof() ) //enquanto end of file for false continua
		{
			getline (myfile,line); // como foi aberto em modo texto(padrão)
			vector<string>  linhas;
			split(line, ';', linhas);
			caracteristicas.push_back(linhas);                       //e não binário(ios::bin) pega cada linha
		}
		myfile.close();
	}

	float* vetorCarct = new float[caracteristicas.size()-1];//deixei o valor fixo pois o visual c++ bugava quando eu alocava dinamicamnee com um valor exato
	float* vetorEtiquetas = new float[caracteristicas.size()-1];
	
	for(int i =0;i<(caracteristicas.size()-1);i++)
	{
		std::string::size_type sz;     // alias of size_t
		float mars = std::stof (caracteristicas[i][caracteristicas[0].size()-1],&sz);//pega as etiquetas
		vetorEtiquetas[i] = mars;
	}
	float* custos = new float[caracteristicas[0].size()-1];
	for(int j =0; j<caracteristicas[0].size()-1;j++) //numero de caracteristicas
	{
		for(int i =0;i<caracteristicas.size()-1;i++) //numero de amostras
		{
			std::string::size_type sz;     // alias of size_t
			float mars = std::stof (caracteristicas[i][j],&sz);
			vetorCarct[i]=mars;
		}
		float teste = funcaoOtimizacao(vetorCarct,vetorEtiquetas,0.01,caracteristicas.size()-1);
		custos[j] = teste; 
		cout << teste<<endl;
	}

	ArquivoCSV csv;
	csv.ListarCustos(custos,caracteristicas[0].size()-1);
	}
	catch (exception& e)
  {
    cout << "Erro: " << e.what() << endl;
  }
	return 0;
}

ClassificadorRegressaoLogistica::ClassificadorRegressaoLogistica(void)
{
}

ClassificadorRegressaoLogistica::~ClassificadorRegressaoLogistica(void)
{
}
