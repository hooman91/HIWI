/*
 * switch.h
 *
 *  Created on: Jun 17, 2015
 *      Author: hooman
 */

#ifndef SWITCH_H_
#define SWITCH_H_

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlp_messages.h"
#include "port.h"

using namespace sc_core;
using namespace tlm;
using namespace tlm_utils;

SC_MODULE(switch_pcie){


public:
	simple_initiator_socket_tagged<switch_pcie> *i_socket[3];

	sc_in<bool>* not_empty_interrupt;
private:
	int n_ports;
	int n_vc;
	bool* pending_transaction;
	npu_header header;
	sc_event receive_event;


private:
	void npu_process();
public:
	tlm_sync_enum nb_transport_bw( int index, tlm_generic_payload& transaction, tlm_phase& phase, sc_time& time);


SC_HAS_PROCESS(switch_pcie);
public:
	switch_pcie(sc_module_name name_, int n_vc_, int n_ports_)\
	:sc_module(name_), n_ports(n_ports_), n_vc(n_vc_){

		pending_transaction = new bool[n_ports];
		not_empty_interrupt = new sc_in<bool>[n_ports];
		//i_socket = new simple_initiator_socket_tagged<switch_pcie>[n_ports];
		for(int i=0;i<3;i++){
//			i_socket[i] = ("hello");
//			i_socket[i] = new simple_initiator_socket_tagged<switch_pcie>();
			i_socket[i] = new simple_initiator_socket_tagged<switch_pcie>("hello");
			//i_socket[i].register_nb_transport_bw(this, &switch_pcie::nb_transport_bw,i);
			i_socket[i]->register_nb_transport_bw(this, &switch_pcie::nb_transport_bw,i);
		}
//		i_socket.register_nb_transport_bw(this, &switch_pcie::nb_transport_bw,4);
		SC_THREAD(npu_process);
	}
};



#endif /* SWITCH_H_ */
