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
	sc_signal<bool> not_empty_interrupt[3];
	cout<<"1\n\r"<<endl;
	endpoint* ep1 = new endpoint("endpoint1",100, 5, 2,1);
	endpoint* ep2 = new endpoint("endpoint2",6000, 5, 2,2);
	endpoint* ep3 = new endpoint("endpoint3",6000, 5, 2,3);
	cout<<"2\n\r"<<endl;
	port_pcie* port1 = new port_pcie("port1", n_vc, n_port, 1);
	port_pcie* port2 = new port_pcie("port2", n_vc, n_port, 2);
	port_pcie* port3 = new port_pcie("port3", n_vc, n_port, 3);
	cout<<"3\n\r"<<endl;
	switch_pcie* switch1 = new switch_pcie("switch1", n_port, n_vc);
	//binding sockets
	cout<<"4\n\r"<<endl;
	ep1->i_socket.bind(port1->t_socket_to_outside);
	ep2->i_socket.bind(port2->t_socket_to_outside);
	ep3->i_socket.bind(port3->t_socket_to_outside);
//
	port1->i_socket_to_outside.bind(ep1->t_socket);
	port2->i_socket_to_outside.bind(ep2->t_socket);
	port3->i_socket_to_outside.bind(ep3->t_socket);
//
	switch1->i_socket_0.bind(port1->t_socket_to_switch);
	switch1->i_socket_1.bind(port2->t_socket_to_switch);
	switch1->i_socket_2.bind(port3->t_socket_to_switch);


	switch1->not_empty_interrupt[0](not_empty_interrupt[0]);
	port1->not_empty_interrupt(not_empty_interrupt[0]);
	switch1->not_empty_interrupt[1](not_empty_interrupt[1]);
	port2->not_empty_interrupt(not_empty_interrupt[1]);
	switch1->not_empty_interrupt[2](not_empty_interrupt[2]);
	port3->not_empty_interrupt(not_empty_interrupt[2]);
	cout<<"5\n\r"<<endl;
	//setting simulation kernel
	sc_time sim_dur = sc_time(450, SC_NS);
	if (argc != 2) {
		cout << "Default simulation time = " << sim_dur << endl;
	} else {
		sim_dur = sc_time(atoi(argv[1]), SC_NS);
	}
	cout<<"6\n\r"<<endl;
	// start simulation
	sc_start(sim_dur);


	return 0;
}

