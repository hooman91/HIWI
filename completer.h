/*
 * completer.h
 *
 *  Created on: Jun 6, 2015
 *      Author: hooman
 */

#ifndef completer_H_
#define completer_H_

#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlp_messages.h"

using namespace sc_core;
using namespace tlm;
using namespace tlm_utils;

#define SC_INCLUDE_DYNAMIC_PROCESSES

SC_MODULE(completer) {

public:
	simple_target_socket<completer> t_socket;
private:

private:
	int request_rate;
	int ID;
	sc_mutex mutex;

	peq_with_get<tlm_generic_payload> r_peq;
	peq_with_get<tlm_generic_payload> w_peq;
private:
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
//	SC_CTOR(completer):i_socket("i_socket"), t_socket("t_socket"),ID(15) {
//		i_socket.register_nb_transport_bw(this, &completer::nb_transport_bw);
//		t_socket.register_nb_transport_fw(this, &completer::nb_transport_fw);
//
//		request_rate = 10;
//
//		SC_THREAD(send_trigger);
//		SC_THREAD(completer);
//
//	}


	//we want constructor with arguments. therefore, we cannot use SC_CTOR
public:
	SC_HAS_PROCESS(completer);
public:
	completer(sc_module_name name_, int request_rate_, int clk_frq,int lanes, int ID_):\
	sc_module(name_),t_socket("t_socket"),r_peq("r_peq"),w_peq("w_peq"),request_rate(request_rate_), ID(ID_)
	{

		t_socket.register_nb_transport_fw(this, &completer::nb_transport_fw);
		SC_THREAD(do_read);
		sensitive << r_peq.get_event();
		SC_THREAD(do_write);
		sensitive << w_peq.get_event();
	}

};

#endif /* completer_H_ */
