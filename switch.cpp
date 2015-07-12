/*
 * switch.cpp
 *
 *  Created on: Jun 17, 2015
 *      Author: hooman
 */
#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "systemc"
#include "tlm.h"
#include "switch.h"
#include "tlp_messages.h"
using namespace std;
using namespace sc_core;


void switch_pcie::npu_process(){
	tlm_generic_payload trans;
	tlm_sync_enum tlm_resp;
	tlm_response_status tlm_stat;
	sc_time delay;
	tlm_phase phase;

	while(true){
//		if(not_empty_interrupt==false)
//			wait(not_empty_interrupt->value_changed_event());
//		if(pending_transaction[index]==true)	wait();   ///not sure about it
//		else{
		for(int index=0;index<n_ports;index++){
			if(not_empty_interrupt[index]==false) continue;
			trans.set_command(TLM_READ_COMMAND);
			trans.set_response_status(TLM_GENERIC_ERROR_RESPONSE);
			trans.set_data_ptr((unsigned char*) &header);
			delay = SC_ZERO_TIME;
			phase = BEGIN_REQ;
			i_socket_0->nb_transport_fw(trans, phase, delay);
			pending_transaction[index] = true;
			wait(receive_event);
			trans.set_command(TLM_WRITE_COMMAND);
			trans.set_response_status(TLM_GENERIC_ERROR_RESPONSE);
			delay = SC_ZERO_TIME;
			phase = BEGIN_REQ;
			i_socket_0->nb_transport_fw(trans, phase, delay);//for now random destination is chosen. it will be replace with routing funct
			wait(receive_event);
		}

//		}
	}
}

tlm_sync_enum switch_pcie::nb_transport_bw(int index, tlm_generic_payload& transaction, tlm_phase& phase, sc_time& time){
	if(phase == BEGIN_RESP)
		cout<<endl<<sc_time_stamp()<<" producer: write confirmation coming" << endl;
	else
		cout<<endl<<sc_time_stamp()<<" producer: write not correctly confirmed" << endl;
	pending_transaction[index] = false;
	receive_event.notify();
	// finish the transaction (end of 2nd phase)
	phase = END_RESP;
	return TLM_COMPLETED;
}




