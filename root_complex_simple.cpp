/*
 * root_complex_simple.cpp
 *
 *  Created on: Jun 14, 2015
 *      Author: hooman
 */


#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "systemc"
#include "tlm.h"
#include "root_complex_simple.h"
#include "tlp_messages.h"

using namespace sc_core;
//using namespace tlm;
//using namespace tlm_utils;


void root_complex_simple::processor_core_0(){

	tlm_generic_payload trans;
	tlm_sync_enum tlm_resp;
	tlm_response_status tlm_stat;
	sc_time delay;
	tlm_phase phase;
	tlp_header header;

	bool pending_transaction = false;

	while(true){
		wait(root_complex_simple::request_rate[0],SC_NS);

		if(pending_transaction){
			wait(response_event[0]);
			pending_transaction = false;
		}

		header.dw0.Length= (short) 15;//???? assigned for test purpose
		header.dw1.RequesterID = 0;
		trans.set_command(TLM_WRITE_COMMAND);
		trans.set_data_length(header.dw0.Length);
		trans.set_data_ptr((unsigned char*) &header);
		trans.set_response_status(TLM_GENERIC_ERROR_RESPONSE);

		delay = SC_ZERO_TIME;
		phase = BEGIN_REQ;
		mutex.lock();
		tlm_resp = i_socket->nb_transport_fw(trans, phase, delay);
		mutex.unlock();
		pending_transaction = true;
		if(tlm_resp != TLM_UPDATED || phase != END_REQ)
		{
			std::cout<<std::endl<<sc_time_stamp()<<" core"<<10<<":"<< "write request not appropriately completed" << std::endl;
		}
		else	cout<<endl<<sc_time_stamp()<<":	core"<<0<<":write cmd is sent with header length: "<<header.dw0.Length<<endl;

	}


}

void root_complex_simple::processor_core_1(){

	tlm_generic_payload trans;
	tlm_sync_enum tlm_resp;
	tlm_response_status tlm_stat;
	sc_time delay;
	tlm_phase phase;
	tlp_header header;
	cout<<endl<<"hehehehehe"<<root_complex_simple::request_rate[1]<<endl;
	bool pending_transaction = false;

	while(true){
		wait(root_complex_simple::request_rate[2],SC_NS);

		if(pending_transaction){
			wait(response_event[1]);

			pending_transaction = false;
		}

		header.dw0.Length= (short) 15;//???? assigned for test purpose
		header.dw1.RequesterID = 1;
		trans.set_command(TLM_WRITE_COMMAND);
		trans.set_data_length(header.dw0.Length);
		trans.set_data_ptr((unsigned char*) &header);
		trans.set_response_status(TLM_GENERIC_ERROR_RESPONSE);

		delay = SC_ZERO_TIME;
		phase = BEGIN_REQ;
		mutex.lock();
		tlm_resp = i_socket->nb_transport_fw(trans, phase, delay);
		mutex.unlock();
		pending_transaction = true;
		if(tlm_resp != TLM_UPDATED || phase != END_REQ)
		{
			std::cout<<std::endl<<sc_time_stamp()<<" core"<<11<<":"<< "write request not appropriately completed" << std::endl;
		}
		else	cout<<endl<<sc_time_stamp()<<":	core"<<1<<":write cmd is sent with header length: "<<header.dw0.Length<<endl;

	}


}

void root_complex_simple::processor_core_2(){

	tlm_generic_payload trans;
	tlm_sync_enum tlm_resp;
	tlm_response_status tlm_stat;
	sc_time delay;
	tlm_phase phase;
	tlp_header header;

	bool pending_transaction = false;

	while(true){
		wait(root_complex_simple::request_rate[2],SC_NS);

		if(pending_transaction){
			wait(response_event[2]);

			pending_transaction = false;
		}

		header.dw0.Length= (short) 15;//???? assigned for test purpose
		header.dw1.RequesterID = 2;
		trans.set_command(TLM_WRITE_COMMAND);
		trans.set_data_length(header.dw0.Length);
		trans.set_data_ptr((unsigned char*) &header);
		trans.set_response_status(TLM_GENERIC_ERROR_RESPONSE);

		delay = SC_ZERO_TIME;
		phase = BEGIN_REQ;
		mutex.lock();
		tlm_resp = i_socket->nb_transport_fw(trans, phase, delay);
		mutex.unlock();
		pending_transaction = true;
		if(tlm_resp != TLM_UPDATED || phase != END_REQ)
		{
			std::cout<<std::endl<<sc_time_stamp()<<" core"<<12<<":"<< "write request not appropriately completed" << std::endl;
		}
		else	cout<<endl<<sc_time_stamp()<<":	core"<<2<<":write cmd is sent with header length: "<<header.dw0.Length<<endl;

	}


}

void root_complex_simple::processor_core_3(){

	tlm_generic_payload trans;
	tlm_sync_enum tlm_resp;
	tlm_response_status tlm_stat;
	sc_time delay;
	tlm_phase phase;
	tlp_header header;

	bool pending_transaction = false;

	while(true){
		wait(root_complex_simple::request_rate[3],SC_NS);

		if(pending_transaction){
			wait(response_event[3]);

			pending_transaction = false;
		}

		header.dw0.Length= (short) 15;//???? assigned for test purpose
		header.dw1.RequesterID = 3;
		trans.set_command(TLM_WRITE_COMMAND);
		trans.set_data_length(header.dw0.Length);
		trans.set_data_ptr((unsigned char*) &header);
		trans.set_response_status(TLM_GENERIC_ERROR_RESPONSE);

		delay = SC_ZERO_TIME;
		phase = BEGIN_REQ;
		mutex.lock();
		tlm_resp = i_socket->nb_transport_fw(trans, phase, delay);
		mutex.unlock();
		pending_transaction = true;
		if(tlm_resp != TLM_UPDATED || phase != END_REQ)
		{
			std::cout<<std::endl<<sc_time_stamp()<<" core"<<13<<":"<< "write request not appropriately completed" << std::endl;
		}
		else	cout<<endl<<sc_time_stamp()<<":	core"<<3<<":write cmd is sent with header length: "<<header.dw0.Length<<endl;

	}


}

tlm_sync_enum  root_complex_simple::nb_transport_bw
( tlm_generic_payload  &payload,         // ref to payload
  tlm_phase            &phase,		 // ref to phase
  sc_time              &delay_time       // ref to delay time
){
	tlp_header* header = (tlp_header*) payload.get_data_ptr();

	if(phase == BEGIN_RESP){
//			cout<<endl<<sc_time_stamp()<<" producer: write confirmation coming" << endl;
			cout<<endl<<sc_time_stamp()<<":	packet successfully written from ID:"<<header->dw1.RequesterID<<endl;
	}
	else
		cout<<endl<<sc_time_stamp()<<" producer: write not correctly confirmed" << endl;

	// consume process should go on after time needed for the confirmation
	response_event[header->dw1.RequesterID].notify();
	// finish the transaction (end of 2nd phase)
	phase = END_RESP;
	return TLM_COMPLETED;
}


