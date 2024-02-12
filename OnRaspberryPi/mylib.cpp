#include "mylib.h"
#include "mylib.cuh"

//---------------------noirBlanc-----------------------

Mat noirBlanc(Mat frame)
{ 
	Mat im_gray_out;
	
	if (frame.empty())
	exit(0);
	
	cvtColor(frame,im_gray_out,COLOR_RGB2GRAY);	
	return im_gray_out;
}



//---------------------get_frame-----------------------

Mat get_frame(Mat frame)
{
	if (frame.empty())
	exit(0);
	
	return frame;
}

//---------------------seuillage------------------------

Mat seuillage(Mat frame)
{ 
	float nr;
	uchar r,v,b;
	Mat frame_out;
	frame_out.create(frame.rows,frame.cols,CV_8UC3);

	if (frame.empty())
	exit(0);
	
	for(int i = 0; i < frame.rows; i++)
	{
		for(int j = 0; j < frame.cols; j++)
		{
			
			b = frame.at<Vec3b>(i,j)[0];
			v = frame.at<Vec3b>(i,j)[1];
			r = frame.at<Vec3b>(i,j)[2];
			
			nr = r/sqrt(r*r+b*b+v*v);
			
			if (nr > 0.7){
				frame_out.at<Vec3b>(i,j)[0] = b;
				frame_out.at<Vec3b>(i,j)[1] = r;
				frame_out.at<Vec3b>(i,j)[2] = r;
			}
			else{
				frame_out.at<Vec3b>(i,j)[0] = b;
				frame_out.at<Vec3b>(i,j)[1] = v;
				frame_out.at<Vec3b>(i,j)[2] = r;
			}
		}
	}
	return frame_out;
}


//---------------------contour------------------------

Mat contour(Mat frame)
{ 
	
	Mat frame_out,frame_grayt;
	
	cvtColor(frame,frame_grayt,COLOR_RGB2GRAY);
	frame_out.create(frame.rows,frame.cols,CV_8UC1);
	
	if (frame.empty())
	exit(0);
	
	for (int i=1;i<frame.rows;i++){
		for (int j=1;j<frame.cols;j++){
			short temp;
			temp = (-1)*(short)frame_grayt.at<uchar>(i,j-1)+(-1)*(short)frame_grayt.at<uchar>(i-1,j)+(-1)*(char)frame_grayt.at<uchar>(i,j+1)+(-1)*(short)frame_grayt.at<uchar>(i+1,j)+4*(short)frame_grayt.at<uchar>(i,j);
			
			frame_out.at<uchar>(i,j)=(uchar)abs(temp);
		}
	}
	return frame_out;
}

//---------------------Mon filtre------------------------------

Mat mon_filtre(Mat frame){

	Mat frame_out;
	frame_out.create(frame.rows,frame.cols,CV_8UC3);

	if (frame.empty()) exit(0);

	for (int l = 1; l < frame.rows; l++)
	{
		for (int c = 1; c < frame.cols; c++)
		{
			frame_out.at<Vec3b>(frame.rows -l,  frame.cols -c )[0] = frame.at<Vec3b>(l, c)[0];
			frame_out.at<Vec3b>(frame.rows -l,  frame.cols -c )[1] = frame.at<Vec3b>(l, c)[1];
			frame_out.at<Vec3b>(frame.rows-l, frame.cols- c )[2] = frame.at<Vec3b>(l, c)[2];
		}
	}
	return frame_out;
}

Mat mon_filtre2(Mat frame){

	Mat frame_out;
	frame_out.create(frame.rows,frame.cols,CV_8UC3);

	if (frame.empty()) exit(0);

	for (int l = 0; l < frame.rows; l++)
	{
		for (int c = 1; c < frame.cols; c++)
		{
			frame_out.at<Vec3b>(l,  frame.cols - c )[0] = frame.at<Vec3b>(l, c)[0];
			frame_out.at<Vec3b>(l, frame.cols - c )[1] = frame.at<Vec3b>(l, c)[1];
			frame_out.at<Vec3b>(l, frame.cols - c )[2] = frame.at<Vec3b>(l, c)[2];
		}
	}
	return frame_out;
}

