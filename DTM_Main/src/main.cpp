#include "Beamalgo.hpp"
#include "Ritalgo.hpp"
#include "meu.hpp"
#include "SimpleField.hpp"
#include "SimpleStone.hpp"
#include "SimpleProblem.hpp"
#include "ExtendedField.hpp"
#include "ExtendedStone.hpp"
#include "BoostBitField.hpp"
#include "Client.hpp"
#include "Core.hpp"
#include <memory>
#include <boost/asio.hpp>

int main()
{

	//std::shared_ptr<Problem> prob(new SimpleProblem());
	// std::unique_ptr<Algorithm> solver(new Beamalgo<ExtendedField, ExtendedStone>(prob));
	// std::unique_ptr<Algorithm> rita(new Ritalgo<SimpleField, SimpleStone>(prob));
	// solver -> solve();
	// rita -> solve();
	//std::unique_ptr<Algorithm> meu(new Meu<SimpleField, SimpleStone>(prob));
	//meu->solve();

 // std::unique_ptr<FieldEX<boost::multiprecision::uint1024_t>> fx(new BoostBitField(prob->get_field_str()));
  //boost::asio::io_service io_service;

  //tcp::resolver resolver(io_service);
  //tcp::resolver::query query(tcp::v4(), "172.26.32.189", "8080");
  //tcp::resolver::iterator iterator = resolver.resolve(query);

  //tcp::socket s(io_service);
  //boost::asio::connect(s, iterator);

  //std::cout << "Enter message: ";
  //char request[1024];
  //std::cin.getline(request, 1024);
  //size_t request_length = strlen(request);
  //boost::asio::write(s, boost::asio::buffer(request, request_length));

  boost::asio::io_service ios;
  /**/
  Client cl(ios, "192.168.1.219", 8080);
  cl.connect();
  cl.send("ping");
  /**/

  return 0;
}

