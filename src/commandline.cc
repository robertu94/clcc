#include "commandline.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unistd.h>
#include <cstdlib>

void
set(NameOrID& storage, std::string const& value)
{
  try {
    storage.setId(std::stoi(value));
  } catch (std::invalid_argument) {
    storage.setName(value);
  }
}

void
usage()
{
  std::cerr << "Usage:\n"
               "\tclcc [-p platform] [-d device] [-q] file [ -- options... ]\n"
               "\tclcc -l\n"
               "\tclcc -h\n"
               "Options\n"
               "\tfile\tthe file to compile\n"
               "\toptions\tthe options to pass to the OpenCL compiler\n"
               "\t-d\tthe OpenCL device to use\n"
               "\t-h\tprints this message\n"
               "\t-h\tlists the installed OpenCL devices and platforms\n"
               "\t-p\tthe OpenCL platform to use\n"
               "\t-q\tprint less output\n"
               "\t--\tonly options passed to the OpenCL compiler after this\n"
               "Environmental\n"
							 "\tCLCC_PLATFORM\tthe OpenCL platform to use; overwritten by options\n"
							 "\tCLCC_DEVICE\tthe OpenCL device to use; overwritten by options\n"
            << std::endl;
}

CommandLineOptions
parse_options(int argc, char* const argv[])
{
  CommandLineOptions options;

	if(char* platform = getenv("CLCC_PLATFORM")){
		set(options.platform, platform);
	}

	if(char* target = getenv("CLCC_DEVICE")){
		set(options.device, target);
	}

  int option;
  while ((option = getopt(argc, argv, "d:hlp:q")) != -1) {
    switch (option) {
      case 'd':
        set(options.device, optarg);
        break;
      case 'h':
        usage();
        exit(EXIT_SUCCESS);
      case 'l':
        options.mode = Mode::ListDevices;
        break;
      case 'p':
        set(options.platform, optarg);
        break;
      case 'q':
        options.quiet = true;
        break;
      default:
        usage();
        exit(EXIT_FAILURE);
    }
  }

  if (options.mode == Mode::Compile) {
    if (optind >= argc) {
      usage();
      exit(EXIT_FAILURE);
    }

    options.source_file = argv[optind++];

    std::ostringstream compile_options;
    if (optind < argc) {
      compile_options << argv[optind++];
      while (optind < argc) {
        compile_options << " " << argv[optind++];
      }
    }
    options.compile_options = compile_options.str();
  }
  return options;
}
