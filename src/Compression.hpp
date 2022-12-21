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
#include "Huffman.hpp"

using namespace std;

class Compression{
	static const unsigned MAX_FRAME_SIZE = 64;
	const unsigned img_sz = 128;
	const unsigned block_sz = 8;
	const unsigned frame_sz = 64;
	unsigned n_blocks = (img_sz/block_sz) * (img_sz/block_sz);
	int ** m_image = new int*[img_sz];
	int * frame = new int[n_blocks*frame_sz];
	int * data;
	double * freq;
	DCTCompression * dct = new DCTCompression(img_sz,img_sz,50);
	Huffman * huff;

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
		int min = 0 , max = 0;
		for (unsigned i = 0; i < n_blocks * MAX_FRAME_SIZE; i++){
			if(frame[i]<min)min = frame[i];
			if (frame[i]>max)max = frame[i];
		}
		cout<<"min : "<< min << ", max : "<<max<<endl;
		int data_len = max-min+1;
		data = new int[data_len];
		freq = new double[data_len];
		for (int i = 0; i < data_len; i++)data[i] = min+i;
		dct->show1D(data, data_len);
		histogram(frame, n_blocks * MAX_FRAME_SIZE, data,freq,data_len);
	}
	~Compression(){

	}
	unsigned histogram ( int* frame, unsigned len, int * data, double * freq, int data_len){
		for (unsigned i = 0; i<len ; i++){
			for (int j = 0; j<data_len ; j++){
				if (frame[i] == data[j]) freq[j]++;
			}
		}
		dct->show1D<int>(data, data_len);
		dct->show1D<double>(freq, data_len);

		huff = new Huffman(data,freq, data_len);
		char ** codes = new char*[400];
		for (int i = 0; i < 400; i ++){
			codes[i] = new char[64];
		}
		huff->get_codes(huff->pq.top(), codes, data_len, "");
		for (int i = 0; i < data_len; i ++){
			cout<<"i : "<< i <<", data : "<< data[i]<<", freq : "<< freq[i]<<", code : "<<codes[i]<<endl;
		}
		char ** frame_codes = new char * [len];
		for (unsigned i = 0; i < len ; i++){
			frame_codes[i] = new char[64];
			frame_codes[i] = codes[158 + frame[i]];
		}
		for (unsigned i = 0; i < len ; i++){
			cout<<"value : "<< frame[i]<< ", code : "<< frame_codes[i]<<endl;
		}
		return data_len;
	}
	void read_stream(char * file){

	}
//	void write_stream(char * frame, char* file_name){
//		try {
//			ofstream file;
//			file.open(file_name);
//
//			//file << ... << endl;
//
//			file.close();;
//		} catch(ofstream::failure& e){
//			cout << "Exception while writing " << file_name << " : " <<e.what() << endl;
//		}
//	}

};


#endif /* COMPRESSION_HPP_ */