Mat mon_filtre3(Mat frame){

	Mat frame_out;
	frame_out.create(frame.rows,frame.cols,CV_8UC3);

	if (frame.empty()) exit(0);

	for (int l = 1; l < frame.rows; l++)
	{
		for (int c = 1; c < frame.cols; c++)
		{
			frame_out.at<Vec3b>(l,  c) = frame.at<Vec3b>(l + 1, c + 1) - frame.at<Vec3b>(l - 1, c - 1) ;
		}
	}
	return frame_out;
}


Mat Filtre_generique(Mat frame, Mat filtreConvolution, int lignes, int colonnes, bool noir_blanc){
	Mat frame_out, frame_grayt;

	int rayon_rows = (frame.rows - 1) / 2;
	int rayon_cols = (frame.cols - 1) / 2;
	//image noir_et_blanc
	if(noir_blanc){
		cvtColor(frame,frame_grayt,COLOR_RGB2GRAY);
		frame_out.create(frame_grayt.rows, frame_grayt.cols,CV_8UC1);
	}
	//image couleur
	else frame_out.create(frame.rows, frame.cols, CV_8UC3);

	for(int i= 1; i< frame.rows ;i++){// - rayon_rows; i++){
		for(int j = 1; j < frame.cols; j++){// - rayon_cols; j++){
			
			for(int l = 1; l < lignes; l++){
				for(int c = 1; c < colonnes; c++){
					if(noir_blanc){ //image en noir et blanc
						frame_out.at<uchar>(i, j) += frame_grayt.at<uchar>(i, j) *filtreConvolution.at<float>(l, c); 
					}
					else{
						frame_out.at<Vec3b>(i, j)[0] += frame.at<Vec3b>(i, j)[0] *filtreConvolution.at<float>(l, c);
						frame_out.at<Vec3b>(i, j)[1] += frame.at<Vec3b>(i, j)[1] *filtreConvolution.at<float>(l, c);
						frame_out.at<Vec3b>(i, j)[2] += frame.at<Vec3b>(i, j)[2] *filtreConvolution.at<float>(l, c);
					}
				}
			}
		}
	}

	return frame_out;
	
}

Mat Filtre_sobel(Mat frame,bool noir_blanc){
	
	Mat frame_out, frame_grayt, Gx, Gy, filtre_sobel_x, filtre_sobel_y;

	filtre_sobel_x.create(3, 3, CV_32F); //taille 32 bits
	filtre_sobel_y.create(3, 3, CV_32F); //taille 32 bits

	int convolution_y[3][3] = {
		{1, 2, 1},
		{0, 0, 0},
		{-1,-2,-1},
	};

	int convolution_x[3][3] = {
		{-1,0,1},
		{-2,0,2},
		{-1,0,1},
	};

	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){

			filtre_sobel_x.at<float>(i, j) = convolution_x[i][j];
			filtre_sobel_y.at<float>(i, j) = convolution_y[i][j];
		}
	}

	int rayon_rows = (frame.rows - 1) / 2;
	int rayon_cols = (frame.cols - 1) / 2;
	//image noir_et_blanc
	if(noir_blanc){
		cvtColor(frame,frame_grayt,COLOR_RGB2GRAY);
		frame_out.create(frame_grayt.rows, frame_grayt.cols,CV_8UC1);
		Gx.create(frame_grayt.rows, frame_grayt.cols,CV_8UC1);
		Gy.create(frame_grayt.rows, frame_grayt.cols,CV_8UC1);
	}
	//image couleur
	else{
		frame_out.create(frame.rows, frame.cols, CV_8UC3);
		Gx.create(frame.rows, frame.cols, CV_8UC3);
		Gy.create(frame.rows, frame.cols, CV_8UC3);
	}

	for(int i= 1; i< frame.rows ;i++){// - rayon_rows; i++){
		for(int j = 1; j < frame.cols; j++){// - rayon_cols; j++){
			
			for(int l = 1; l < 3; l++){ //3 = nombres de lignes du tab de convolution
				for(int c = 1; c < 3; c++){ //nbr colonnes
					if(noir_blanc){ //image en noir et blanc
						Gx.at<uchar>(l, c) = frame_grayt.at<uchar>(i,j)*filtre_sobel_x.at<float>(l, c); 
						Gy.at<uchar>(l, c) = frame_grayt.at<uchar>(i,j)*filtre_sobel_y.at<float>(l, c); 
					//	frame_out.at<uchar>(i, j) += frame_grayt.at<uchar>(i , j) *filtreConvolution_x.at<float>(l-1, c-1); 
						frame_out.at<uchar>(i, j) += sqrt(Gx.at<uchar>(l, c) * Gx.at<uchar>(l, c) + Gy.at<uchar>(l, c) * Gy.at<uchar>(l, c));
					}
					else{
						for(int RGB = 0; RGB <3; RGB++){
							Gx.at<Vec3b>(l,c)[RGB] = frame.at<Vec3b>(i,j)[RGB]*filtre_sobel_x.at<float>(l, c);
							Gy.at<Vec3b>(l,c)[RGB] = frame.at<Vec3b>(i,j)[RGB] *filtre_sobel_y.at<float>(l, c);
						}

						for(int RGB = 0; RGB < 3; RGB++) frame_out.at<Vec3b>(i,j)[RGB] += sqrt(Gx.at<Vec3b>(l,c)[RGB]*Gx.at<Vec3b>(l,c)[RGB] + Gy.at<Vec3b>(l,c)[RGB]*Gy.at<Vec3b>(l,c)[RGB]);

					}
				}
			}
		}
	}

	return frame_out;

	
}

