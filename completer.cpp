/*
 * completer.cpp
 *
 *  Created on: Jun 6, 2015
 *      Author: hooman
 */
#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "completer.h"
#include "tlp_messages.h"
using namespace std;




tlm_sync_enum completer::nb_transport_fw( tlm_generic_payload  &payload, tlm_phase	&phase, sc_time	&delay_time){

	if(phase != BEGIN_REQ){
		cout << sc_time_stamp()<<" ERROR: nb_transport_fw call with phase!=BEGIN_REQ" << endl;
		exit(1);
	}
	tlp_header* header;
	header = (tlp_header*) payload.get_data_ptr();
	//delay calculation
	delay_time += sc_time((int)header->dw0.Length, SC_NS);
//	delay_time += sc_time(20, SC_NS);
	tlm_command cmd = payload.get_command();
	if(cmd == TLM_WRITE_COMMAND){
		w_peq.notify(payload,delay_time);
	}
	else if(cmd == TLM_READ_COMMAND){
		r_peq.notify(payload,delay_time);
	}

	phase = END_REQ;
	return TLM_UPDATED;
}

void completer::do_read(){
	tlm_sync_enum tlm_resp;
	tlm_response_status status;
	sc_time delay;
	tlm_phase phase;
	tlm_generic_payload *payload;

	while(true){
		wait();
		cout<<endl<<sc_time_stamp()<<" read operation in "<<this->name()<<endl;
		//payload->set_data_length(len);
		//payload->set_response_status(status);
		phase = BEGIN_RESP;
		delay = SC_ZERO_TIME;

		// call nb_transport_bw
		tlm_resp = t_socket->nb_transport_bw(*payload, phase, delay);
		if(tlm_resp != TLM_COMPLETED || phase != END_RESP)
		{
			cout<<endl<<sc_time_stamp()<<" fifo: write response not appropriately completed" << endl;
		}
	}
}
void completer::do_write(){
	tlm_sync_enum tlm_resp;
	tlm_response_status status;
	sc_time delay;
	tlm_phase phase;
	tlm_generic_payload* payload;

	while(true){
		wait();


		phase = BEGIN_RESP;
		delay = SC_ZERO_TIME;
		payload = w_peq.get_next_transaction();
		// call nb_transport_bw
		mutex.lock();
		tlm_resp = t_socket->nb_transport_bw(*payload, phase, delay);
		mutex.unlock();
		if(tlm_resp != TLM_COMPLETED || phase != END_RESP)
		{
			cout<<endl<<sc_time_stamp()<<" fifo: write response not appropriately completed" << endl;
		}
	}
}


