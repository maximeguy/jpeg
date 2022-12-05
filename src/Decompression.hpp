/*
 * Compression.hpp
 *
 *  Created on: 5 déc. 2022
 *      Author: maxime
 */

#ifndef DECOMPRESSION_HPP_
#define DECOMPRESSION_HPP_
#include <iostream>
#include <fstream>

using namespace std;

class Decompression{
	const unsigned img_sz = 129;

	int ** image = new int*[img_sz];

	for (unsigned i = 0; i < img_sz; i++){
		image[i] = new int[img_sz];
	}
	Decompression(){

	}
	~Decompression(){

	}

	void read_stream(char * file){
		ifstream img_file;
		img_file.open(file, ios_base::in);
		if (!img_file){
			cout<<"Can't read file."<<endl;
		}
		else{
			cout<<"Reading file..."<<endl;
			int number;
			unsigned i = 0;
			unsigned j = 0;
			while (img_file >> number) {
				image[i][j++] = number;
				if (j == img_sz-1){
					j=0;
					i++;
				}
			}
			img_file.close();
		}

		for(unsigned i = 0; i < img_sz; i++){
			for(unsigned j = 0; j < img_sz; j++){
				cout << image[i][j] << ",";
			}
			cout << endl;
		}
	}
	void write_stream(char * frame, char* file){

	}

};


#endif /* DECOMPRESSION_HPP_ */
