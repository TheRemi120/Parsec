#include "mylib.h"
#include "opencv2/opencv.hpp"
//#include "opencv2/highgui.hpp"
#include <iostream>

Mat3b moncanvas;
std::string winName = "Table des filtres";

Rect button, button2, button3, button4, button5;
Mat frame;

typedef enum {choix_camera, choix_sobel, choix_generique, choix_inverse_horizontal, choix_inverse_vertical} choix_filtre;
choix_filtre choix_user = choix_camera;
bool initialisation=false;

Mat filtre_convolution_generique,filtre_sobel_generique_x, filtre_sobel_generique_y;

//Pour tableau de convolution generique
int lignes, colonnes;

//Pour que l'utilisateur choisisse entre une image monochrome ou polychrome
bool noir_blanc = false;

//Pour permettre d'avoir un flux video et non une image seule lors du choix de la fonction de filtrage
bool laisser_passer_mon_filtre_sobel = false, laisser_passer_mon_filtre_inverseur_horizontal = false, laisser_passer_mon_filtre = false, laisser_passer_mon_filtre_inverseur_vertical = false, laisser_passer_mon_filtre_contour=false;

//Lorsque l'on clique sur un boutton
void callBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        if (button.contains(Point(x, y)))
        {
			//Filtre generique
            rectangle(moncanvas, button, Scalar(0,0,255), 1);		
			choix_user = choix_generique;

			laisser_passer_mon_filtre_inverseur_horizontal = false;
			laisser_passer_mon_filtre_sobel=false;
			laisser_passer_mon_filtre_inverseur_vertical = false;
			laisser_passer_mon_filtre_contour = false;
        }

		else if(button2.contains(Point(x, y))){
			//Filtre de Sobel
            rectangle(moncanvas, button2, Scalar(0,0,255), 1);
			choix_user = choix_sobel;

			laisser_passer_mon_filtre = false;
			laisser_passer_mon_filtre_inverseur_horizontal = false;
			laisser_passer_mon_filtre_inverseur_vertical = false;
			laisser_passer_mon_filtre_contour = false;
		}

		else if(button3.contains(Point(x, y))){
			//Filtre de Sobel
            rectangle(moncanvas, button3, Scalar(0,0,255), 1);
			choix_user = choix_inverse_horizontal;

			laisser_passer_mon_filtre = false;
			laisser_passer_mon_filtre_sobel = false;
			laisser_passer_mon_filtre_inverseur_vertical = false;
			laisser_passer_mon_filtre_contour = false;
		}
		else if(button4.contains(Point(x, y))){
			//Filtre de Sobel
            rectangle(moncanvas, button3, Scalar(0,0,255), 1);
			choix_user = choix_inverse_horizontal;

			laisser_passer_mon_filtre = false;
			laisser_passer_mon_filtre_sobel = false;
			laisser_passer_mon_filtre_contour = false;
			laisser_passer_mon_filtre_inverseur_horizontal = false;
		}
		else if(button5.contains(Point(x, y))){
			//Filtre de Sobel
            rectangle(moncanvas, button3, Scalar(0,0,255), 1);
			choix_user = choix_inverse_horizontal;

			laisser_passer_mon_filtre = false;
			laisser_passer_mon_filtre_sobel = false;
			laisser_passer_mon_filtre_inverseur_vertical = false;
			laisser_passer_mon_filtre_inverseur_horizontal = false;
		}

		switch (choix_user){
				case choix_camera:
					destroyWindow("Camera");
					break;
				case choix_sobel:
					destroyWindow("Filtre de Sobel");
					break;
				case choix_generique:
					destroyWindow("Convolution generique");
					break;
				case choix_inverse_horizontal:
					destroyWindow("Inverseur image horizontalement");
					break;
			};
			
		initialisation = true; //on change de filtre
    }
    if (event == EVENT_LBUTTONUP)
    {
		   if (button.contains(Point(x, y)))
        {
            rectangle(moncanvas, button, Scalar(200, 200, 200), 1);
        }
		else if(button2.contains(Point(x, y))){
            rectangle(moncanvas, button2, Scalar(200, 200, 200), 1);
		}
		else if(button3.contains(Point(x,y))){
			rectangle(moncanvas, button3, Scalar(200, 200, 200), 1);
		}
		else if(button4.contains(Point(x,y))){
			rectangle(moncanvas, button4, Scalar(200, 200, 200), 1);
		}
		else if(button5.contains(Point(x,y))){
			rectangle(moncanvas, button5, Scalar(200, 200, 200), 1);
		}
    }


    imshow(winName, moncanvas);
  //  waitKey(1);
}

