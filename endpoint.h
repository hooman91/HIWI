/*
 * endpoint.h
 *
 *  Created on: Jun 6, 2015
 *      Author: hooman
 */

#ifndef endpoint_H_
#define endpoint_H_

#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlp_messages.h"

using namespace sc_core;
using namespace tlm;
using namespace tlm_utils;

#define SC_INCLUDE_DYNAMIC_PROCESSES

SC_MODULE(endpoint) {

public:
	simple_initiator_socket<endpoint> i_socket;
	simple_target_socket<endpoint> t_socket;
private:
	sc_event send_event;
	sc_event response_event;
private:
	int request_rate;
	int ID;

	peq_with_get<tlm_generic_payload> r_peq;
	peq_with_get<tlm_generic_payload> w_peq;
private:
	void send_trigger();
	void send_procedure();
	void do_read();
	void do_write();




public:
	//to register with initiator socket
	tlm_sync_enum                            // returns status
	nb_transport_bw
	( tlm_generic_payload  &payload,         // ref to payload
	  tlm_phase            &phase,		 // ref to phase
	  sc_time              &delay_time       // ref to delay time
	);
	//to register with target socket
	tlm_sync_enum                            // returns status
	nb_transport_fw
	( tlm_generic_payload  &payload,         // ref to payload
	  tlm_phase            &phase,		 // ref to phase
	  sc_time              &delay_time       // ref to delay time
	);

//////////constructor
//public:
//
//	SC_CTOR(endpoint):i_socket("i_socket"), t_socket("t_socket"),ID(15) {
//		i_socket.register_nb_transport_bw(this, &endpoint::nb_transport_bw);
//		t_socket.register_nb_transport_fw(this, &endpoint::nb_transport_fw);
//
//		request_rate = 10;
//
//		SC_THREAD(send_trigger);
//		SC_THREAD(endpoint);
//
//	}


	//we want constructor with arguments. therefore, we cannot use SC_CTOR
public:
	SC_HAS_PROCESS(endpoint);
public:
	endpoint(sc_module_name name_, int request_rate_, int clk_frq,int lanes, int ID_):\
	sc_module(name_),i_socket("i_socket"),t_socket("t_socket"),r_peq("r_peq"),w_peq("w_peq"),request_rate(request_rate_), ID(ID_)
	{
		i_socket.register_nb_transport_bw(this, &endpoint::nb_transport_bw);
		t_socket.register_nb_transport_fw(this, &endpoint::nb_transport_fw);

		SC_THREAD(send_trigger);
		SC_THREAD(send_procedure);
		SC_THREAD(do_read);
		sensitive << r_peq.get_event();
		SC_THREAD(do_write);
		sensitive << w_peq.get_event();
		//cout<<"ep1:"<<this->name()<<" intialized successfully"<< endl;
	}

};

#endif /* endpoint_H_ */
