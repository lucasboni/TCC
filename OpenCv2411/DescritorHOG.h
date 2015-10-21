#pragma once

#define PI 3.14159265

#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/opencv.hpp"
#include <math.h>
#include "Candidato.h"

using namespace cv;
using namespace std;

enum HOGmodo { HOGnormal, HOGnormalizado,HOGintegral};



struct DadoHOG {
	int x;
	int y;
	int magnitude;
	int graus;
};

/*
*	1   2
*	3   4
*/
struct BlocoHOG{
	DadoHOG** bloco1;
	DadoHOG** bloco2;
	DadoHOG** bloco3;
	DadoHOG** bloco4;
};


struct BlocoHOGintegtal{
	float intervalo[9];
};

struct SubBlocoHOG{
	DadoHOG** bloco;
};


class DescritorHOG
{
private:
	void normalizar(float vetor[]);
	void gerarDescritor(Mat& frame,SubBlocoHOG& blocosHOG);
	void gerarDescritorIntegral(Mat& frame,BlocoHOGintegtal** descritor);

public:
	int histograma(Mat& edges,HOGmodo modo,float *vetor,int etiqueta,boolean salvar=true);

	DescritorHOG(void);
	~DescritorHOG(void);
};

