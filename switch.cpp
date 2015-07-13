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


//void switch_pcie::npu_process(){
//	tlm_generic_payload trans;
//	tlm_sync_enum tlm_resp;
//	tlm_response_status tlm_stat;
//	sc_time delay;
//	tlm_phase phase;
//
//	while(true){
//		for(int index=0;index<n_ports;index++){
//			if(not_empty_interrupt[index]==false) continue;
//			cout<<endl<<"inside switch"<<endl;
//			trans.set_command(TLM_READ_COMMAND);
//			trans.set_response_status(TLM_GENERIC_ERROR_RESPONSE);
//			trans.set_data_ptr((unsigned char*) &header);
//			delay = SC_ZERO_TIME;
//			phase = BEGIN_REQ;
//			i_socket_0->nb_transport_fw(trans, phase, delay);
//			pending_transaction[index] = true;
//			wait(receive_event);
//			trans.set_command(TLM_WRITE_COMMAND);
//			trans.set_response_status(TLM_GENERIC_ERROR_RESPONSE);
//			delay = SC_ZERO_TIME;
//			phase = BEGIN_REQ;
//			i_socket_1->nb_transport_fw(trans, phase, delay);//for now random destination is chosen. it will be replace with routing funct
//			wait(receive_event);
//		}
//
////		}
//	}
//}

void switch_pcie::npu_process_0(){
	tlm_generic_payload trans;
	tlm_sync_enum tlm_resp;
	tlm_response_status tlm_stat;
	sc_time delay;
	tlm_phase phase;
	npu_header* header= new npu_header();

	while(true){

			if(pending_transaction[1] == true){
				wait(receive_event[1]);
				pending_transaction[1] = false;
				trans.clear_extension(header);
			}

			if(not_empty_interrupt[0]==0)
				wait(not_empty_interrupt[0].value_changed_event());
			cout<<endl<<"inside switch"<<endl;
			cout<<endl<<"extension port:"<<header->port<<endl;
			trans.set_command(TLM_READ_COMMAND);
			trans.set_response_status(TLM_GENERIC_ERROR_RESPONSE);
			//trans.set_data_ptr((unsigned char*) &header);
			trans.set_extension(header);
			delay = SC_ZERO_TIME;
			phase = BEGIN_REQ;
			i_socket_0->nb_transport_fw(trans, phase, delay);
			pending_transaction[0] = true;
			if(pending_transaction[0] == true){
				wait(receive_event[0]);
				pending_transaction[0] = false;
				cout<<endl<<"extension port:"<<header->port<<endl;
				trans.clear_extension(header);
				cout<<endl<<"extension port:"<<header->port<<endl;
			}
			trans.set_command(TLM_WRITE_COMMAND);
			trans.set_response_status(TLM_GENERIC_ERROR_RESPONSE);
			trans.set_extension(header);
			delay = SC_ZERO_TIME;
			phase = BEGIN_REQ;
			i_socket_1->nb_transport_fw(trans, phase, delay);//for now random destination is chosen. it will be replace with routing funct
			pending_transaction[1] = true;
//		}
	}
}

void switch_pcie::npu_process_1(){
	tlm_generic_payload trans;
	tlm_sync_enum tlm_resp;
	tlm_response_status tlm_stat;
	sc_time delay;
	tlm_phase phase;
	while(true){

			if(pending_transaction[2] == true){
				wait(receive_event[2]);
				pending_transaction[2] = false;
			}

			if(not_empty_interrupt[1]==0)
				wait(not_empty_interrupt[1].value_changed_event());
			cout<<endl<<"inside switch"<<endl;
			trans.set_command(TLM_READ_COMMAND);
			trans.set_response_status(TLM_GENERIC_ERROR_RESPONSE);
			trans.set_data_ptr((unsigned char*) &header);
			delay = SC_ZERO_TIME;
			phase = BEGIN_REQ;
			i_socket_1->nb_transport_fw(trans, phase, delay);
			pending_transaction[1] = true;
			if(pending_transaction[1] == true){
				wait(receive_event[1]);
				pending_transaction[1] = false;
			}
			trans.set_command(TLM_WRITE_COMMAND);
			trans.set_response_status(TLM_GENERIC_ERROR_RESPONSE);
			delay = SC_ZERO_TIME;
			phase = BEGIN_REQ;
			i_socket_2->nb_transport_fw(trans, phase, delay);//for now random destination is chosen. it will be replace with routing funct
			pending_transaction[2] = true;
//		}
	}
}

