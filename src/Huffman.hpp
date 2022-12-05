/*
 * Huffman.hpp
 *
 *  Created on: 16 nov. 2022
 *      Author: maxime
 */

#ifndef HUFFMAN_HPP_
#define HUFFMAN_HPP_

#include <iostream>
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
		char s_val;
		double s_freq;
		node* s_left;
		node* s_right;

		/*
		 * Node constructor, assign member values and initializes left and right node to nullptr
		 * @param val : Char of the data element
		 * @param freq : Double representing the frequency of the element in the dataset
		 */
		node( char val, double freq ) {
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
	char * m_frame;
	unsigned m_len;
	node * m_head;

	void browse(node *leaf){
		if (leaf != nullptr){
			cout<<leaf->s_freq<<endl;
			browse(leaf->s_left);
			//cout<<"left\n"<<endl;
			browse(leaf->s_right);
			//cout<<"right\n"<<endl;
		}
	}

public:
	/*
	 * Priority queue comparison function to order the queue, gets the lowest frequency of two nodes
	 * Results in getting the lowest frequency first
	 */
	struct compare{
		bool operator()(node * left, node * right){
			return (left->s_freq > right->s_freq);
		}
	};
	/*
	 * Huffman encoder constructor, build the queue from nodes encapsulating data elements (characters) and frequencies of the elements
	 * @param data : Char array containing the characters encoded
	 * @param freq : Double array containing the frequencies (number of occurences / number of elements) of each elements
	 * @param len  : Unsigned number of distinct data elements
	 */
	Huffman(char data[6], double freq[6], unsigned len){
		m_len = len;
		m_head = nullptr;
		m_frame = nullptr;
		priority_queue<node*, vector<node*>, compare> pq;
		for (unsigned i = 0 ; i < len; i++){
			pq.push(new node(data[i],freq[i]));
		}

		while(pq.size() > 1 ){
			cout<<"Queue size : "<<(int)pq.size();
			node * nl = pq.top();
			pq.pop();
			node * nr = pq.top();
			pq.pop();
			node * np = new node('#', nl->s_freq + nr->s_freq);
			np->s_left = nl;
			np->s_right = nr;
			cout<<" >> Push parent node with freq : "<< np->s_freq<<", with childrens : [("
					<<np->s_left->s_val<<", "<<np->s_left->s_freq<<"), ("<<np->s_right->s_val<<", "<<np->s_right->s_freq<<")]"<<endl;
			pq.push(np);
		}
		printCodes(pq.top(), "");
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

	void show(){
		if (m_head != nullptr){
			cout<<"["<<m_head->s_val<<", "<<"]"<<endl;
			browse(m_head->s_left);
			browse(m_head->s_right);
		}
	}
};



#endif /* HUFFMAN_HPP_ */
