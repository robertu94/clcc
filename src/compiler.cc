#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <vector>
#include <stdexcept>

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

#include "commandline.h"

template <int Field, class Vector>
typename Vector::value_type
find_or_exit(Vector available, NameOrID option, const char* message)
{
  if (available.empty()) {
    std::cerr << message << std::endl;
    exit(EXIT_FAILURE);
  } else if (option.empty()) {
    return available.front();
  } else if (option.hasId()) {
    return available.at(option.getId());
  } else {
    auto requested = std::regex(option.getName(), std::regex::icase);
    std::smatch match;
    auto found =
      std::find_if(std::begin(available), std::end(available),
                   [requested, &match](auto const& item) {
                     auto name = item.template getInfo<Field>();
                     return std::regex_search(name, match, requested);
                   });

    if (found != std::end(available)) {
      return *found;
    } else {
      std::cerr << message << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

std::string
read_file(std::string path)
{
  std::ifstream infile(path);
	if(infile.fail()) throw std::runtime_error(path + "not found");
  std::ostringstream stream;
  stream << infile.rdbuf();
  return stream.str();
}

void
compile(CommandLineOptions options)
{
  std::vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);
  cl::Platform platform = find_or_exit<CL_PLATFORM_NAME>(
    platforms, options.platform, "No OpenCL platforms found!");

  std::vector<cl::Device> devices;
  platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
  cl::Device device = find_or_exit<CL_DEVICE_NAME>(devices, options.device,
                                                   "No OpenCL devices found!");

  cl::Context context({ device });
  auto source_file = read_file(options.source_file);
  cl::Program::Sources program_sources = { { source_file.c_str(),
                                             source_file.size() + 1 } };
  auto program = cl::Program(context, program_sources);

  try {
    if (!options.quiet) {
      std::cout << "compiling for: " << platform.getInfo<CL_PLATFORM_NAME>()
                << " " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
      if (!options.compile_options.empty()) {
        std::cout << "options: " << options.compile_options << std::endl;
      }
    }
    program.build({ device }, options.compile_options.c_str());
  } catch (cl::Error const& e) {
    std::cerr << e.what() << " : " << e.err() << std::endl;
    std::cerr << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device)
              << std::endl;
  }
}

void
list_devices(CommandLineOptions options)
{
  std::vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);
  for (auto platform : platforms) {
    std::vector<cl::Device> devices;
    try {
      platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
      auto platform_name = platform.getInfo<CL_PLATFORM_NAME>();
      for (auto device : devices) {
        auto device_name = device.getInfo<CL_DEVICE_NAME>();
        std::cout << platform_name << '\t' << device_name << std::endl;
      }
    } catch(cl::Error const&) {/*intentional noop*/}
  }
}

int
main(int argc, char* const argv[])
{
  CommandLineOptions options = parse_options(argc, argv);

  switch (options.mode) {
    case Mode::Compile:
      compile(options);
      break;
    case Mode::ListDevices:
      list_devices(options);
      break;
  }

  return 0;
}
