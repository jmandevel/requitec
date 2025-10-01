
#include <rq/context.hpp>
#include <rq/options.hpp>
#include <rq/types.hpp>

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/FileSystem.h>

int main(int argc, const char **argv) {
  std::string executable_path =
      llvm::sys::fs::getMainExecutable(argv[0], reinterpret_cast<void *>(main));
  // TODO remove fake_argv when done with alpha stage of development. this is
  // here for ease of use.
  if (argc == 1) {
    const char *fake_argv[] = {"./requite", "--emit=object", "test.rq", "-o",
                               "test.srq"};
    if (!rq::parseCommandLineOptions(5, fake_argv)) {
      return 1;
    }
  } else {
    if (!rq::parseCommandLineOptions(argc, argv)) {
      return 1;
    }
  }
  rq::Context context(std::move(executable_path));
  if (!context.run()) {
    return 1;
  }
  return 0;
}