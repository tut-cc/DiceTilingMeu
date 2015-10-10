#include "Beamalgo.hpp"
#include "Ritalgo.hpp"
#include "meu.hpp"
#include "SimpleField.hpp"
#include "SimpleStone.hpp"
#include "SampleProblem.hpp"
#include "ProductionProblem.hpp"
#include "ExtendedField.hpp"
#include "ExtendedStone.hpp"
#include "Client.hpp"
#include "Core.hpp"
#include <memory>
#include <boost/asio.hpp>

int main(const int argc, const char **argv)
{
	if (argc == 1) {
		std::shared_ptr<Problem> prob(new SampleProblem());
		std::unique_ptr<Algorithm> beam(new Beamalgo<ExtendedField, ExtendedStone>(prob));
		beam->solve();
	}
	else {
		std::unique_ptr<Core> core(new Core(argv[1]));
		const std::string probs = core->get();
		std::shared_ptr<Problem> prob(new ProductionProblem(probs));
		std::unique_ptr<Algorithm> beam(new Beamalgo<ExtendedField, ExtendedStone>(prob, std::move(core)));
		beam->solve();
	}
	return 0;

  return 0;
}