int main(int, char**)
{ 
	VideoCapture cap(0); // open the default camera
	if(!cap.isOpened())  // check if we succeeded
		return -1;
	 //L' image de fond
    Mat3b img(300, 300, Vec3b(0, 255, 0));

    //Les buttons
    button = Rect(0,0,img.cols, 50);
	button2 = Rect(0,50, img.cols, 50);
	button3 = Rect(0, 100, img.cols, 50);
	button4 = Rect(0, 150, img.cols, 50);
	button5 = Rect(0, 200, img.cols, 50);

    //zone de dessin
	moncanvas = Mat3b(img.rows + 5*button.height, img.cols, Vec3b(0,255,0));

	//On creer le rectangle pour les bouttons
	rectangle(moncanvas, button, Scalar(125, 50,35), FILLED);
	rectangle(moncanvas, button2, Scalar(100,15,35), FILLED);
	rectangle(moncanvas, button3, Scalar(85,15,35), FILLED);
	rectangle(moncanvas, button4, Scalar(65,15,35), FILLED);
	rectangle(moncanvas, button5, Scalar(50,15,35), FILLED);
	
    // texte des bouttons
    putText(moncanvas, "Filtre generique", Point(button.width*0.15, button.height*0.7), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0,255,25), 1);
	putText(moncanvas, "Filtre de Sobel", Point(button.width*0.15, 2.5*button.height*0.7), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0,255,25), 1);
	putText(moncanvas, "Inverseur horizontal", Point(button.width*0.01, 5*button.height*0.7), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0,255,25), 1);
	putText(moncanvas, "Inverseur vertical", Point(button.width*0.01, 7.5*button.height*0.7), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0,255,25), 1);
	putText(moncanvas, "Detecteur de contour", Point(button.width*0.01, 10*button.height*0.7), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0,255,25), 1);

    // Setup callback function
    namedWindow(winName);
    setMouseCallback(winName, callBackFunc);

	while(true){

		Mat originalframe;
		cap >> originalframe;
		char c=(char)waitKey(25);

		resize(originalframe, frame, Size(700, 700));
		
		/*

		if(laisser_passer_mon_filtre){
			Mat mon_filtre;
			Mat inverseur_image = My_filtre2D(frame, mon_filtre);
			imshow("mon Filtre", inverseur_image);
		}
		if(laisser_passer_mon_filtre2){
			Mat inverseur_image2 = My_filtre2D(frame, mon_filtre2);
			imshow("mon Filtre2", inverseur_image2);
		}
		if(laisser_passer_mon_filtre3){
			Mat inverseur_image3 = My_filtre2D(frame, mon_filtre3);
			imshow("mon Filtre3", inverseur_image3);
		}*/

		if(c == '1'){				// if '1' est appuye
			Mat NB = noirBlanc(frame);
	    		imshow("NoirEtBlanc", NB);
		}
		else if(c == '2'){			// if '2' est appuye
			Mat seuil = seuillage(frame);
	    		imshow("seuillage", seuil);
		}
		else if (c == '3'){			// if '3' est appuye
			Mat cont = contour(frame);
	    		imshow("contour", cont);
		}
		else if (c == '4'){			// if '4' est appuye
			Mat seuilgpu = seuillage(frame);
	    		imshow("seuillage ",seuilgpu);
		}

		else if(c=='5' || (choix_user == choix_inverse_horizontal && initialisation == true)){
			std::string choix_couleur;

			std::cout<<"\nSi vous souhaitez avoir une image en noir et blanc entrez 'monochrome' sinon entrez 'polychrome' :";
			std::cin>>choix_couleur;

			if(choix_couleur == "monochrome") noir_blanc = true;
			else noir_blanc = false;

			initialisation = false;
			laisser_passer_mon_filtre_inverseur_horizontal = true;
		}

		else if(c == '8'  || (choix_user == choix_generique && initialisation == true)){
		
			std::string choix_couleur;

			std::cout<<"\nSi vous souhaitez avoir une image en noir et blanc entrez 'monochrome' sinon entrez 'polychrome' :";
			std::cin>>choix_couleur;

			if(choix_couleur == "monochrome") noir_blanc = true;
			else noir_blanc = false;

			std::cout<<"\nVeuillez entrer le nombre de lignes de votre filtre de convolution : ";
			std::cin>>lignes;

			std::cout<<"\nVeuillez entrer le nombre de colonnes de votre filtre de convolution : ";
			std::cin>>colonnes;

			filtre_convolution_generique.create(lignes, colonnes, CV_32F); //taille 32 bits

			std::cout<<std::endl<<"Remplissez votre tableau (chaque chiffre doit etre tape un a un, du haut gauche du tableau au bas droit): \n ";
			
			bool remplissagefiltre = false;// pour remplir la fonction rempli_mon_filtre
			filtre_convolution_generique = rempli_mon_filtre(filtre_convolution_generique, lignes, colonnes, remplissagefiltre);

			initialisation = false;
			laisser_passer_mon_filtre = true;
		}
		else if(c == '9' || (choix_user == choix_sobel && initialisation == true)){

			
			std::string choix_couleur;

			std::cout<<"\nSi vous souhaitez avoir une image en noir et blanc entrez 'monochrome' sinon entrez 'polychrome' :";
			std::cin>>choix_couleur;

			if(choix_couleur == "monochrome") noir_blanc = true;
			else noir_blanc = false;

			initialisation = false;
			laisser_passer_mon_filtre_sobel = true;

		}
		else if(c == '0') destroyAllWindows();	// if '0' est appuye

		else{
			if(laisser_passer_mon_filtre){
				Mat frame_generique = Filtre_generique(frame, filtre_convolution_generique, lignes, colonnes, noir_blanc);
				imshow("Convolution generique",frame_generique);
				moveWindow("Convolution generique", 400,30);
			}
			else if(laisser_passer_mon_filtre_sobel){
				Mat frame_sobel= Filtre_sobel(frame, noir_blanc);
				imshow("Filtre de Sobel",frame_sobel);
				moveWindow("Filtre de Sobel", 400,30);
			}
			else if(laisser_passer_mon_filtre_inverseur_horizontal){
			Mat inverseur_image_horizontal = mon_filtre2(frame);
			imshow("Inverseur image horizontalement", inverseur_image_horizontal);
			moveWindow("Inverseur image horizontalement", 400,30);
			}
			else if(laisser_passer_mon_filtre_inverseur_vertical){
			Mat inverseur_image_vertical = mon_filtre(frame);
			imshow("Inverseur image verticalement", inverseur_image_vertical);
			moveWindow("Inverseur image verticalement", 400,30);
			}
			else if(laisser_passer_mon_filtre_contour){
			Mat detection_contour = mon_filtre3(frame);
			imshow("Detection de contour", detection_contour);
			moveWindow("Detection de contour", 400,30);
			}
			else{
				imshow("Camera", frame);
				moveWindow("Camera", 400,30);
			} 
		}
			
	    	if(c==27)// if 'esc' est appuye
	      		break;

			 //Dessine le tableau des filtres
			img.copyTo(moncanvas(Rect(0, 5*button.height, img.cols, img.rows)));

			imshow(winName, moncanvas);
	}
  	

	

// When everything done, release the video capture object
	cap.release();

	// Closes all the frames
	destroyAllWindows();

	return 0;
}