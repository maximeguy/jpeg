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
#include "stdio.h"

using namespace std;

class DCTCompression{
	unsigned m_width;
	unsigned m_height;
	unsigned char **m_buffer;
	unsigned int m_quality;

public:
	static const unsigned block_sz = 8;
	DCTCompression(const unsigned width,const unsigned height,const unsigned quality = 50){
		m_width = width;
		m_height = height;
		m_quality = quality;
		m_buffer = new unsigned char*[height];
		for (unsigned i = 0; i < height; i++){
			m_buffer[i]= new unsigned char[width];
		}
	}

	~DCTCompression(){

	}

	void SetWidth(unsigned width){
		m_width = width;
	}

	void SetHeight(unsigned height){
		m_height = height;
	}

	void SetQuality(unsigned quality){
		m_quality = quality;
	}

	unsigned GetWidth(){
		return m_width;
	}

	unsigned GetHeight(){
		return m_height;
	}

	unsigned GetQuality(){
		return m_quality;
	}

	void DCT_Block(double**DCT_img, int block[][block_sz]){
		cout<<"dct in : "<<endl;
		for (unsigned x = 0; x < block_sz; x++){
			for (unsigned y = 0; y < block_sz; y++){
				printf("%d | ",block[x][y]-128);
			}
			cout<<"\n";
		}
		double cu, cv, p,pi;
		p=0;
		pi=atan(1)*4.;
		for (unsigned u = 0; u < block_sz; u++){
			u == 0 ? cu=1/sqrt(2) : cu=1;
			for (unsigned v = 0; v < block_sz; v++){
				v == 0 ? cv=1/sqrt(2) : cv=1;
				cout<<u<<", "<<v<<" : "<<cu<<", "<<cv<<endl;
				for (unsigned x = 0; x < block_sz; x++){
					for (unsigned y = 0; y < block_sz; y++){

						p += (block[x][y]-128)*
								cos((2*x+1)*pi*u/16.)*
								cos((2*y+1)*pi*v/16.);

					}
				}
				DCT_img[u][v]= cv*cu*p/4.;
				p=0;
			}
		}
		cout<<"dct out : "<<endl;
		for (unsigned x = 0; x < block_sz; x++){
			for (unsigned y = 0; y < block_sz; y++){
				printf("%f | ",DCT_img[x][y]);
			}
			cout<<"\n";
		}
	}

	void IDCT_Block(double**DCT_img, int block[][block_sz]){
		double cu, cv, p,pi;
		p=0;
		pi=atan(1)*4.;
		for (unsigned x = 0; x < block_sz; x++){
			for (unsigned y = 0; y < block_sz; y++){

				for (unsigned u = 0; u < block_sz; u++){
					u == 0 ? cu=1/sqrt(2) : cu=1;

					for (unsigned v = 0; v < block_sz; v++){
						v == 0 ? cv=1/sqrt(2) : cv=1;

						p += (block[x][y]-128)*cv*cu*
								cos((2*x+1)*pi*u/16.)*
								cos((2*y+1)*pi*v/16.);

					}
				}
				DCT_img[x][y]= p/4.;
				p=0;
			}
		}
	}
};



#endif /* DCTCOMPRESSION_HPP_ */
