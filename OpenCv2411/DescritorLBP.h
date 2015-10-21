#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdlib.h>
#include "opencv2/opencv.hpp"
#include "Candidato.h"

using namespace cv;

enum LBPmodo { LBPnormal, LBPuniforme};

class DescritorLBP
{
private:

public:
	void histogramaOtimizado(Mat& frame, LBPmodo modo,int etiqueta,float *vetor,boolean salvar = true);

	void gerarDescritor(Mat& frame,Mat& LBP, LBPmodo modo,int etiqueta);

	void DetectarObjto(Mat& frame,int intervalos[][2], int quantidadeIntervalos, LBPmodo modo,int etiqueta);

	void histograma(Mat& frame, LBPmodo modo,int etiqueta,float *vetor,boolean salvar = true);

	//histogrma usado para deteccao de objetos pois ele já recebe uma figura convertida
	void histogramaDetecao(Mat& frame, LBPmodo modo,int etiqueta,float *vetor);

	DescritorLBP(void);
	~DescritorLBP(void);
};

