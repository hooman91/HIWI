/*
 * port.cpp
 *
 *  Created on: Jun 21, 2015
 *      Author: hooman
 */
#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "systemc"
#include "tlm.h"
#include "port.h"
#include "tlp_messages.h"
using namespace std;
using namespace sc_core;


int port_pcie::tc_vc_mapper(int tc){
	int temp = ceil(float (8 / n_vc));
	return tc/temp;
}


void port_pcie::port_arbiter(int index){
	int available[n_port];
	int j=0;
	srand (time(NULL));
	for(int i=0;i++;i<n_port){
		if (port_arbiter_fifo[index][i]->num_available()!=0){
//			vc_n_empty[index] = true;
			available[j]=i;
			j++;
		}
	}
	if(j==0)
		port_arbitration_result[index] = -1;//indicating empty fifo
	else
		port_arbitration_result[index] = available[rand()%j];
}
void port_pcie::vc_arbiter(){
	int available_vcs[n_vc];
	int j=0;
	for(int i=0;i<n_vc;i++){
		if(port_arbitration_result[i] == -1){
			cout<<endl<<"no more packets"<<endl;
			vc_arbitration_result = -1;
			return;
		}
		else{
			available_vcs[j] = i;
			j++;
		}
	}

	vc_arbitration_result = available_vcs[rand()%j];
}
////these to functions perfomr arbitration and transmit on egress port
void port_pcie::mapping_process(){
	while(true){
		wait();
		npu_header* header;
		from_switch_fifo.nb_read(*header);
		int vc = tc_vc_mapper(header->header->dw0.TC);
		port_arbiter_fifo[vc][header->port]->write(*(header->header));
		//port_arbiter(vc);
	}
}
void port_pcie::transmit_process(){
	tlm_generic_payload trans;
	tlm_sync_enum tlm_resp;
	tlm_response_status tlm_stat;
	sc_time delay;
	tlm_phase phase;
	tlp_header header;
	while(true){
		if(pending_transaction==true)	wait();   ///not sure about it
		else{
			for (int i=0;i<n_vc;i++)	port_arbiter(i);
			do	vc_arbiter();	while(vc_arbitration_result==-1);
			trans.set_command(TLM_WRITE_COMMAND);
			trans.set_data_ptr((unsigned char*) &port_arbiter_fifo[vc_arbitration_result][port_arbitration_result[vc_arbitration_result]]);
			trans.set_response_status(TLM_GENERIC_ERROR_RESPONSE);
			delay = SC_ZERO_TIME;
			phase = BEGIN_REQ;
			i_socket_to_outside->nb_transport_fw(trans, phase, delay);
			pending_transaction = true;
		}
	}

}
/////////////////////////////////////////////////////////////////////////////////////

tlm_sync_enum port_pcie::nb_transport_bw_outside(tlm_generic_payload& payload, tlm_phase& phase, sc_time&delay_time){
	if(phase == BEGIN_RESP)
		cout<<endl<<sc_time_stamp()<<" producer: write confirmation coming" << endl;
	else
		cout<<endl<<sc_time_stamp()<<" producer: write not correctly confirmed" << endl;
	pending_transaction = false;
	// finish the transaction (end of 2nd phase)
	phase = END_RESP;
	return TLM_COMPLETED;
}
tlm_sync_enum port_pcie::nb_transport_fw_switch(tlm_generic_payload& payload, tlm_phase& phase, sc_time& delay_time){
	if(phase != BEGIN_REQ){
		cout << sc_time_stamp()<<" ERROR: nb_transport_fw call with phase!=BEGIN_REQ" << endl;
		exit(1);
	}
//	npu_header* header;
//	header = (npu_header*) payload.get_data_ptr();
	if(payload.get_command()==TLM_READ_COMMAND){
//		to_switch_fifo.nb_read(*header);
		switch_read_peq.notify(payload, sc_time(10, SC_NS));
		payload.set_response_status(TLM_OK_RESPONSE);
	}
	else if(payload.get_command()==TLM_WRITE_COMMAND){
//		from_switch_fifo.nb_write(*header);
		switch_write_peq.notify(payload, sc_time(10, SC_NS));
		payload.set_response_status(TLM_OK_RESPONSE);
	}

	phase = END_REQ;
	return TLM_UPDATED;
}
tlm_sync_enum port_pcie::nb_transport_fw_outside(tlm_generic_payload& payload, tlm_phase& phase, sc_time&delay_time){
	if(phase != BEGIN_REQ){
		cout << sc_time_stamp()<<" ERROR: nb_transport_fw call with phase!=BEGIN_REQ" << endl;
		exit(1);
	}

	outside_write_peq.notify(payload, sc_time(10, SC_NS));
	phase = END_REQ;
	return TLM_UPDATED;
}

void port_pcie::not_empty_process(){
	while(true){
		wait();
		if(to_switch_fifo.num_available()>0)
			not_empty_interrupt=1;
		else
			not_empty_interrupt=0;
	}
}
void port_pcie::switch_read(){
	tlm_sync_enum tlm_resp;
	tlm_response_status status;
	sc_time delay;
	tlm_phase phase;
	tlm_generic_payload *payload;
	while(true){
		wait();
		payload = switch_read_peq.get_next_transaction();
		npu_header* header;
		to_switch_fifo.nb_read(*header);
		payload->set_data_ptr((unsigned char*) header);
		phase = BEGIN_RESP;
		delay = sc_time(10, SC_NS);
		// call nb_transport_bw
		tlm_resp = t_socket_to_switch->nb_transport_bw(*payload, phase, delay);
		if(tlm_resp != TLM_COMPLETED || phase != END_RESP)
		{
			cout<<endl<<sc_time_stamp()<<" fifo: write response not appropriately completed" << endl;
		}

	}
}
void port_pcie::switch_write(){
	tlm_sync_enum tlm_resp;
	tlm_response_status status;
	sc_time delay;
	tlm_phase phase;
	tlm_generic_payload *payload;
	while(true){
		wait();
		npu_header* header;
		payload = switch_read_peq.get_next_transaction();
		header = (npu_header*) payload->get_data_ptr();
		from_switch_fifo.nb_write(*header);
		phase = BEGIN_RESP;
		delay = SC_ZERO_TIME;
		// call nb_transport_bw
		tlm_resp = t_socket_to_switch->nb_transport_bw(*payload, phase, delay);
		packet_from_switch.notify();
		if(tlm_resp != TLM_COMPLETED || phase != END_RESP)
		{
			cout<<endl<<sc_time_stamp()<<" fifo: write response not appropriately completed" << endl;
		}
	}
}
void port_pcie::outside_write(){
	tlm_sync_enum tlm_resp;
	tlm_response_status status;
	sc_time delay;
	tlm_phase phase;
	tlm_generic_payload *payload;
	while(true){
		wait();
		npu_header* header;
		payload = outside_write_peq.get_next_transaction();
		header->header = (tlp_header*) payload->get_data_ptr();
		header->port = this->port_index;
		to_switch_fifo.nb_write(*header);
		packet_outside.notify();
		phase = BEGIN_RESP;
		delay = SC_ZERO_TIME;
		// call nb_transport_bw
		tlm_resp = t_socket_to_outside->nb_transport_bw(*payload, phase, delay);
		if(tlm_resp != TLM_COMPLETED || phase != END_RESP)
		{
			cout<<endl<<sc_time_stamp()<<" fifo: write response not appropriately completed" << endl;
		}
	}
}
