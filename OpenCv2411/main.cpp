#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/opencv.hpp"

#include "DescritorLBP.h"
#include "DescritorHOG.h"
#include "Candidato.h"

#include "ClassificadorRegressaoLogistica.h"

#include "ClassificadorDistanciaMinima.h"

#include "ClassificadorDistanciaMinimaPositiva.h"

#include <windows.h>

#include "Enumeradores.h"

using namespace cv;
using namespace std;

int main()
{
	
	
	float *vetor;
	vetor = new float[256];
	Mat frame;

		int inicio, final, tmili; 
		 
		//Sleep(1000);
		

        //cap >> frame; // get a new frame from camera
		
		inicio = GetTickCount(); 

		//ClassificadorDistanciaMinima a;
		//a.GerarDescritorMedio(TipoDescritor::LBPUNIFORMEdesc,1,vetor);

		Candidato candidato(100,50);
		//ClassificadorRegressaoLogistica a;
		//a.gerarClassificador(TipoDescritor::HOGdesc,15,vetor);

		//ClassificadorDistanciaMinimaPositiva a;
		//a.GerarDescritorMedio(TipoDescritor::LBPUNIFORMEdesc,1,vetor);
		//if(true)
		//	return 0;
		cout <<"----"<<endl;
		
		for(int i =1;i<=50;i++)
		{
		
		string Result;          // string which will contain the result
		ostringstream convert;   // stream used for the conversion
		convert << i;      // insert the textual representation of 'Number' in the characters in the stream
		Result = convert.str();
			string diretorio = "C:\\Users\\Lucas\\Documents\\Visual Studio 2010\\Projects\\OpenCv2411\\Debug\\";
			diretorio =diretorio+(Result+".jpg");
		frame = imread(diretorio);
		candidato.VarrerFrameVariasClasses(frame,TipoDescritor::HOGdesc,10,0.75,1,3,TipoClassificador::regressaoLogistica);
		}
		
		for(int i =1;i<=50;i++)
		{
		
		string Result;          // string which will contain the result
		ostringstream convert;   // stream used for the conversion
		convert << i;      // insert the textual representation of 'Number' in the characters in the stream
		Result = convert.str();
			string diretorio = "C:\\Users\\Lucas\\Documents\\Visual Studio 2010\\Projects\\OpenCv2411\\Debug\\";
			diretorio =diretorio+(Result+".jpg");
		frame = imread(diretorio);
		candidato.VarrerFrameVariasClasses(frame,TipoDescritor::HOGdesc,15,0.7,1,2,TipoClassificador::regressaoLogistica);
		}

		final = GetTickCount();
		tmili = final - inicio;
		printf("tempo decorrido: %d\n", tmili); 

	system ("PAUSE");
    return 0;
}