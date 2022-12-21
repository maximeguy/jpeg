/*
 * Compression.hpp
 *
 *  Created on: 5 déc. 2022
 *      Author: Maxime GUY
 */

#ifndef COMPRESSION_HPP_
#define COMPRESSION_HPP_
#include <iostream>
#include <fstream>
#include "DCTCompression.hpp"
#include "Huffman.hpp"

using namespace std;

class Compression {
	const unsigned MAX_FRAME_SIZE = 64;
	const unsigned img_sz = 128;
	const unsigned block_sz = 8;
	const unsigned n_blocks = (img_sz / block_sz) * (img_sz / block_sz);
	const unsigned frame_len = n_blocks * MAX_FRAME_SIZE;
	int **m_image = new int*[img_sz];
	int *frame = new int[frame_len];
	int *data;
	double *freq;
	DCTCompression *dct = new DCTCompression(img_sz, img_sz, 50);
	Huffman *huff;

public:
	/**
	 * Compression class constructor. Reads data file, from wich an RLE frame is extracted and compute an histogram to encode the frame (Huffman) and write it to a new file.
	 * @param file_name : Name of the file to read
	 */
	Compression(char *file_name) {
		for (unsigned i = 0; i < img_sz; i++) {
			m_image[i] = new int[img_sz];
		}

		read_stream(file_name);

		dct->RLE(frame);

		int min = 0, max = 0;
		for (unsigned i = 0; i < n_blocks * MAX_FRAME_SIZE; i++) {
			if (frame[i] < min) min = frame[i];
			if (frame[i] > max) max = frame[i];
		}

		cout << "frame min : " << min << ", max : " << max << " values." << endl;

		int data_len = max - min + 1;
		data = new int[data_len];
		freq = new double[data_len];

		for (int i = 0; i < data_len; i++)
			data[i] = min + i;

		histogram(frame, frame_len, data, freq, data_len);

		huff = new Huffman(data, freq, data_len);

		//huff->print_codes(huff->pq.top(), ""); // Either use print_codes or get_codes but not both

		char **codes = new char*[data_len];
		for (int i = 0; i < data_len; i++) {
			codes[i] = new char[64];
		}
		huff->get_codes(huff->pq.top(), codes, min, max, "");
		// Somehow there are some longer codes in codes[] than while checking codes directrly in Huffman::get_codes
//		for (int i = 0; i < data_len; i++) {
//			cout << "i : " << i << ", data : " << data[i] << ", freq : " << freq[i] << ", code : " << codes[i] << endl;
//		}
		char **frame_codes = new char*[frame_len];
		for (unsigned i = 0; i < frame_len; i++) {
			frame_codes[i] = new char[256];
			frame_codes[i] = codes[158 + frame[i]];
		}

		// Write encoded frame to file
		write_stream(frame_codes, "lenna_encoded.txt");
	}

	/**
	 Compression class destructor
	 */
	~Compression() {
		delete (frame);
		for (unsigned i = 0; i < img_sz; i++) {
			delete m_image[i];
		}
		delete m_image;
		delete data;
		delete freq;
		delete dct;
		delete huff;
	}

	/**
	 * Compute histogram of the RLE frame, the frequency of each value is computed.
	 * @param frame : RLE frame
	 * @param len : RLE frame length
	 * @param data : values contained in RLE frame
	 * @param freq : frequencies of the values contained in RLE frame
	 * @param data_len : Number of values to encode
	 * @return returns data_len
	 */
	unsigned histogram(int *frame, unsigned len, int *data, double *freq, int data_len) {
		for (unsigned i = 0; i < len; i++) {
			for (int j = 0; j < data_len; j++) {
				if (frame[i] == data[j]) freq[j]++;
			}
		}
		cout << "frame hist data :";
		dct->show1D<int>(data, data_len);
		cout << "frame hist freq :";
		dct->show1D<double>(freq, data_len);

		return data_len;
	}

	/**
	 * Reads data file.
	 * @param file_name : Name of the file to read
	 */
	void read_stream(char *file_name) {
		ifstream img_file;
		cout << "Opening '" << file_name << "'." << endl;
		img_file.open(file_name, ios_base::in);
		if (!img_file) {
			cout << "Can't read file." << endl;
		} else {
			cout << "Reading file..." << endl;
			int number;
			unsigned i = 0;
			unsigned j = 0;
			while (img_file >> number) {
				m_image[i][j++] = number;
				if (j == img_sz) {
					j = 0;
					i++;
				}
			}
			cout << "Image buffered, closing '" << file_name << "'." << endl;
			img_file.close();
		}
		dct->set_buffer(m_image);
	}

	/**
	 * Write encoded data file.
	 * @param frame_codes : encoded frame array
	 * @param file_name : Name of the file to write
	 */
	void write_stream(char **frame_codes, char *file_name) {
		try {
			ofstream file;
			file.open(file_name, ios::out | ios::binary);

			// Not sure about how to format the file.
			//file << ... << endl;

			file.close();
			;
		} catch (ofstream::failure &e) {
			cout << "Exception while writing " << file_name << " : " << e.what() << endl;
		}
	}

};

#endif /* COMPRESSION_HPP_ */
