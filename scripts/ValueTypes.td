//===- ValueTypes.td - ValueType definitions ---------------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Value types - These values correspond to the register types defined in the
// ValueTypes.h file.  If you update anything here, you must update it there as
// well!
//
//===----------------------------------------------------------------------===//

class ValueType<int size, int value> {
  string Namespace = "MVT";
  int Size = size;
  int Value = value;
}

def OtherVT: ValueType<0  ,  0>;   // "Other" value
def i1	: ValueType<1 , 1>;
def i2	: ValueType<2 , 2>;
def i4	: ValueType<4 , 3>;
def i8	: ValueType<8 , 4>;
def i16	: ValueType<16 , 5>;
def i32	: ValueType<32 , 6>;
def i64	: ValueType<64 , 7>;
def i128	: ValueType<128 , 8>;
def i256	: ValueType<256 , 9>;
def i512	: ValueType<512 , 10>;
def i1024	: ValueType<1024 , 11>;

def f16    : ValueType<16 , 12>;   // 16-bit floating point value
def f32    : ValueType<32 , 13>;   // 32-bit floating point value
def f64    : ValueType<64 , 14>;   // 64-bit floating point value
def f80    : ValueType<80 , 15>;   // 80-bit floating point value
def f128   : ValueType<128, 16>;   // 128-bit floating point value
def ppcf128: ValueType<128, 17>;   // PPC 128-bit floating point value

def v1i1	: ValueType<1 , 18>;
def v2i1	: ValueType<2 , 19>;
def v4i1	: ValueType<4 , 20>;
def v8i1	: ValueType<8 , 21>;
def v16i1	: ValueType<16 , 22>;
def v32i1	: ValueType<32 , 23>;
def v64i1	: ValueType<64 , 24>;
def v128i1	: ValueType<128 , 25>;
def v256i1	: ValueType<256 , 26>;
def v512i1	: ValueType<512 , 27>;
def v1024i1	: ValueType<1024 , 28>;
def v1i2	: ValueType<2 , 29>;
def v2i2	: ValueType<4 , 30>;
def v4i2	: ValueType<8 , 31>;
def v8i2	: ValueType<16 , 32>;
def v16i2	: ValueType<32 , 33>;
def v32i2	: ValueType<64 , 34>;
def v64i2	: ValueType<128 , 35>;
def v128i2	: ValueType<256 , 36>;
def v256i2	: ValueType<512 , 37>;
def v512i2	: ValueType<1024 , 38>;
def v1i4	: ValueType<4 , 39>;
def v2i4	: ValueType<8 , 40>;
def v4i4	: ValueType<16 , 41>;
def v8i4	: ValueType<32 , 42>;
def v16i4	: ValueType<64 , 43>;
def v32i4	: ValueType<128 , 44>;
def v64i4	: ValueType<256 , 45>;
def v128i4	: ValueType<512 , 46>;
def v256i4	: ValueType<1024 , 47>;
def v1i8	: ValueType<8 , 48>;
def v2i8	: ValueType<16 , 49>;
def v4i8	: ValueType<32 , 50>;
def v8i8	: ValueType<64 , 51>;
def v16i8	: ValueType<128 , 52>;
def v32i8	: ValueType<256 , 53>;
def v64i8	: ValueType<512 , 54>;
def v128i8	: ValueType<1024 , 55>;
def v1i16	: ValueType<16 , 56>;
def v2i16	: ValueType<32 , 57>;
def v4i16	: ValueType<64 , 58>;
def v8i16	: ValueType<128 , 59>;
def v16i16	: ValueType<256 , 60>;
def v32i16	: ValueType<512 , 61>;
def v64i16	: ValueType<1024 , 62>;
def v1i32	: ValueType<32 , 63>;
def v2i32	: ValueType<64 , 64>;
def v4i32	: ValueType<128 , 65>;
def v8i32	: ValueType<256 , 66>;
def v16i32	: ValueType<512 , 67>;
def v32i32	: ValueType<1024 , 68>;
def v1i64	: ValueType<64 , 69>;
def v2i64	: ValueType<128 , 70>;
def v4i64	: ValueType<256 , 71>;
def v8i64	: ValueType<512 , 72>;
def v16i64	: ValueType<1024 , 73>;
def v1i128	: ValueType<128 , 74>;
def v2i128	: ValueType<256 , 75>;
def v4i128	: ValueType<512 , 76>;
def v8i128	: ValueType<1024 , 77>;
def v1i256	: ValueType<256 , 78>;
def v2i256	: ValueType<512 , 79>;
def v4i256	: ValueType<1024 , 80>;
def v1i512	: ValueType<512 , 81>;
def v2i512	: ValueType<1024 , 82>;
def v1i1024	: ValueType<1024 , 83>;


def v1f16	: ValueType<16 , 84>;
def v2f16	: ValueType<32 , 85>;
def v4f16	: ValueType<64 , 86>;
def v8f16	: ValueType<128 , 87>;
def v16f16	: ValueType<256 , 88>;
def v32f16	: ValueType<512 , 89>;
def v64f16	: ValueType<1024 , 90>;
def v1f32	: ValueType<32 , 91>;
def v2f32	: ValueType<64 , 92>;
def v4f32	: ValueType<128 , 93>;
def v8f32	: ValueType<256 , 94>;
def v16f32	: ValueType<512 , 95>;
def v32f32	: ValueType<1024 , 96>;
def v1f64	: ValueType<64 , 97>;
def v2f64	: ValueType<128 , 98>;
def v4f64	: ValueType<256 , 99>;
def v8f64	: ValueType<512 , 100>;
def v16f64	: ValueType<1024 , 101>;


def x86mmx : ValueType<64 , 102>;   // X86 MMX value
def FlagVT : ValueType<0  , 103>;   // Pre-RA sched glue
def isVoid : ValueType<0  , 104>;   // Produces no value
def untyped: ValueType<8  , 105>;   // Produces an untyped value

def MetadataVT: ValueType<0, 250>; // Metadata

// Pseudo valuetype mapped to the current pointer size to any address space.
// Should only be used in TableGen.
def iPTRAny   : ValueType<0, 251>;

// Pseudo valuetype to represent "vector of any size"
def vAny   : ValueType<0  , 252>;

// Pseudo valuetype to represent "float of any format"
def fAny   : ValueType<0  , 253>;

// Pseudo valuetype to represent "integer of any bit width"
def iAny   : ValueType<0  , 254>;

// Pseudo valuetype mapped to the current pointer size.
def iPTR   : ValueType<0  , 255>;
