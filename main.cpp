#define SC_INCLUDE_DYNAMIC_PROCESSES

#include "systemc"
#include "endpoint.h"
#include "root_complex.h"
#include "root_complex_simple.h"
#include "completer.h"
#include "switch.h"
#include "port.h"



int sc_main(int argc, char* argv[]){
	//system parameters
	int n_vc = 3;
	int n_port =3;
	//instantiating modules
	endpoint* ep1 = new endpoint("endpoint1",55, 5, 2,222);
	endpoint* ep2 = new endpoint("endpoint2",35, 5, 2,222);
	endpoint* ep3 = new endpoint("endpoint2",45, 5, 2,222);

	port_pcie* port1 = new port_pcie("port1", n_vc, n_port, 1);
	port_pcie* port2 = new port_pcie("port2", n_vc, n_port, 2);
	port_pcie* port3 = new port_pcie("port3", n_vc, n_port, 3);

	switch_pcie* switch1 = new switch_pcie("switch1", n_port, n_vc);
	//binding sockets
	ep1->i_socket.bind(port1->t_socket_to_outside);
	ep2->i_socket.bind(port2->t_socket_to_outside);
	ep3->i_socket.bind(port3->t_socket_to_outside);
//
	port1->i_socket_to_outside.bind(ep1->t_socket);
	port2->i_socket_to_outside.bind(ep1->t_socket);
	port3->i_socket_to_outside.bind(ep1->t_socket);
//
	switch1->i_socket[0]->bind(port1->t_socket_to_switch);
	switch1->i_socket[1]->bind(port2->t_socket_to_switch);
	switch1->i_socket[2]->bind(port3->t_socket_to_switch);
	//setting simulation kernel
	sc_time sim_dur = sc_time(4500, SC_NS);
	if (argc != 2) {
		cout << "Default simulation time = " << sim_dur << endl;
	} else {
		sim_dur = sc_time(atoi(argv[1]), SC_NS);
	}

	// start simulation
	sc_start(sim_dur);


	return 0;
}

