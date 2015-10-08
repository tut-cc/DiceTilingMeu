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
  boost::asio::io_service io_service;
  Client cl(io_service, "172.26.32.189", 65534);
  cl.connect();
  cl.send("ping");
	return 0;
}

