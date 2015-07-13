/*
 * endpoint.cpp
 *
 *  Created on: Jun 6, 2015
 *      Author: hooman
 */
#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "endpoint.h"
#include "tlp_messages.h"
using namespace std;

void endpoint::send_procedure(){
	tlm_generic_payload trans;
	tlm_sync_enum tlm_resp;
	tlm_response_status tlm_stat;
	sc_time delay;
	tlm_phase phase;
	tlp_header* header= new tlp_header();


	bool pending_transaction = false;

	while(true){
		if(pending_transaction){
			wait(response_event);
			pending_transaction = false;
			trans.clear_extension(header);

		}
		wait(send_event);
		//generate tlp header
		//send packet calling fw function
		//check the result
		header->dw0.Length= (short) rand()%128;//????
		trans.set_command(TLM_WRITE_COMMAND);
		trans.set_data_length(header->dw0.Length);
//		trans.set_data_ptr((unsigned char*) &header); //I am not sure if this is gonna work! :D
		trans.set_response_status(TLM_GENERIC_ERROR_RESPONSE);
		trans.set_extension(header);
		delay = SC_ZERO_TIME;
		phase = BEGIN_REQ;

		tlm_resp = i_socket->nb_transport_fw(trans, phase, delay);



		pending_transaction = true;
		if(tlm_resp != TLM_UPDATED || phase != END_REQ)
		{
			std::cout<<std::endl<<sc_time_stamp()<<" endpoint: write request not appropriately completed" << std::endl;
		}
	}


}

void endpoint::send_trigger(){
	while(true){
		wait(request_rate, SC_NS);
		send_event.notify();
		cout<<endl<<"event:send_event"<<endl;
	}
}

tlm_sync_enum endpoint::nb_transport_fw( tlm_generic_payload  &payload, tlm_phase	&phase, sc_time	&delay_time){

	if(phase != BEGIN_REQ){
		cout << sc_time_stamp()<<" ERROR: nb_transport_fw call with phase!=BEGIN_REQ" << endl;
		exit(1);
	}
	tlp_header* header;
	header = (tlp_header*) payload.get_data_ptr();
	//delay calculation
	delay_time += sc_time((int)header->dw0.Length, SC_NS);
	cout<<endl<<"length:"<<header->dw0.Length<<endl;
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


tlm_sync_enum endpoint::nb_transport_bw( tlm_generic_payload  &payload, tlm_phase	&phase, sc_time	&delay_time){
	if(phase == BEGIN_RESP)
		cout<<endl<<sc_time_stamp()<<"   end-point"<<this->ID<<": write confirmation coming" << endl;
	else
		cout<<endl<<sc_time_stamp()<<" producer: write not correctly confirmed" << endl;


	//delay_time += sc_time(50, SC_NS);

	// consume process should go on after time needed for the confirmation
	response_event.notify(delay_time);

	// finish the transaction (end of 2nd phase)
	phase = END_RESP;
	return TLM_COMPLETED;
}
void endpoint::do_read(){
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
		payload = w_peq.get_next_transaction();
		// call nb_transport_bw
		tlm_resp = t_socket->nb_transport_bw(*payload, phase, delay);
		if(tlm_resp != TLM_COMPLETED || phase != END_RESP)
		{
			cout<<endl<<sc_time_stamp()<<" fifo: write response not appropriately completed" << endl;
		}
	}
}
void endpoint::do_write(){
	tlm_sync_enum tlm_resp;
	tlm_response_status status;
	sc_time delay;
	tlm_phase phase;
	tlm_generic_payload *payload;

	while(true){
		wait();
		cout<<endl<<sc_time_stamp()<<" write operation in "<<this->name()<<endl;

		phase = BEGIN_RESP;
		delay = SC_ZERO_TIME;
		payload = w_peq.get_next_transaction();
		// call nb_transport_bw
		tlm_resp = t_socket->nb_transport_bw(*payload, phase, delay);
		if(tlm_resp != TLM_COMPLETED || phase != END_RESP)
		{
			cout<<endl<<sc_time_stamp()<<" fifo: write response not appropriately completed" << endl;
		}
	}
}


