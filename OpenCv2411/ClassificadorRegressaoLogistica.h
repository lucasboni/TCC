#include<math.h>
#include <direct.h>
#include "Enumeradores.h"
#include <windows.h>
#include <string>

#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <sstream>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;



class ClassificadorRegressaoLogistica
{
	//classificador = transpostata do modelo x transpostata do descritor = 0
	float funcaoCusto(float caracteristicas[],float etiquetas[],float w,int quantidade);
	float funcaoOtimizacao(float caracteristicas[],float etiquetas[],float tolerancia,int quantidade);
	void split(const string& s, char c,vector<string>& v);

public:
	float funcaoLogistica(float valor);
	int gerarClassificador(TipoDescritor descritor,int numero,float *vetor);
	ClassificadorRegressaoLogistica(void);
	~ClassificadorRegressaoLogistica(void);
};

