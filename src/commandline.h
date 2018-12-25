#include <string>
#include <variant>

struct Empty {};

class NameOrID {
	public:

		bool empty() const { return std::holds_alternative<Empty>(storage); }

		int getId() const { return std::get<int>(storage); }
		bool hasId() const { return std::holds_alternative<int>(storage); }
		void setId(int id) { storage = id; }

		bool hasName() const { return std::holds_alternative<std::string>(storage); }
		void setName(std::string const& name) { storage = name; }
		std::string getName() const { return std::get<std::string>(storage); }
	private:

	std::variant<Empty, int, std::string> storage;
};

void set(NameOrID& storage, std::string const& value);

enum class Mode {
	Compile,
	ListDevices
};

struct CommandLineOptions {
	NameOrID platform;
	NameOrID device;
	std::string source_file;
	std::string compile_options = "";
	Mode mode = Mode::Compile;
	bool quiet = false;
};

CommandLineOptions parse_options(int argc, char * const argv[]);
