//============================================================================
// Name        : jpeg.cpp
// Author      : Maxime GUY
// Description : JPEG compression program
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include "Compression.hpp"
#include "DCTCompression.hpp"
#include "Huffman.hpp"

using namespace std;

int main() {
	const unsigned w = 8;
	const unsigned h = 8;
	const unsigned block_sz = 8;
	const unsigned img_sz = 128;

	int block[block_sz][block_sz]={
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


	Compression * comp = new Compression("data/lenna.dat");

	// Test DCTCompression & Huffman with examples
//	DCTCompression * dct = new DCTCompression(img_sz,img_sz,50);
//
//	dct->DCT_Block(img_dct, block);
//	dct->IDCT_Block(img_dct , img_idct);
//	dct->quantification(img_dct, img_quant);
//	dct->dequantification(img_quant, img_dequant);
//	cout<<"EQM = "<<dct->EQM(img_dequant,block)<<endl;
//	cout<<"Compression rate = "<<dct->compression_rate(img_dequant,block)<<endl;
//	//cout<<"Non null elements = "<< dct->non_null_elements<int>(img_quant)*2+1<<endl;
//	//int * block_frame = new int[(dct->non_null_elements<int>(img_quant)*2) +1];
//	int * block_frame = new int[64];
//	cout<<"Test block frame : ";
//	dct->RLE_Block(img_quant, 0, block_frame);

	// Change type in class declaration to test Huffman on the test set
//	char huff_data[6] = {'a','b','c','d','e','f'};
//	double huff_freq[6] = {5,3,7,1,10,2};
//	Huffman * huff = new Huffman(huff_data, huff_freq, sizeof(huff_freq) / sizeof(huff_freq[0]));
//	huff->print_codes(huff->pq.top(), "");
	return 0;
}
