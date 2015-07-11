/*
 * processor_core.h
 *
 *  Created on: Jun 13, 2015
 *      Author: hooman
 */

#ifndef PROCESSOR_CORE_H_
#define PROCESSOR_CORE_H_

#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlp_messages.h"

using namespace sc_core;
using namespace tlm;
using namespace tlm_utils;

#define SC_INCLUDE_DYNAMIC_PROCESSES


SC_MODULE(processor_core){

private:

public:
processor_core(sc_module_name name_,int clk_frequency,short core_index,int request_rate,int ID, sc_mutex& mutex):sc_module(name_){



}

};



#endif /* PROCESSOR_CORE_H_ */
