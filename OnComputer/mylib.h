#ifndef MYLIB_H
#define MYLIB_H

#define SIZE_I 960
#define SIZE_J 1280
#define BLOCK_SIZE 16

#include "opencv2/opencv.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
//#include <cuda_runtime.h>

using namespace std;

using namespace cv;

//bool initialisation_filtre_generique = true;
//int lignes, colonnes;

//---------------------get_frame---------------------

Mat get_frame(Mat frame);

//---------------------noirBlanc------------------

Mat noirBlanc(Mat frame);

//---------------------seuillage------------------

Mat seuillage(Mat frame);
Mat seuillageGPU( Mat in);

//---------------------contour--------------------

Mat contour(Mat frame);

//------------------Mon filtre-----------------
Mat mon_filtre(Mat frame);

Mat mon_filtre2(Mat frame);
Mat mon_filtre3(Mat frame);

Mat Filtre_generique(Mat frame, Mat filtreConvolution,int lignes, int colonnes, bool noir_blanc);

Mat rempli_mon_filtre(Mat monFiltre, int lignes, int colonnes, bool &remplissage_fini);
Mat Filtre_sobel(Mat frame, bool noir_blanc);
//Mat Filtre_sobel_generique(Mat frame, Mat filtreConvolution_x, int lignes, int colonnes, bool noir_blanc);
Mat My_filtre2D(Mat frame, Mat filter2D);


#endif