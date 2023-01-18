#include <catch.hpp>
#include <queue>
#include <sstream>

#include "../bit_streams.h"

TEST_CASE("BitStreams") {
    std::istringstream istream("abc");
    BitReader bit_reader(istream);
    REQUIRE(bit_reader.ReadNBits(3) == std::vector{false, true, true});
    REQUIRE(bit_reader.ReadNBits(6) == std::vector{false, false, false, false, true, false});
    REQUIRE(bit_reader.ReadNBits(100) == std::vector{true, true, false, false, false, true, false, false, true, true,
                                                     false, false, false, true, true});

    std::ostringstream ostream;
    { BitWriter bit_writer(ostream); }
    {
        BitWriter bit_writer(ostream);
        bit_writer.WriteNBits({false, true, true, false, false, false, false, true});
    }
    REQUIRE(ostream.str() == "a");
    {
        BitWriter bit_writer(ostream);
        bit_writer.WriteBit(false);
        bit_writer.WriteNBits(
            {true, false, false, false, false, true, false, false, true, true, false, false, false, true, true});
    }
    REQUIRE(ostream.str() == "aBc");
    {
        BitWriter bit_writer(ostream);
        std::vector<bool> to_stream;
        for (size_t i = 0; i < 2048; ++i) {
            to_stream.insert(to_stream.end(), {false, true, false, false, false, true, false, false});
        }
        bit_writer.WriteNBits(to_stream);
    }
    REQUIRE(ostream.str() == "aBc" + std::string(2048, 'D'));
}
