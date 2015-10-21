#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/opencv.hpp"
#include <math.h>
#include "Candidato.h"

using namespace cv;
using namespace std;

#pragma once
class DescritorHAAR
{
public:
	void gerarDescritor(Mat& frame,Mat& HAAR);

	DescritorHAAR(void);
	~DescritorHAAR(void);
};

