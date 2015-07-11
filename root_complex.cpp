/*
 * root_complex.cpp
 *
 *  Created on: Jun 14, 2015
 *      Author: hooman
 */


#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "systemc"
#include "tlm.h"
#include "root_complex.h"
#include "tlp_messages.h"

using namespace sc_core;
//using namespace tlm;
//using namespace tlm_utils;


void root_complex::processor_core(int index){

	tlm_generic_payload trans;
	tlm_sync_enum tlm_resp;
	tlm_response_status tlm_stat;
	sc_time delay;
	tlm_phase phase;
	tlp_header header;

	bool pending_transaction = false;
	cout<<endl<<"index :	"<<index<<"	rate:	"<<request_rate[index]<<endl;
	while(true){
		wait(root_complex::request_rate[index],SC_NS);

		if(pending_transaction){
			wait(response_event[index]);
			pending_transaction = false;
		}

		header.dw0.Length= (short) 20+index;//???? assigned for test purpose
		header.dw1.RequesterID = index;
		trans.set_command(TLM_WRITE_COMMAND);
		trans.set_data_length(header.dw0.Length);
		trans.set_data_ptr((unsigned char*) &header);
		trans.set_response_status(TLM_GENERIC_ERROR_RESPONSE);

		delay = SC_ZERO_TIME;
		phase = BEGIN_REQ;
//		mutex.lock();
		tlm_resp = i_socket->nb_transport_fw(trans, phase, delay);
//		mutex.unlock();
		pending_transaction = true;
		if(tlm_resp != TLM_UPDATED || phase != END_REQ)
		{
			std::cout<<std::endl<<sc_time_stamp()<<" core"<<index<<":"<< "write request not appropriately completed" << std::endl;
		}
		else	cout<<endl<<sc_time_stamp()<<":		write cmd is sent with header length: "<<header.dw0.Length<<endl;

	}


}

tlm_sync_enum  root_complex::nb_transport_bw
( tlm_generic_payload  &payload,         // ref to payload
  tlm_phase            &phase,		 // ref to phase
  sc_time              &delay_time       // ref to delay time
){
	tlp_header* header = (tlp_header*) payload.get_data_ptr();

	if(phase == BEGIN_RESP){
//			cout<<endl<<sc_time_stamp()<<" producer: write confirmation coming" << endl;
			cout<<endl<<sc_time_stamp()<<":		length:"<<header->dw0.Length<<endl;
	}
	else
		cout<<endl<<sc_time_stamp()<<" producer: write not correctly confirmed" << endl;


	//delay_time += sc_time(50, SC_NS);
	// consume process should go on after time needed for the confirmation
	response_event[header->dw1.RequesterID].notify();
	// finish the transaction (end of 2nd phase)
	phase = END_RESP;
	return TLM_COMPLETED;
}

void root_complex::runner(){
	sc_process_handle handle[root_complex::number_of_cores];
	for(int i=0;i<root_complex::number_of_cores;i++){
				handle[i] = sc_spawn(sc_bind(&root_complex::processor_core, this, i));
			}

}
