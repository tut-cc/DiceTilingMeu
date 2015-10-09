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

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

const int max_length = 1024;

void uploadFile() {
	try {
		cURLpp::Cleanup myCleanup;

		cURLpp::Easy myRequest;
		using namespace cURLpp::Options;
		// 出力先の指定
		myRequest.setOpt(new WriteStream(&std::cout));
		myRequest.setOpt(new Url("testform26.procon-online.net"));
		myRequest.perform();

	}
	catch (cURLpp::RuntimeError &e) {
		std::cout << e.what() << std::endl;
	}
	catch (cURLpp::LogicError &e) {
		std::cout << e.what() << std::endl;
	}
	//char * url = "testform26.procon-online.net";
	//char * filename = "answer.txt";
	//char * token = "0123456789abcdef";
	//cURLpp::Easy request;

	//// ファイルサイズの取得
	//struct stat st;
	//if (stat(filename, &st) < 0) {
	//	perror("fstat");
	//	exit(1);
	//}

	//// ヘッダの作成．ヘッダはstd::list<std::string> として作る．
	//char buf[100];
	//std::list< std::string > headers;
	//headers.push_back("Content - Disposition: form - data; name = \"answer\"; filename = \"answer.txt\"");
	//headers.push_back("Content-Type: text/plain");
	//sprintf(buf, "Content-Length: %d", st.st_size);
	//headers.push_back(buf);

	//using namespace cURLpp::Options;
	//// file の中身をifstreamとしてcURLに渡す
	//request.setOpt(new ReadStream(new std::ifstream(filename)));
	//request.setOpt(new InfileSize(st.st_size));
	//request.setOpt(new Upload(true));
	//request.setOpt(new HttpHeader(headers));
	//// defaultは1.1．1.0にするとExpect: 100-continueが出なくなる
	////request.setOpt(new HttpVersion(1.0));
	//request.setOpt(new Url(url));

	//request.perform();
}

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

  boost::asio::io_service ios;
  // test DONE
  //Client cl(ios, "172.26.32.189", 65534);
  //cl.accept();
  //auto str = cl.receive();
  //std::cerr << str << std::endl;

  //Core core(ios, "testform26.procon-online.net", "0123456789abcdef", 1);
  //auto probs = core.get();
  Core core2(ios, "testform26.procon-online.net", "0123456789abcdef", 1);
  std::stringstream ss;
  for (int i = 0; i < 36; ++i) {
    ss  << std::endl;
  }
  uploadFile();
//  core2.submit(ss.str());


  return 0;
}

