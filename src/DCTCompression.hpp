/*
 * DCTCompression.hpp
 *
 *  Created on: 3 nov. 2022
 *      Author: maxime
 */

#ifndef DCTCOMPRESSION_HPP_
#define DCTCOMPRESSION_HPP_

#include <cmath>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>

using namespace std;

/**
 * Implements DCT Compression algorithm
 */

class DCTCompression{
	unsigned m_width;			/**< Image Width */
	unsigned m_height;			/**< Image Height */
	unsigned char **m_buffer;
	unsigned m_quality;		/**< JPEG Quality */
	unsigned m_Q[8][8] = {
			{16,11,10,16,24,40,51,61},
			{12,12,14,19,26,58,60,55},
			{14,13,16,24,40,57,69,56},
			{14,17,22,29,51,87,80,62},
			{18,22,37,56,68,109,103,77},
			{24,35,55,64,81,104,113,92},
			{49,64,78,87,103,121,120,101},
			{72,92,95,98,112,100,103,99},
	};

public:
	static const unsigned block_sz = 8;		/**< DCT Block size */

    /**
     * Class constructor
     * Assign m_width, m_height and m_quality variable and allocates m_buffer as a bidimensional array
     */
	DCTCompression(const unsigned width,const unsigned height,const unsigned quality = 50){
		m_width = width;
		m_height = height;
		m_quality = quality;
		m_buffer = new unsigned char*[height];
		for (unsigned i = 0; i < height; i++){
			m_buffer[i]= new unsigned char[width];
		}
	}

	/**
	 * Class destructor
	 * Deallocate m_buffer
	 */
	~DCTCompression(){

	}

	/**
	 * Width setter
	 */
	void SetWidth(unsigned width){
		m_width = width;
	}

	/**
	 * Height setter
	 */
	void SetHeight(unsigned height){
		m_height = height;
	}

	/**
	 * Quality setter
	 */
	void SetQuality(unsigned quality){
		m_quality = quality;
	}

	/**
	 * Width getter
	 */
	unsigned GetWidth(){
		return m_width;
	}

	/**
	 * Height getter
	 */
	unsigned GetHeight(){
		return m_height;
	}

	/**
	 * Quality getter
	 */
	unsigned GetQuality(){
		return m_quality;
	}

	/**
	 * Compute discrete cosinus transform (DCT) on a bidimensional array
	 * @param DCT_img : Pointer to the output image
	 * @param block : Pointer to 8x8 block of pixels from the input image
	 */
	void DCT_Block(double**DCT_img, int block[][block_sz]){

		cout<<"\n DCT in : \n"<<endl;
		for (unsigned x = 0; x < block_sz; x++){
			for (unsigned y = 0; y < block_sz; y++){
				printf("  %-4d | ",block[x][y]);
			}
			cout<<"\n";
		}
		double cu, cv, p;
		p=0;
		for (unsigned u = 0; u < block_sz; u++){
			u == 0 ? cu=1/sqrt(2) : cu=1;
			for (unsigned v = 0; v < block_sz; v++){
				v == 0 ? cv=1/sqrt(2) : cv=1;
				for (unsigned x = 0; x < block_sz; x++){
					for (unsigned y = 0; y < block_sz; y++){

						p += (block[x][y]-128)*
								cos((2*x+1)*M_PI*u/16.)*
								cos((2*y+1)*M_PI*v/16.);

					}
				}
				DCT_img[u][v]= cv*cu*p/4.;
				p=0;
			}
		}

		cout<<"\n DCT out : \n"<<endl;
		show<double>(DCT_img);

	}

	/**
	 * Compute inverse discrete cosinus transform (DCT) on a bidimensional array
	 * @param DCT_img : Pointer to the output image
	 * @param block : Pointer to 8x8 block of pixels from the input image
	 */
	void IDCT_Block(double**DCT_img, int**block){

		double cu, cv, p;
		p=0;
		for (unsigned x = 0; x < block_sz; x++){

			for (unsigned y = 0; y < block_sz; y++){

				for (unsigned u = 0; u < block_sz; u++){
					u == 0 ? cu=1/sqrt(2) : cu=1;

					for (unsigned v = 0; v < block_sz; v++){
						v == 0 ? cv=1/sqrt(2) : cv=1;

						p += (DCT_img[u][v])*cv*cu*
								cos(((2*x+1)*M_PI*u)/16.)*
								cos(((2*y+1)*M_PI*v)/16.);

					}
				}
				block[x][y]= p/4. + 128;
				p=0;
			}
		}

		cout<<"\n Inverse DCT out : \n"<<endl;
		show<int>(block);
	}

	int** get_quantification_table(){
		int** Q_tab = new int*[block_sz];

		double lambda;
		if (m_quality < 50) lambda = 5000/m_quality;
		else lambda = 200 - 2 * m_quality;

		for(unsigned i = 0; i<block_sz; i++) {
			Q_tab[i] = new int[block_sz];
			for (unsigned j = 0; j < block_sz; j++){
				if (floor((m_Q[i][j]*lambda+50)/100) < 1) Q_tab[i][j] = 1;
				else if(floor((m_Q[i][j]*lambda+50)/100) > 255) Q_tab[i][j] = 255;
				else Q_tab[i][j] = floor((m_Q[i][j]*lambda+50)/100);
			}
		}
		return Q_tab;
	}



	void quantification (double **DCT_img, int** img_quant){
		int** Q_tab = get_quantification_table();

		for (unsigned i = 0; i < block_sz; i++){
			for (unsigned j = 0; j < block_sz; j++){
				img_quant[i][j] = round(DCT_img[i][j] / Q_tab[i][j]);
			}
		}
		cout<<"\n quantification matrix : \n"<<endl;
		show<int>(Q_tab);

		cout<<"\n quantification out : \n"<<endl;
		show<int>(img_quant);
	}

	void dequantification (int **img_quant, double** DCT_img){
			int** Q_tab = get_quantification_table();

			for (unsigned i = 0; i < block_sz; i++){
				for (unsigned j = 0; j < block_sz; j++){
					DCT_img[i][j] = img_quant[i][j] * Q_tab[i][j];
				}
			}

			cout<<"\n dequantification out : \n"<<endl;
			show<double>(DCT_img);
		}

	template <typename T>
	void show(T** mat){
		for (unsigned i = 0; i < block_sz; i++){
			for (unsigned j = 0; j < block_sz; j++){
				cout<< setw(7) << setprecision(3) << mat[i][j] << setw(3)<< '|';
			}
			cout<<"\n";
		}
	}
};



#endif /* DCTCOMPRESSION_HPP_ */
