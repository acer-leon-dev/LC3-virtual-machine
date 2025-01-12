#include "LC3VM/lc3.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::println("lc3 [image-file1] ...");
        return 2;
    }

    lc3::LC3 vm;

    for (int i = 1; i < argc; i++) {
        if (!vm.read_image(argv[i])) {
            std::println("failed to load image: {}", argv[i]);
            return 1;
        }
    }

    vm.run();
}