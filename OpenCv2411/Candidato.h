#pragma once

#include<math.h>
#include <direct.h>
#include "Enumeradores.h"
#include <windows.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <sstream>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "Enumeradores.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "DescritorLBP.h"
#include "ArquivoCSV.h"
#include "DescritorHOG.h"
#include "ClassificadorRegressaoLogistica.h"
#include "Enumeradores.h"

using namespace cv;
using namespace std;


class Candidato
{
private:
	int tamX;
	int tamY;

	void MarcarImagemLBP(Mat& frame,int x, int y,int passos);
	void split(const string& s, char c,vector<string>& v);

public:

	void VarrerFrame(Mat& frame,TipoDescritor Tipo,int passos,float limiar1,float limiar2,TipoClassificador classificador = TipoClassificador::regressaoLogistica);

	void VarrerFrameVariasClasses(Mat& frame,TipoDescritor Tipo,int passos,float limiar1,float limiar2,int numeroVistorias,TipoClassificador classificador = TipoClassificador::regressaoLogistica);

	int Histograma(Mat& frame,TipoDescritor Tipo,int etiqueta,float *vetor,boolean salvar = true);
	float DetectarRegressao(Mat& frame,TipoDescritor Tipo);
	int DetectarRegressao(Mat& frame,TipoDescritor Tipo,float limiar1,float limiar2);

	float DetectarDistanciaMinima(Mat& frame,TipoDescritor Tipo);
	bool DetectarDistanciaMinimaPositiva(Mat& frame,TipoDescritor Tipo);

	Candidato(int TamX,int TamY);
	Candidato(void);
	~Candidato(void);
};

