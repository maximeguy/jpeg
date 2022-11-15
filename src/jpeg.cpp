//============================================================================
// Name        : jpeg.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "DCTCompression.hpp"

using namespace std;

int main() {
	const unsigned w = 8;
	const unsigned h = 8;
//	int **block = new int*[h];
//	for(unsigned i = 0; i<h; i++){
//		block[i] = new int[w];
//	}
	int block[h][w]={
		{139, 144, 149, 153, 155, 155, 155, 155},
		{144, 151, 153, 156, 159, 156, 156, 156},
		{150, 155, 160, 163, 158, 156, 156, 156},
		{159, 161, 162, 160, 160, 159, 159, 159},
		{159, 160, 161, 162, 162, 155, 155, 155},
		{161, 161, 161, 161, 160, 157, 157, 157},
		{162, 162, 161, 163, 162, 157, 157, 157},
		{162, 162, 161, 161, 163, 158, 158, 158}
	};


	double ** img_dct = new double*[h];
	int ** img_idct = new int*[h];
	int ** img_quant = new int*[h];
	double ** img_dequant = new double*[h];
	//double ** img_reverse = new double*[h];
	for(unsigned i = 0; i<h; i++) {
		img_dct[i] = new double[w];
		img_idct[i] = new int[w];
		img_quant[i] = new int[w];
		img_dequant[i] = new double[w];
	}

	DCTCompression * dct = new DCTCompression(w,h,50);

	dct->DCT_Block(img_dct, block);
	dct->IDCT_Block(img_dct , img_idct);
	dct->quantification(img_dct, img_quant);
	dct->dequantification(img_quant, img_dequant);
	//dct->IDCT_Block(img_quant_idct, img_quant);
	dct->EQM(img_quant);
	//cout<<"\n EQM = "<<dct->EQM(img_quant)<<endl;

	return 0;
}
