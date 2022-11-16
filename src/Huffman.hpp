/*
 * Huffman.hpp
 *
 *  Created on: 16 nov. 2022
 *      Author: maxime
 */

#ifndef HUFFMAN_HPP_
#define HUFFMAN_HPP_

class Huffman{
	char * m_frame;
	unsigned m_len;
	unsigned m_head;

	struct node {
	    int   m_val;
	    node* m_next;
	    node* m_prev;

	    node( int val ) {
	    	m_val = val;
	    	m_prev = nullptr;
	    	m_next = nullptr;
	    }
	    ~node() {
	      if ( m_prev ) delete m_next;
	    }
	  } ;
public:
	Huffman(){

	}
};



#endif /* HUFFMAN_HPP_ */
