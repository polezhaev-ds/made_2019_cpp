//
// Created by admin2 on 17.11.2019.
//

#define CATCH_CONFIG_MAIN

#include <string>
#include <sstream>
#include <type_traits>
#include "data.h"
#include "catch2/catch.hpp"
#include "serialization.h"
#include "test_helpers.h"

template <class TData>
bool checkSerializerForData(const TData& data, const std::string& serializedStr) {
    std::stringstream stream;
    Serializer serializer(stream);
    return serializer.save(data) == Error::NoError && stream.str() == serializedStr;
}

template <class TData>
bool checkDeserializerForData(const std::string& serializedStr, const TData& data) {
    std::stringstream stream(serializedStr);
    Deserializer deserializer(stream);
    TData tempData;
    const Error error = deserializer.load(tempData);
    return error == Error::NoError && tempData == data;
}

template <class TData>
Error getDeserializationResult(const std::string& serializedStr) {
    std::stringstream stream(serializedStr);
    Deserializer deserializer(stream);
    TData tempData;
    return deserializer.load(tempData);
}

TEST_CASE( "Serializer works correctly for types with a single fieled" ) {

    SECTION( "Serializer works correctly for types with a uint64_t field" ) {
        CHECK( checkSerializerForData(SingleFieldData<std::uint64_t> {0ull}, "0") );
        CHECK( checkSerializerForData(SingleFieldData<std::uint64_t> {12345ull}, "12345") );
        std::uint64_t max_uint64_value = std::numeric_limits<uint64_t>::max();
        std::string max_uint64_str = std::to_string(max_uint64_value);
        CHECK( checkSerializerForData(SingleFieldData<std::uint64_t> {max_uint64_value}, max_uint64_str) );
    }

    SECTION( "Serializer works correctly for types with a bool field" ) {
        CHECK( checkSerializerForData(SingleFieldData<bool> {true}, "true") );
        CHECK( checkSerializerForData(SingleFieldData<bool> {false}, "false") );
    }

}

TEST_CASE( "Deserializer works correctly for types with a single filed" ) {

    SECTION( "Deserializer works correctly for types with a uint64_t field" ) {

        SECTION("Deserializer works correctly for correct strings") {
            CHECK( checkDeserializerForData("0", SingleFieldData<std::uint64_t> {0ull}) );
            std::uint64_t max_uint64_value = std::numeric_limits<uint64_t>::max();
            std::string max_uint64_str = std::to_string(max_uint64_value);
            CHECK( checkDeserializerForData(max_uint64_str, SingleFieldData<std::uint64_t> {max_uint64_value}) );
            CHECK( checkDeserializerForData("55345", SingleFieldData<std::uint64_t> {55345ull}) );
        }

        SECTION("Deserializer works correctly for incorrect strings") {
            CHECK( getDeserializationResult<SingleFieldData<std::uint64_t>>("-1") == Error::CorruptedArchive );
            CHECK( getDeserializationResult<SingleFieldData<std::uint64_t>>("  -5234234  ") == Error::CorruptedArchive );
            CHECK( getDeserializationResult<SingleFieldData<std::uint64_t>>("") == Error::CorruptedArchive );
            CHECK( getDeserializationResult<SingleFieldData<std::uint64_t>>("   ") == Error::CorruptedArchive );
            CHECK( getDeserializationResult<SingleFieldData<std::uint64_t>>("999999999999999999999999999999999999999999999")
                                                                            == Error::CorruptedArchive );
        }

    }

    SECTION( "Deserializer works correctly for types with a bool field" ) {

        SECTION("Deserializer works correctly for correct strings") {
            CHECK( checkDeserializerForData("true", SingleFieldData<bool> {true}) );
            CHECK( checkDeserializerForData("false", SingleFieldData<bool> {false}) );
            CHECK( checkDeserializerForData(" true ", SingleFieldData<bool> {true}) );
        }

        SECTION("Deserializer works correctly for incorrect strings") {
            CHECK( getDeserializationResult<SingleFieldData<bool>>("truesdf") == Error::CorruptedArchive );
            CHECK( getDeserializationResult<SingleFieldData<bool>>("FALSE") == Error::CorruptedArchive );
            CHECK( getDeserializationResult<SingleFieldData<bool>>("") == Error::CorruptedArchive );
            CHECK( getDeserializationResult<SingleFieldData<bool>>("   ") == Error::CorruptedArchive );
            CHECK( getDeserializationResult<SingleFieldData<bool>>("sdfs") == Error::CorruptedArchive );
        }

    }

}


TEST_CASE( "Serializer works correctly for Data type (with three fields - uint64_t, bool, uint64_t)" ) {

    SECTION( "Serializer works correctly" ) {
        CHECK( checkSerializerForData(Data {0ull, true, 0ull}, "0 true 0") );
        std::uint64_t max_uint64_value = std::numeric_limits<uint64_t>::max();
        std::string max_uint64_str = std::to_string(max_uint64_value);
        CHECK( checkSerializerForData(Data {0ull, false, max_uint64_value},
                                      std::string("0 false ") + max_uint64_str) );
        CHECK( checkSerializerForData(Data {max_uint64_value, true, max_uint64_value},
                                      max_uint64_str + std::string(" true ") + max_uint64_str) );
        CHECK( checkSerializerForData(Data {55345ull, true, 234234ull}, "55345 true 234234") );
    }
}


TEST_CASE( "Deserializer works correctly for Data type (with three fields - uint64_t, bool, uint64_t)" ) {

    SECTION( "Deserializer works correctly for correct strings" ) {
        CHECK( checkDeserializerForData("0 true 0", Data {0ull, true, 0ull}) );
        std::uint64_t max_uint64_value = std::numeric_limits<uint64_t>::max();
        std::string max_uint64_str = std::to_string(max_uint64_value);
        CHECK( checkDeserializerForData(std::string("0 false ") + max_uint64_str,
                                        Data {0ull, false, max_uint64_value}) );
        CHECK( checkDeserializerForData(max_uint64_str + std::string(" true ") + max_uint64_str,
                                        Data {max_uint64_value, true, max_uint64_value}) );
        CHECK( checkDeserializerForData("55345 true 234234", Data {55345ull, true, 234234ull}) );
    }

    SECTION( "Deserializer returns errors for incorrect strings" ) {
        CHECK( getDeserializationResult<Data>("-5 false 10") == Error::CorruptedArchive );
        CHECK( getDeserializationResult<Data>("5 false -10") == Error::CorruptedArchive );
        CHECK( getDeserializationResult<Data>("5 sdf 10") == Error::CorruptedArchive );
        CHECK( getDeserializationResult<Data>("5 10") == Error::CorruptedArchive );
        CHECK( getDeserializationResult<Data>("false 10") == Error::CorruptedArchive );
        CHECK( getDeserializationResult<Data>("") == Error::CorruptedArchive );
        CHECK( getDeserializationResult<Data>("true") == Error::CorruptedArchive );
        CHECK( getDeserializationResult<Data>(" true ") == Error::CorruptedArchive );
        CHECK( getDeserializationResult<Data>("324534") == Error::CorruptedArchive );
        CHECK( getDeserializationResult<Data>(" 324534 ") == Error::CorruptedArchive );
        CHECK( getDeserializationResult<Data>("5 true 999999999999999999999999999999999999999999999") == Error::CorruptedArchive );
        CHECK( getDeserializationResult<Data>("999999999999999999999999999999999999999999999 true 3") == Error::CorruptedArchive );
    }

}
