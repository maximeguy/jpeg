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
	int **m_buffer;
	int **m_origin;
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
		m_buffer = new int*[height];
		for (unsigned i = 0; i < height; i++){
			m_buffer[i]= new int[width];
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
		//fill in buffer with original image for EQM
//		for (unsigned i = 0; i < block_sz; i++){
//			for (unsigned j = 0; j < block_sz; j++){
//				m_buffer[i][j] = block[i][j];
//			}
//		}

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
		show2D<double>(DCT_img);

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
						//cout<<DCT_img[u][v]<<endl;
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
		show2D<int>(block);
	}

	/**
	 * Retrieve JPEG quantification table
	 */

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

	/**
	 * JPEG quantification
	 * @param DCT_img : Pointer to the output image
	 * @param img_quant : Pointer to the quantified image
	 */

	void quantification (double **DCT_img, int** img_quant){
		int** Q_tab = get_quantification_table();

		for (unsigned i = 0; i < block_sz; i++){
			for (unsigned j = 0; j < block_sz; j++){
				img_quant[i][j] = round(DCT_img[i][j] / Q_tab[i][j]);
			}
		}
		cout<<"\n quantification matrix : \n"<<endl;
		show2D<int>(Q_tab);

		cout<<"\n quantification out : \n"<<endl;
		show2D<int>(img_quant);
	}

	/**
	 * JPEG dequantification
	 * @param img_quant : Pointer to the quantified image
	 * @param DCT_img : Pointer to the output image
	 */

	void dequantification (int **img_quant, double** DCT_img){
		int** Q_tab = get_quantification_table();

		for (unsigned i = 0; i < block_sz; i++){
			for (unsigned j = 0; j < block_sz; j++){
				DCT_img[i][j] = img_quant[i][j] * Q_tab[i][j];
			}
		}

		cout<<"\n dequantification out : \n"<<endl;
		show2D<double>(DCT_img);
	}


	/**
	 * Mean squared deviation
	 * Measures the average of the squares of the errors, (average loss of data)
	 * @param block : Pointer to the quantified image to be compared with the original
	 */
	double EQM(int ** block, int original_block[][block_sz]){
		double EQM;

		double ** block_dequant = new double*[block_sz];
		int ** block_idct = new int*[block_sz];
		for(unsigned i=0; i< block_sz; i++){
			block_dequant[i] = new double[block_sz];
			block_idct[i] = new int[block_sz];
		}

		dequantification(block, block_dequant);
		IDCT_Block(block_dequant, block_idct);


		for (unsigned i = 0; i < block_sz; i++){
			for (unsigned j = 0; j < block_sz; j++){
				EQM += pow(original_block[i][j]-block_idct[i][j],2);
			}
		}
		cout<<EQM/64<<endl;
		return EQM /= 64;
	}

	/**
	 * Rate of compression between the quantified image and the original
	 * @param block : Pointer to the quantified image
	 */
	double compression_rate(int ** block){

	}

	/**
	 * Run length encoding of a 8x8 block from the image
	 * Run through all the index except [0,0] in a diagonal zigzag pattern, each non-zero coefficient cn is then extracted with a number of zeros that precedes it nz_cn.
	 * The information is included in an array [DC, nz_c1, c1, nz_c2,c2,...,nz_cn,cn]
	 * DC represents the mean value of the previous block.
	 * @param img_quant : Pointer to the quantified image
	 * @param DC_prev : Mean value of the previous block
	 * @param frame : Pointer to an array containing the encoded information
	 */
	void RLE_Block(int** img_quant, int DC_prev, int * frame){
		unsigned i, j,zeros,frame_idx;
		i=1;
		j = zeros = frame_idx = 0;
		frame[frame_idx++]=img_quant[0][0];
		bool sw = false;
		for (unsigned n = 0 ; n<62; n++){
			if (img_quant[j][i] == 0) zeros++;
			else{
				frame[frame_idx++]=zeros;
				frame[frame_idx++]=img_quant[j][i];
				zeros = 0;
			}
			//printf("index [%d, %d], value %u  : %d, dir : %s \n", j,i,n+1,img_quant[j][i],sw ? "up" : "down");
			if(n<34){
				if (!sw){
					if (i!=0)i -= 1;
					else sw = !sw;
					j += 1;
				}
				else{
					if (j!=0)j -= 1;
					else sw = !sw;
					i += 1;
				}
			}
			else{
				if (!sw){
					if (j!=7){
						i -= 1;
						j += 1;
					}
					else {
						sw = !sw;
						i += 1;
					}

				}
				else{
					if (i!=7){
						j -= 1;
						i += 1;
					}
					else {
						sw = !sw;
						j += 1;
					}
				}
			}
		}
		show1D(frame,15);
	}

	/**
	 * Calls RLE_Block for each block of an image and concatenate their frames
	 * @param frame : Pointer to  an array resulting of the concatenation of each block's frames
	 */
	void RLE(int*frame){
		int ** block = new int*[block_sz];
		int ** frames = new int*[(m_height/block_sz)*(m_width/block_sz)];
		int * temp_frame;
		int DC = 0;
		for (unsigned j = 0; j < m_height; j+=block_sz){
			for (unsigned i = 0; i < m_width; i+=block_sz){

				for (unsigned y = j; y < j+block_sz; y++){
					block[y] = new int[block_sz];
					for (unsigned x = i; x < i+block_sz; x++){
						block[y][x] = m_buffer[y][x];
					}
				}
				RLE_Block(block, DC, temp_frame);
				frames[j] = new int[sizeof(temp_frame)/sizeof(int)];
				for (unsigned u = 0; u < sizeof(temp_frame)/sizeof(int); u++){
					frames[j][i]= temp_frame[u];
				}
			}

		}

	}

	template <typename T>
	void show2D(T** mat){
		for (unsigned i = 0; i < block_sz; i++){
			for (unsigned j = 0; j < block_sz; j++){
				cout<< setw(7) << setprecision(3) << mat[i][j] << setw(3)<< '|';
			}
			cout<<"\n";
		}
	}

	void show1D(int* vect, unsigned len){
		for(unsigned i = 0; i< len; i++) cout<<setw(4)<<vect[i]<<"|";
		cout<<"\n";
	}
};



#endif /* DCTCOMPRESSION_HPP_ */
