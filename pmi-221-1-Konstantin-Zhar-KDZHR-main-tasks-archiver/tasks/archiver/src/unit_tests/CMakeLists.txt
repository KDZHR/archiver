SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")

add_catch(test_priority_queue test_priority_queue.cpp)
add_catch(test_trie test_trie.cpp)
add_catch(test_bit_streams ../bit_streams.cpp ../exceptions.cpp test_bit_streams.cpp)
