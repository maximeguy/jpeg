/*
 * Compression.hpp
 *
 *  Created on: 5 déc. 2022
 *      Author: maxime
 */

#ifndef COMPRESSION_HPP_
#define COMPRESSION_HPP_
#include <iostream>
#include <fstream>
#include "DCTCompression.hpp"

using namespace std;

class Compression{
	const unsigned img_sz = 128;
	int ** m_image = new int*[img_sz];
	int * frame = new int[img_sz/8];
	DCTCompression * dct = new DCTCompression(img_sz,img_sz,50);

public:
	Compression(char * filename){
		for (unsigned i = 0; i < img_sz; i++){
			m_image[i] = new int[img_sz];
		}
		ifstream img_file;
		cout<<"Opening '"<<filename<<"'."<<endl;
		img_file.open(filename, ios_base::in);
		if (!img_file){
			cout<<"Can't read file."<<endl;
		}
		else{
			cout<<"Reading file..."<<endl;
			int number;
			unsigned i = 0;
			unsigned j = 0;
			while (img_file >> number) {
				m_image[i][j++] = number;
				if (j == img_sz){
					j=0;
					i++;
				}
			}
			cout<<"Image buffered, closing '"<<filename<<"'."<<endl;
			img_file.close();
		}
		dct->SetBuffer(m_image);
		dct->RLE(frame);
	}
	~Compression(){

	}
	unsigned histogram ( char* frame, unsigned len, char * data, double * freq){

	}
	void read_stream(char * file){

	}
	void write_stream(char * frame, char* file){

	}

};


#endif /* COMPRESSION_HPP_ */
