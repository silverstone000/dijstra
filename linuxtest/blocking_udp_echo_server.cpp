//
// blocking_udp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <stdlib.h>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <string>

#include "icmp_header.hpp"

using namespace std;
using boost::asio::ip::udp;

enum { max_length = 1024 };

void server(boost::asio::io_context& io_context, unsigned short port)
{
  udp::socket sock(io_context, udp::endpoint(udp::v4(), port));

  std::cout << "port number:" << sock.local_endpoint().port() << std::endl;


  enum {
	  echo_reply = 0, destination_unreachable = 3, source_quench = 4,
	  redirect = 5, echo_request = 8, time_exceeded = 11, parameter_problem = 12,
	  timestamp_request = 13, timestamp_reply = 14, info_request = 15,
	  info_reply = 16, address_request = 17, address_reply = 18
  };

  for (;;)
  {
    char data[max_length];
    udp::endpoint sender_endpoint;

	
    size_t length = sock.receive_from(
        boost::asio::buffer(data, max_length), sender_endpoint);

	data[0] = echo_reply;

    sock.send_to(boost::asio::buffer(data, length), sender_endpoint);
	this_thread::yield();
  }
}

struct testst
{
	enum {
		echo_reply = 0, destination_unreachable = 3, source_quench = 4,
		redirect = 5, echo_request = 8, time_exceeded = 11, parameter_problem = 12,
		timestamp_request = 13, timestamp_reply = 14, info_request = 15,
		info_reply = 16, address_request = 17, address_reply = 18
	};
	int type;
	int cont;
};


int mainudpe(int argc, char* argv[])
{
  //try
  //{
  //  if (argc != 2)
  //  {
  //    std::cerr << "Usage: blocking_udp_echo_server <port>\n";
  //    return 1;
  //  }

  //  boost::asio::io_context io_context;

  //  server(io_context, std::atoi(argv[1]));
  //}
  //catch (std::exception& e)
  //{
  //  std::cerr << "Exception: " << e.what() << "\n";
  //}
	testst st;
	st.type = testst::address_reply;
	st.cont = 1;
	cout << "value of type: " << st.type << endl;
	cout << "value of enum: " << st.address_reply << endl;

	sleep(5);

  return 0;
}
