#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/opencv.hpp"
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

using namespace std;
using namespace cv;

#pragma once
class ClassificadorDistanciaMinima
{

private:
	
	void split(const string& s, char c,vector<string>& v);

public:
	void GerarDescritorMedio(TipoDescritor descritor,int numero,float *vetor);
	void gerarDescritor();

	ClassificadorDistanciaMinima(void);
	~ClassificadorDistanciaMinima(void);
};




