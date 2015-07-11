/*
 * root_complex.h
 *
 *  Created on: Jun 13, 2015
 *      Author: hooman
 */

#ifndef ROOT_COMPLEX_H_
#define ROOT_COMPLEX_H_

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlp_messages.h"

using namespace sc_core;
using namespace tlm;
using namespace tlm_utils;




SC_MODULE(root_complex){

public:
	simple_initiator_socket<root_complex> i_socket;
private:
	void processor_core(int index);
	void runner();
	tlm_sync_enum  nb_transport_bw
	( tlm_generic_payload  &payload,         // ref to payload
	  tlm_phase            &phase,		 // ref to phase
	  sc_time              &delay_time       // ref to delay time
	);
private:
	int number_of_cores;
	int* request_rate;
	sc_event* response_event;
	sc_mutex mutex;


	//we want constructor with arguments. therefore, we cannot use SC_CTOR
public:
	SC_HAS_PROCESS(root_complex);
public:
	root_complex(sc_module_name name_,int clk_frequency,int number_of_cores_, int request_rate_[], int ID):\
	sc_module(name_), i_socket("i_socket"), number_of_cores(number_of_cores_){

		i_socket.register_nb_transport_bw(this, &root_complex::nb_transport_bw);
		request_rate = new int[number_of_cores];
		for(int i=0;i<number_of_cores;i++)	{request_rate[i]= request_rate_[i];}
		response_event = new sc_event[number_of_cores];
		SC_THREAD(runner);



	}

};





#endif /* ROOT_COMPLEX_H_ */
