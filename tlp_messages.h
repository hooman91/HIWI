/*
 * tlp_messages.h
 *
 *  Created on: Jun 6, 2015
 *      Author: hooman
 */

#ifndef TLP_MESSAGES_H_
#define TLP_MESSAGES_H_

#include "iostream"
struct DW0{
	char Fmt;
	char R;
	char Type;
	unsigned short Length;
	char TC;
	char TD;
	char EP;
	char Attr;

};
struct DW1{
	short RequesterID;
	char Tag;
	char LastDWBE;
	char FirstDWBE;
};
struct DW2{
	int Address;
};
struct DW3{
	int Address;
};

struct tlp_header{

	DW0 dw0;
	DW1 dw1;
	DW2 dw2;
	DW3 dw3;
	friend std::ostream& operator<<(std::ostream & Str, tlp_header const & v);

};
// NPU sends this type of header to output ports
struct npu_header{
	tlp_header* header;
	int port;
	friend std::ostream& operator<<(std::ostream & Str, npu_header const & v);
};



#endif /* TLP_MESSAGES_H_ */
