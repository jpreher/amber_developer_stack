#include "yaml-cpp-0.2.7/yaml.h"
#include "yaml-cpp-0.2.7/eventhandler.h"
#include <fstream>
#include <iostream>
#include <vector>

struct Params {
	bool hasFile;
	std::string fileName;
};

Params ParseArgs(int argc, char **argv) {
	Params p;

	std::vector<std::string> args(argv + 1, argv + argc);
	
	return p;
}

class NullEventHandler: public YAML_0_2_7::EventHandler
{
public:
	virtual void OnDocumentStart(const YAML_0_2_7::Mark&) {}
	virtual void OnDocumentEnd() {}
	
	virtual void OnNull(const YAML_0_2_7::Mark&, YAML_0_2_7::anchor_t) {}
	virtual void OnAlias(const YAML_0_2_7::Mark&, YAML_0_2_7::anchor_t) {}
	virtual void OnScalar(const YAML_0_2_7::Mark&, const std::string&, YAML_0_2_7::anchor_t, const std::string&) {}
	
	virtual void OnSequenceStart(const YAML_0_2_7::Mark&, const std::string&, YAML_0_2_7::anchor_t) {}
	virtual void OnSequenceEnd() {}
	
	virtual void OnMapStart(const YAML_0_2_7::Mark&, const std::string&, YAML_0_2_7::anchor_t) {}
	virtual void OnMapEnd() {}
};

void parse(std::istream& input)
{
	try {
		YAML_0_2_7::Parser parser(input);
		YAML_0_2_7::Node doc;
		NullEventHandler handler;
		while(parser.GetNextDocument(doc)) {
			YAML_0_2_7::Emitter emitter;
			emitter << doc;
			std::cout << emitter.c_str() << "\n";
		}
	} catch(const YAML_0_2_7::Exception& e) {
		std::cerr << e.what() << "\n";
	}
}

int main(int argc, char **argv)
{
	Params p = ParseArgs(argc, argv);

	if(argc > 1) {
		std::ifstream fin;
		fin.open(argv[1]);
		parse(fin);
	} else {
		parse(std::cin);
	}

	return 0;
}
