#include "gtest/gtest.h"
#include "geco-bit-stream.h"
#include "geco-net-type.h"
#include "geco-basic-type.h"

using namespace geco::net;

struct vec { float x; float y; float z; };
TEST(GecoMemoryStreamTestCase, test_all_reads_and_writes)
{
    geco_bit_stream_t s8;

    uint24_t uint24 = 24;
    uchar uint8 = 8;
    ushort uint16 = 16;
    uint uint32 = 32;
    ulonglong uint64 = 64;
    char int8 = -8;
    short int16 = -16;
    int int32 = -32;
    longlong int64 = -64;

    uchar particialByte = 0xf0; /// 11110000
    guid_t guid(123);
    //NetworkAddress addr("192.168.1.107", 32000);
    vec vector_ = { 0.2f, -0.4f, -0.8f };
    vec vector__ = { 2.234f, -4.78f, -32.2f };

    uint curr = 12;
    uint min = 8;
    uint max = 64;
    s8.write_ranged_integer(curr, min, max);
    curr = 0;
    s8.ReadIntegerRange(curr, min, max);
    EXPECT_TRUE(curr == 12);

    s8.Write(uint24);
    uint24 = 0;
    s8.Read(uint24);
    EXPECT_TRUE(uint24.val == 24);

    s8.WriteBits(&particialByte, 7, false);
    uchar v = 0;
    s8.ReadBits(&v, 7, false);
    EXPECT_TRUE(particialByte == v);
    EXPECT_TRUE(s8.get_payloads() == 0);

    for (int i = 10; i >= 0; i--)
    {
        uint looptimes = 100000;
        for (uint i = 1; i <= looptimes; i++)
        {
            s8.Write(uint24);

            s8.WriteMini(guid);

            s8.WriteMini(uint24);

            //s8.WriteMini(addr);

            s8.WriteMini(uint24);

            s8.Write(uint8);
            s8.Write(int64);
            s8.WriteMini(uint8);
            s8.WriteMini(int64, false);

            s8.Write(uint16);
            s8.Write(int32);
            s8.WriteMini(uint16);
            s8.WriteMini(int32, false);

            s8.WriteBits(&particialByte, 4, true);
            s8.Write(uint24);
            s8.write_normal_vector(vector_.x, vector_.y, vector_.z);

            s8.write_ranged_integer(curr, min, max);

            s8.write_vector(vector__.x, vector__.y, vector__.z);

            s8.Write(uint32);
            s8.Write(int16);
            s8.WriteMini(uint32);
            s8.WriteMini(int16, false);

            s8.WriteBits(&particialByte, 4, false);

            s8.Write(uint64);
            s8.Write(int8);
            s8.WriteMini(uint64);
            s8.WriteMini(int8);

            s8.WriteBits(&particialByte, 7, false);
        }

        geco_bit_stream_t s9;
        s9.Write(s8);
        if (i == 0)
        {
            printf("uncompressed  '%.5f' MB\n", float(BITS_TO_BYTES(s9.get_payloads()) / 1024 / 1024));
        }

        for (uint i = 1; i <= looptimes; i++)
        {
            uint24 = 0;
            s9.Read(uint24);
            EXPECT_TRUE(uint24.val == 24);

            guid_t guidd;
            s9.ReadMini(guidd);
            EXPECT_TRUE(guid == guidd);

            uint24_t mini_uint24 = 0;
            s9.ReadMini(mini_uint24);
            EXPECT_TRUE(mini_uint24.val == 24);

            //NetworkAddress addrr;
            //s9.ReadMini(addrr);
            // EXPECT_TRUE(addr == addrr);

            mini_uint24 = 0;
            s9.ReadMini(mini_uint24);
            EXPECT_TRUE(mini_uint24.val == 24);

            s9.Read(uint8);
            s9.Read(int64);
            uchar mini_uint8;
            s9.ReadMini(mini_uint8);
            EXPECT_TRUE(mini_uint8 == uint8);

            longlong mini_int64;
            s9.ReadMini(mini_int64, false);
            EXPECT_TRUE(mini_int64 == int64);

            s9.Read(uint16);
            s9.Read(int32);
            ushort mini_uint16;
            s9.ReadMini(mini_uint16);
            EXPECT_TRUE(mini_uint16 == uint16);

            int mini_int32;
            s9.ReadMini(mini_int32, false);
            EXPECT_TRUE(mini_int32 == int32);


            uchar v = 0;
            s9.ReadBits(&v, 4, true);
            EXPECT_TRUE(v == 0);

            uint24 = 0;
            s9.Read(uint24);
            EXPECT_TRUE(uint24.val == 24);

            vec vectorr;
            s9.ReadNormVector(vectorr.x, vectorr.y, vectorr.z);
            EXPECT_TRUE(fabs(vectorr.x - vector_.x) <= 0.0001f);
            EXPECT_TRUE(fabs(vectorr.y - vector_.y) <= 0.0001f);
            EXPECT_TRUE(fabs(vectorr.y - vector_.y) <= 0.0001f);

            uint v1;
            s9.ReadIntegerRange(v1, min, max);
            EXPECT_TRUE(v1 == curr);

            vec vectorrr;
            s9.ReadVector(vectorrr.x, vectorrr.y, vectorrr.z);
            EXPECT_TRUE(fabs(vectorrr.x - vector__.x) <= 0.001f);
            EXPECT_TRUE(fabs(vectorrr.y - vector__.y) <= 0.001f);
            EXPECT_TRUE(fabs(vectorrr.y - vector__.y) <= 0.001f);

            s9.Read(uint32);
            s9.Read(int16);
            uint mini_uint32;
            s9.ReadMini(mini_uint32);
            EXPECT_TRUE(mini_uint32 == uint32);

            short mini_int16;
            s9.ReadMini(mini_int16, false);
            EXPECT_TRUE(mini_int16 == int16);

            v = 0;
            s9.ReadBits(&v, 4, false);
            EXPECT_TRUE(particialByte == ((v >> 4) << 4));

            s9.Read(uint64);
            s9.Read(int8);
            ulonglong mini_uint64;
            s9.ReadMini(mini_uint64);
            EXPECT_TRUE(mini_uint64 == uint64);

            char mini_int8;
            s9.ReadMini(mini_int8, false);
            EXPECT_TRUE(mini_int8 == int8);

            v = 0;
            s9.ReadBits(&v, 7, false);
            EXPECT_TRUE(particialByte == ((v >> 1) << 1));

            EXPECT_TRUE(uint8 == 8);
            EXPECT_TRUE(int8 == -8);
            EXPECT_TRUE(uint16 == 16);
            EXPECT_TRUE(int16 == -16);
            EXPECT_TRUE(uint24.val == 24);
            EXPECT_TRUE(uint32 == 32);
            EXPECT_TRUE(int32 == -32);
            EXPECT_TRUE(uint64 == 64);
            EXPECT_TRUE(int64 == -64);
        }

        s8.reset();
        s9.reset();
    }
}
TEST(GecoMemoryStreamTestCase, test_all_reads_and_writes_un_compressed)
{
    geco_bit_stream_t s8;

    uint24_t uint24 = 24;
    uchar uint8 = 8;
    ushort uint16 = 16;
    uint uint32 = 32;
    ulonglong uint64 = 64;
    char int8 = -8;
    short int16 = -16;
    int int32 = -32;
    longlong int64 = -64;

    uchar particialByte = 0xf0; /// 11110000
    guid_t guid(123);
    //NetworkAddress addr("192.168.1.107", 32000);
    vec vector_ = { 0.2f, -0.4f, -0.8f };
    vec vector__ = { 2.234f, -4.78f, -32.2f };

    uint curr = 12;
    uint min = 8;
    uint max = 64;
    s8.write_ranged_integer(curr, min, max);
    curr = 0;
    s8.ReadIntegerRange(curr, min, max);
    EXPECT_TRUE(curr == 12);

    s8.Write(uint24);
    uint24 = 0;
    s8.Read(uint24);
    EXPECT_TRUE(uint24.val == 24);

    s8.WriteBits(&particialByte, 7, false);
    uchar v = 0;
    s8.ReadBits(&v, 7, false);
    EXPECT_TRUE(particialByte == v);
    EXPECT_TRUE(s8.get_payloads() == 0);

    for (int i = 10; i >= 0; i--)
    {
        uint looptimes = 100000;
        for (uint i = 1; i <= looptimes; i++)
        {
            s8.Write(uint24);

            s8.Write(guid);

            s8.Write(uint24);

            //s8.Write(addr);

            s8.Write(uint24);

            s8.Write(uint8);
            s8.Write(int64);
            s8.Write(uint8);
            s8.Write(int64);

            s8.Write(uint16);
            s8.Write(int32);
            s8.Write(uint16);
            s8.Write(int32);

            s8.WriteBits(&particialByte, 4, true);
            s8.Write(uint24);
            s8.write_normal_vector(vector_.x, vector_.y, vector_.z);

            s8.write_ranged_integer(curr, min, max);

            s8.write_vector(vector__.x, vector__.y, vector__.z);

            s8.Write(uint32);
            s8.Write(int16);
            s8.Write(uint32);
            s8.Write(int16);

            s8.WriteBits(&particialByte, 4, false);

            s8.Write(uint64);
            s8.Write(int8);
            s8.Write(uint64);
            s8.Write(int8);

            s8.WriteBits(&particialByte, 7, false);
        }

        geco_bit_stream_t s9;
        s9.Write(s8);

        if (i == 0)
        {
            printf("uncompressed  '%.5f' MB\n", float(BITS_TO_BYTES(s9.get_payloads()) / 1024 / 1024));
        }

        for (uint i = 1; i <= looptimes; i++)
        {
            uint24 = 0;
            s9.Read(uint24);
            EXPECT_TRUE(uint24.val == 24);

            guid_t guidd;
            s9.Read(guidd);
            EXPECT_TRUE(guid == guidd);

            uint24_t mini_uint24 = 0;
            s9.Read(mini_uint24);
            EXPECT_TRUE(mini_uint24.val == 24);

            //NetworkAddress addrr;
            //s9.Read(addrr);
            //EXPECT_TRUE(addr == addrr);

            mini_uint24 = 0;
            s9.Read(mini_uint24);
            EXPECT_TRUE(mini_uint24.val == 24);

            s9.Read(uint8);
            s9.Read(int64);
            uchar mini_uint8;
            s9.Read(mini_uint8);
            EXPECT_TRUE(mini_uint8 == uint8);

            longlong mini_int64;
            s9.Read(mini_int64);
            EXPECT_TRUE(mini_int64 == int64);

            s9.Read(uint16);
            s9.Read(int32);
            ushort mini_uint16;
            s9.Read(mini_uint16);
            EXPECT_TRUE(mini_uint16 == uint16);

            int mini_int32;
            s9.Read(mini_int32);
            EXPECT_TRUE(mini_int32 == int32);


            uchar v = 0;
            s9.ReadBits(&v, 4, true);
            EXPECT_TRUE(v == 0);

            uint24 = 0;
            s9.Read(uint24);
            EXPECT_TRUE(uint24.val == 24);

            vec vectorr;
            s9.ReadNormVector(vectorr.x, vectorr.y, vectorr.z);
            EXPECT_TRUE(fabs(vectorr.x - vector_.x) <= 0.0001f);
            EXPECT_TRUE(fabs(vectorr.y - vector_.y) <= 0.0001f);
            EXPECT_TRUE(fabs(vectorr.y - vector_.y) <= 0.0001f);

            uint v1;
            s9.ReadIntegerRange(v1, min, max);
            EXPECT_TRUE(v1 == curr);

            vec vectorrr;
            s9.ReadVector(vectorrr.x, vectorrr.y, vectorrr.z);
            EXPECT_TRUE(fabs(vectorrr.x - vector__.x) <= 0.001f);
            EXPECT_TRUE(fabs(vectorrr.y - vector__.y) <= 0.001f);
            EXPECT_TRUE(fabs(vectorrr.y - vector__.y) <= 0.001f);

            s9.Read(uint32);
            s9.Read(int16);
            uint mini_uint32;
            s9.Read(mini_uint32);
            EXPECT_TRUE(mini_uint32 == uint32);

            short mini_int16;
            s9.Read(mini_int16);
            EXPECT_TRUE(mini_int16 == int16);

            v = 0;
            s9.ReadBits(&v, 4, false);
            EXPECT_TRUE(particialByte == ((v >> 4) << 4));

            s9.Read(uint64);
            s9.Read(int8);
            ulonglong mini_uint64;
            s9.Read(mini_uint64);
            EXPECT_TRUE(mini_uint64 == uint64);

            char mini_int8;
            s9.Read(mini_int8);
            EXPECT_TRUE(mini_int8 == int8);

            v = 0;
            s9.ReadBits(&v, 7, false);
            EXPECT_TRUE(particialByte == ((v >> 1) << 1));

            EXPECT_TRUE(uint8 == 8);
            EXPECT_TRUE(int8 == -8);
            EXPECT_TRUE(uint16 == 16);
            EXPECT_TRUE(int16 == -16);
            EXPECT_TRUE(uint24.val == 24);
            EXPECT_TRUE(uint32 == 32);
            EXPECT_TRUE(int32 == -32);
            EXPECT_TRUE(uint64 == 64);
            EXPECT_TRUE(int64 == -64);
        }

        s8.reset();
        s9.reset();
    }
}
