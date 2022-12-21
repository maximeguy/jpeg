/*
 * Huffman.hpp
 *
 *  Created on: 16 nov. 2022
 *      Author: maxime
 */

#ifndef HUFFMAN_HPP_
#define HUFFMAN_HPP_

#include <iostream>
#include <cstring>
#include <queue>
#include <vector>

using namespace std;


/*
 * Huffman encoding class
 * Encodes data losslessly to the smallest possible bit size
 */
class Huffman{

	/*
	 * Node structure representing a data element with its frequencies, it is linked to two adjacent left and right nodes
	 */
	struct node {
		int s_val;
		double s_freq;
		node* s_left;
		node* s_right;

		/*
		 * Node constructor, assign member values and initializes left and right node to nullptr
		 * @param val : Char of the data element
		 * @param freq : Double representing the frequency of the element in the dataset
		 */
		node( int val, double freq ) {
			s_val = val;
			s_freq = freq;
			s_left = nullptr;
			s_right = nullptr;
		}

		~node() {
		  if ( s_left ) delete s_left;
		  if ( s_right ) delete s_right;
		}
	};
	/*
		 * Priority queue comparison function to order the queue, gets the lowest frequency of two nodes
		 * Results in getting the lowest frequency first
		 */
		struct compare{
			bool operator()(node * left, node * right){
				return (left->s_freq > right->s_freq);
			}
		};
	int * m_frame;
	unsigned m_len;
	node * m_head;
public:
	priority_queue<node*, vector<node*>, compare> pq;

	/*
	 * Huffman encoder constructor, build the queue from nodes encapsulating data elements (characters) and frequencies of the elements
	 * @param data : Char array containing the characters encoded
	 * @param freq : Double array containing the frequencies (number of occurences / number of elements) of each elements
	 * @param len  : Unsigned number of distinct data elements
	 */
	Huffman(int * data, double * freq, unsigned len){
		m_len = len;
		m_head = nullptr;
		m_frame = nullptr;

		for (unsigned i = 0 ; i < len; i++){
			pq.push(new node(data[i],freq[i]));
		}

		while(pq.size() > 1 ){
			//cout<<"Queue size : "<<(int)pq.size();
			node * nl = pq.top();
			pq.pop();
			node * nr = pq.top();
			pq.pop();
			node * np = new node('#', nl->s_freq + nr->s_freq);
			np->s_left = nl;
			np->s_right = nr;
//			cout<<" >> Push parent node with freq : "<< np->s_freq<<", with childrens : [("
//					<<np->s_left->s_val<<", "<<np->s_left->s_freq<<"), ("<<np->s_right->s_val<<", "<<np->s_right->s_freq<<")]"<<endl;
			pq.push(np);
		}
		//printCodes(pq.top(), "");
	}


	/*
	 * Browse nodes and prints the binary codes of the data elements
	 * @param node : Node to evaluate
	 * @param str: String used to represent the code
	 */
	void printCodes(struct node * root, string str){
		if (!root) return;

		if (root->s_val != '#')cout << root->s_val << ": " << str << "\n";
		printCodes(root->s_left, str + "0");
		printCodes(root->s_right, str + "1");
	}

	/*
	 * Browse nodes and prints the binary codes of the data elements
	 * @param node : Node to evaluate
	 * @param str: String used to represent the code
	 */
	void get_codes(struct node * root, char**codes, int data_len, string str){
		if (!root) return;

		if (root->s_val != '#'){
			cout<<"value : "<< root->s_val << ", code : "<< str << ", idx : "<< 158 + root->s_val<<endl;
			//cout<<"value : "<< root->s_val << ", codes idx : "<<data_len - root->s_val-159<<"/"<<data_len<<endl;
			strcpy(codes[158 + root->s_val], str.c_str());
		}
		get_codes(root->s_left,codes,data_len, str + "0");
		get_codes(root->s_right,codes,data_len, str + "1");
	}
};



#endif /* HUFFMAN_HPP_ */
