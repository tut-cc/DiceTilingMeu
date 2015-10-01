#include "Beamalgo.hpp"
#include "Ritalgo.hpp"
#include "meu.hpp"
#include "SimpleField.hpp"
#include "SimpleStone.hpp"
#include "SimpleProblem.hpp"
#include "SampleProblem.hpp"
#include "ExtendedField.hpp"
#include "ExtendedStone.hpp"
#include "BoostBitField.hpp"
#include <memory>

int main()
{

	std::shared_ptr<Problem> prob(new SampleProblem());
	std::unique_ptr<Algorithm> solver(new Beamalgo<ExtendedField, ExtendedStone>(prob));
	//std::unique_ptr<Algorithm> rita(new Ritalgo<SimpleField, SimpleStone>(prob));
	solver -> solve();
	//rita -> solve();
	//std::unique_ptr<Algorithm> meu(new Meu<SimpleField, SimpleStone>(prob));
	//meu->solve();

  //std::unique_ptr<FieldEX<boost::multiprecision::uint1024_t>> fx(new BoostBitField(prob->get_field_str()));
	return 0;
}

