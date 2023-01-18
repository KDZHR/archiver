add_executable(
        archiver
        archiver.cpp
        exceptions.cpp compress.cpp decompress.cpp bit_streams.cpp help.cpp binary_processing_tools.cpp arg_parser.cpp
        binary_processing_tools.h  priority_queue.h trie.h)

add_subdirectory(unit_tests)
