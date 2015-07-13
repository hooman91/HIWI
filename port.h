/*
 * port.h
 *
 *  Created on: Jun 21, 2015
 *      Author: hooman
 */

#ifndef PORT_H_
#define PORT_H_

#define EGRESS_FIFO_DEPTH 50
#define INGRESS_FIFO_DEPTH 50

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlp_messages.h"

using namespace sc_core;
using namespace tlm;
using namespace tlm_utils;

SC_MODULE(port_pcie){


public:
	simple_target_socket<port_pcie> t_socket_to_switch;
	simple_initiator_socket<port_pcie> i_socket_to_outside;
	simple_target_socket<port_pcie> t_socket_to_outside;

	sc_out<bool> not_empty_interrupt;
private:
	int n_vc; 		//number of VCS
	int n_port;		//number of ports in the corresponding switch module
	int port_index;	//index of this port
	int* port_arbitration_result;
	int vc_arbitration_result;
//	bool* vc_n_empty;
	bool pending_transaction;
	sc_fifo<tlp_header>*** port_arbiter_fifo;
	sc_fifo<npu_header> from_switch_fifo;       //??? which data type to use? port number has to be included with tlp
	sc_fifo<npu_header> to_switch_fifo;
	sc_event packet_from_switch;
	sc_event packet_outside;
	sc_event send_event;
	sc_event transmit_event;
//	sc_event switch_read_cmd;
//	sc_event switch_write_cmd;
	peq_with_get<tlm_generic_payload> switch_read_peq;
	peq_with_get<tlm_generic_payload> switch_write_peq;
	peq_with_get<tlm_generic_payload> outside_write_peq;
private:
	int tc_vc_mapper(int tc);
	void port_arbiter(int index);
	void vc_arbiter();
	///
	void mapping_process();
	void transmit_process();
	void not_empty_process();
	void switch_read();
	void switch_write();
	void outside_write();
public:
	tlm_sync_enum nb_transport_bw_outside(tlm_generic_payload& transaction, tlm_phase& phase, sc_time& time);
	tlm_sync_enum nb_transport_fw_outside(tlm_generic_payload& transaction, tlm_phase& phase, sc_time& time);
	tlm_sync_enum nb_transport_fw_switch(tlm_generic_payload& transaction, tlm_phase& phase, sc_time& time);


SC_HAS_PROCESS(port_pcie);
public:
	port_pcie(sc_module_name name_,int n_vc_, int n_port_, int port_index_)\
	:sc_module(name_), n_vc(n_vc_), n_port(n_port_), t_socket_to_switch("t_socket_to_switch")\
	 , i_socket_to_outside("i_socket_to_outside"), t_socket_to_outside("t_socket_to_outside"), port_index(port_index_)\
	 , switch_read_peq("switch_read_peq"), switch_write_peq("switch_write_peq"), outside_write_peq("outside_write_peq"){
		port_arbitration_result = new int[n_vc];
		vc_arbitration_result = 0;
		pending_transaction = false;
//		port_arbiter_fifo = new sc_fifo<tlp_header>[n_vc][n_port];
//		for(int i=0; i<4; i++){
//			for(int j=0;j<4-1;j++)
//				port_arbiter_fifo[i][j] = new sc_fifo<tlp_header>(20);
//		}
		port_arbiter_fifo = new sc_fifo<tlp_header>**[n_vc];
		for(int i=0; i<n_vc; i++)	port_arbiter_fifo[i] = new sc_fifo<tlp_header>*[n_port];

		for(int i=0; i<n_vc; i++)
			for(int j=0;j<n_port-1;j++)
				port_arbiter_fifo[i][j] = new sc_fifo<tlp_header>(20);


		SC_THREAD(mapping_process);
		sensitive<<packet_from_switch;
		SC_THREAD(transmit_process);
		SC_THREAD(switch_read);
		sensitive<<switch_read_peq.get_event();
		SC_THREAD(switch_write);
		sensitive<<switch_write_peq.get_event();
		SC_THREAD(outside_write);
		sensitive<<outside_write_peq.get_event();
		SC_THREAD(not_empty_process);
		sensitive<<packet_outside;

		i_socket_to_outside.register_nb_transport_bw(this, &port_pcie::nb_transport_bw_outside);
		t_socket_to_switch.register_nb_transport_fw(this, &port_pcie::nb_transport_fw_switch);
		t_socket_to_outside.register_nb_transport_fw(this, &port_pcie::nb_transport_fw_outside);

	}


};




#endif /* PORT_H_ */