void switch_pcie::npu_process_2(){
	tlm_generic_payload trans;
	tlm_sync_enum tlm_resp;
	tlm_response_status tlm_stat;
	sc_time delay;
	tlm_phase phase;
	while(true){

			if(pending_transaction[0] == true){
				wait(receive_event[0]);
				pending_transaction[0] = false;
			}

			if(not_empty_interrupt[2]==0)
				wait(not_empty_interrupt[2].value_changed_event());
			cout<<endl<<"inside switch"<<endl;
			trans.set_command(TLM_READ_COMMAND);
			trans.set_response_status(TLM_GENERIC_ERROR_RESPONSE);
			trans.set_data_ptr((unsigned char*) &header);
			delay = SC_ZERO_TIME;
			phase = BEGIN_REQ;
			i_socket_2->nb_transport_fw(trans, phase, delay);
			pending_transaction[2] = true;
			if(pending_transaction[2] == true){
				wait(receive_event[2]);
				pending_transaction[2] = false;
			}
			trans.set_command(TLM_WRITE_COMMAND);
			trans.set_response_status(TLM_GENERIC_ERROR_RESPONSE);
			delay = SC_ZERO_TIME;
			phase = BEGIN_REQ;
			i_socket_0->nb_transport_fw(trans, phase, delay);//for now random destination is chosen. it will be replace with routing funct
			pending_transaction[0] = true;
//		}
	}
}

//tlm_sync_enum switch_pcie::nb_transport_bw(int index, tlm_generic_payload& transaction, tlm_phase& phase, sc_time& time){
//	if(phase == BEGIN_RESP)
//		cout<<endl<<sc_time_stamp()<<" producer: write confirmation coming" << endl;
//	else
//		cout<<endl<<sc_time_stamp()<<" producer: write not correctly confirmed" << endl;
//	//pending_transaction[index] = false;
//	receive_event[index].notify();
//	// finish the transaction (end of 2nd phase)
//	phase = END_RESP;
//	return TLM_COMPLETED;
//}

tlm_sync_enum switch_pcie::nb_transport_bw_0(tlm_generic_payload& transaction, tlm_phase& phase, sc_time& time){
	if(phase == BEGIN_RESP)
		cout<<endl<<sc_time_stamp()<<" switch 0: write confirmation coming" << endl;
	else
		cout<<endl<<sc_time_stamp()<<" producer: write not correctly confirmed" << endl;
	//pending_transaction[index] = false;
	receive_event[0].notify(time);
	// finish the transaction (end of 2nd phase)
	phase = END_RESP;
	return TLM_COMPLETED;
}

tlm_sync_enum switch_pcie::nb_transport_bw_1(tlm_generic_payload& transaction, tlm_phase& phase, sc_time& time){
	if(phase == BEGIN_RESP)
		cout<<endl<<sc_time_stamp()<<" switch 1: write confirmation coming" << endl;
	else
		cout<<endl<<sc_time_stamp()<<" producer: write not correctly confirmed" << endl;
	//pending_transaction[index] = false;
	receive_event[1].notify(time);
	// finish the transaction (end of 2nd phase)
	phase = END_RESP;
	return TLM_COMPLETED;
}

tlm_sync_enum switch_pcie::nb_transport_bw_2(tlm_generic_payload& transaction, tlm_phase& phase, sc_time& time){
	if(phase == BEGIN_RESP)
		cout<<endl<<sc_time_stamp()<<" switch 2: write confirmation coming" << endl;
	else
		cout<<endl<<sc_time_stamp()<<" producer: write not correctly confirmed" << endl;
	//pending_transaction[index] = false;
	receive_event[2].notify(time);
	cout<<endl<<"receive event notified"<<endl;
	// finish the transaction (end of 2nd phase)
	phase = END_RESP;
	return TLM_COMPLETED;
}




