#include "ClassificadorDistanciaMinimaPositiva.h"


void ClassificadorDistanciaMinimaPositiva::split(const string& s, char c,vector<string>& v)
{
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




void ClassificadorDistanciaMinimaPositiva::GerarDescritorMedio(TipoDescritor descritor,int numero,float *vetor)
{
	//gerar descritores positivo e negativo
	//calcular valor medio do positivo e negativo
	//quardar cdv com valores medio

#pragma region gerar descritor
	char *path=NULL;
	size_t size =0;
	path=getcwd(path,size);
	string diretorioPositivo(path);
	string diretorioNegativo(path);
	diretorioPositivo ="C:\\Users\\Lucas\\Documents\\Visual Studio 2010\\Projects\\OpenCv2411\\Debug\\pos\\";

	
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
		candidato.Histograma(frame,descritor,1,vetor);
	}
#pragma endregion

#pragma region gerar descritor medio
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

	float *vetorMedPos = new float[caracteristicas[0].size()];//deixei o valor fixo pois o visual c++ bugava quando eu alocava dinamicamnee com um valor exato//problema removido

	memset(vetorMedPos,(float)0,sizeof(float)*caracteristicas[0].size());

	float mars;
	for(int i =0;i<caracteristicas.size()-1;i++)
	{
		std::string::size_type sz;     // alias of size_t
		int temp =caracteristicas[i].size()-1;
		mars = std::stof (caracteristicas[i][temp],&sz);//pega as etiquetas
		for(int j =1;j<caracteristicas[i].size()-1;j++) //remove o primeiro que esta gravado como 1
		{
			std::string::size_type sz;     // alias of size_t
			mars = std::stof (caracteristicas[i][j],&sz);//pega as etiquetas
			vetorMedPos[j-1] +=mars;
		}
	}

	const int numeroDeCaracterisicas = caracteristicas[0].size()-1;
	const int numeroDeAmostras=caracteristicas.size()-1;
	const int tam = caracteristicas[0].size()+1;

	for(int j =0;j<numeroDeCaracterisicas;j++)
		vetorMedPos[j]=vetorMedPos[j]/numeroDeAmostras; //faz a media de cada caracteristicas

#pragma endregion

	//int a = caracteristicas[0].size()-1;
	float *vetorClassificador = new float[tam];
	float soma = 0;
	for(int i =0;i<numeroDeCaracterisicas-1;i++)
	{
		vetorClassificador[i] = vetorMedPos[i]*vetorMedPos[i];
		soma= -(vetorMedPos[i]*vetorMedPos[i]);
	}
	soma = 0.5*soma;
	vetorClassificador[caracteristicas[0].size()-2] = soma;

	float referenciaMedia=0;
	for(int j =1;j<caracteristicas[0].size();j++)
		referenciaMedia +=(vetorClassificador[j]*vetorMedPos[j]);
	referenciaMedia +=vetorClassificador[caracteristicas[0].size()-2];

	float MaiorDistancia=-9999;
	float MenorDistancia=9999;

	float temp1 = -9999;
	float temp2 = -9999;

	float maiorRaio = 0;
		
	for(int i =0;i<numeroDeAmostras;i++)
	{
		soma = 0;

		std::string::size_type sz;     // alias of size_t
		int temp =caracteristicas[i].size()-1;
		mars = std::stof (caracteristicas[i][temp],&sz);//pega as etiquetas
		for(int j =1;j<numeroDeCaracterisicas;j++)
		{
			mars = std::stof (caracteristicas[i][j],&sz);//pega as etiquetas
			soma +=(mars*vetorClassificador[j-1]);
		}
		soma +=vetorClassificador[caracteristicas[0].size()-2];

		//temp2 =sqrt((referenciaMedia-soma)*(referenciaMedia-soma));

		float raoi  =referenciaMedia-soma;

		raoi = sqrt(raoi*raoi);


		if(raoi>maiorRaio)
			maiorRaio = soma;

		/*if(temp2>temp1)
		{
			temp1=temp2;
			MaiorDistancia = soma;
		}*/

		if(soma<MenorDistancia)
			MenorDistancia=soma;
		if(soma>MaiorDistancia)
			MaiorDistancia = soma;
	}

	vetorClassificador[caracteristicas[0].size()-1]=referenciaMedia-maiorRaio;
	vetorClassificador[caracteristicas[0].size()]=referenciaMedia+maiorRaio;
	ArquivoCSV csv;
	csv.ListarDistanciaMinima(vetorClassificador,tam); //valor do descritor -1
}


void VerificarMaiorDistancia()
{


}


ClassificadorDistanciaMinimaPositiva::ClassificadorDistanciaMinimaPositiva(void)
{
}

ClassificadorDistanciaMinimaPositiva::~ClassificadorDistanciaMinimaPositiva(void)
{
}