Mat rempli_mon_filtre(Mat monFiltre, int lignes, int colonnes, bool &remplissage_fini){

	std::cout<<std::endl<<"Remplissez votre tableau (chaque chiffre doit etre tape un a un, du haut gauche du tableau au bas droit): \n ";
	monFiltre.create(lignes, colonnes, CV_32F); //taille 32 bits

	// on rempli la matrice de 0 pour gérer l'affichage du filtre
	for(int i=0; i< lignes; i++){
		for(int j=0; j<colonnes; j++){
			monFiltre.at<float>(i, j) = 0;
		}
	}

	for(int i=0; i< lignes; i++){
		for(int j=0; j<colonnes; j++){
			std::cin>>monFiltre.at<float>(i, j);

			//Pour afficher le tableau à l'utilisateur
			
			for(int x=0; x< lignes; x++){
				for(int y=0; y<colonnes; y++){
					std::cout<<monFiltre.at<float>(x, y)<<" | ";
				}
				std::cout<<std::endl;
			}
		}

	}

	remplissage_fini = true;
	return monFiltre;
}

Mat My_filtre2D(Mat frame, Mat filter2D){

	int rayon_rows, rayon_cols;
	rayon_rows = (frame.rows - 1) / 2;
	rayon_cols = (frame.cols - 1) / 2;

	Mat frame_out;
	frame_out.create(frame.rows,frame.cols,CV_8UC3);

	for(int i = 0; i < frame.rows; i++){
		for(int j = 0; j < frame.cols; j++){

			for (int l = 1; l < frame.rows; l++){
				for (int c = 1; c < frame.cols; c++){

				frame_out.at<Vec3b>(i,  j) += filter2D.at<Vec3b>(-rayon_rows, -rayon_cols);// * frame.at<Vec3b>(i, j) ;
			}
		}
	}
	/*

	if(filter2D == mon_filtre2){
		
		for (int l = 0; l < frame.rows; l++){
			for (int c = 1; c < frame.cols; c++){
				frame_out.at<Vec3b>(l,  frame.cols - c )[0] = frame.at<Vec3b>(l, c)[0];
				frame_out.at<Vec3b>(l, frame.cols - c )[1] = frame.at<Vec3b>(l, c)[1];
				frame_out.at<Vec3b>(l, frame.cols - c )[2] = frame.at<Vec3b>(l, c)[2];
			}
		}
	}

	if(filter2D == mon_filtre){

		for (int l = 1; l < frame.rows; l++){
			for (int c = 1; c < frame.cols; c++){
				frame_out.at<Vec3b>(frame.rows -l,  frame.cols -c )[0] = frame.at<Vec3b>(l, c)[0];
				frame_out.at<Vec3b>(frame.rows -l,  frame.cols -c )[1] = frame.at<Vec3b>(l, c)[1];
				frame_out.at<Vec3b>(frame.rows-l, frame.cols- c )[2] = frame.at<Vec3b>(l, c)[2];
			}
		}*/

	}

	return frame_out;
}
