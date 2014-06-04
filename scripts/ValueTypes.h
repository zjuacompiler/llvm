//===- CodeGen/ValueTypes.h - Machine-Level types ---------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the set of machine-level target independent types which
// legal values in the code generator use.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CODEGEN_VALUETYPES_H
#define LLVM_CODEGEN_VALUETYPES_H

#include "llvm/Support/DataTypes.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/MathExtras.h"
#include <cassert>
#include <string>

namespace llvm {
  class Type;
  class LLVMContext;
  struct EVT;

  /// MVT - Machine Value Type. Every type that is supported natively by some
  /// processor targeted by LLVM occurs here. This means that any legal value
  /// type can be represented by an MVT.
  class MVT {
  public:
    enum SimpleValueType {
      // INVALID_SIMPLE_VALUE_TYPE - Simple value types less than zero are
      // considered extended value types.
      INVALID_SIMPLE_VALUE_TYPE = -1,

      // If you change this numbering, you must change the values in
      // ValueTypes.td as well!
      Other          =   0,   // This is a non-standard value
	i1		=  1,
	i2		=  2,
	i4		=  3,
	i8		=  4,
	i16		=  5,
	i32		=  6,
	i64		=  7,
	i128		=  8,
	i256		=  9,
	i512		=  10,
	i1024		=  11,


      FIRST_INTEGER_VALUETYPE = i1,
      LAST_INTEGER_VALUETYPE  = i1024,
        
      f16            =   12,   // This is a 16 bit floating point value
      f32            =   13,   // This is a 32 bit floating point value
      f64            =   14,   // This is a 64 bit floating point value
      f80            =  15,   // This is a 80 bit floating point value
      f128           =  16,   // This is a 128 bit floating point value
      ppcf128        =  17,   // This is a PPC 128-bit floating point value
        
      FIRST_FP_VALUETYPE = f16,
      LAST_FP_VALUETYPE  = ppcf128,
        
	v1i1		=  18,
	v2i1		=  19,
	v4i1		=  20,
	v8i1		=  21,
	v16i1		=  22,
	v32i1		=  23,
	v64i1		=  24,
	v128i1		=  25,
	v256i1		=  26,
	v512i1		=  27,
	v1024i1		=  28,
	v1i2		=  29,
	v2i2		=  30,
	v4i2		=  31,
	v8i2		=  32,
	v16i2		=  33,
	v32i2		=  34,
	v64i2		=  35,
	v128i2		=  36,
	v256i2		=  37,
	v512i2		=  38,
	v1i4		=  39,
	v2i4		=  40,
	v4i4		=  41,
	v8i4		=  42,
	v16i4		=  43,
	v32i4		=  44,
	v64i4		=  45,
	v128i4		=  46,
	v256i4		=  47,
	v1i8		=  48,
	v2i8		=  49,
	v4i8		=  50,
	v8i8		=  51,
	v16i8		=  52,
	v32i8		=  53,
	v64i8		=  54,
	v128i8		=  55,
	v1i16		=  56,
	v2i16		=  57,
	v4i16		=  58,
	v8i16		=  59,
	v16i16		=  60,
	v32i16		=  61,
	v64i16		=  62,
	v1i32		=  63,
	v2i32		=  64,
	v4i32		=  65,
	v8i32		=  66,
	v16i32		=  67,
	v32i32		=  68,
	v1i64		=  69,
	v2i64		=  70,
	v4i64		=  71,
	v8i64		=  72,
	v16i64		=  73,
	v1i128		=  74,
	v2i128		=  75,
	v4i128		=  76,
	v8i128		=  77,
	v1i256		=  78,
	v2i256		=  79,
	v4i256		=  80,
	v1i512		=  81,
	v2i512		=  82,
	v1i1024		=  83,

    
      FIRST_INTEGER_VECTOR_VALUETYPE = v1i1,
      LAST_INTEGER_VECTOR_VALUETYPE = v1i1024,

	v1f16		=  84,
	v2f16		=  85,
	v4f16		=  86,
	v8f16		=  87,
	v16f16		=  88,
	v32f16		=  89,
	v64f16		=  90,
	v1f32		=  91,
	v2f32		=  92,
	v4f32		=  93,
	v8f32		=  94,
	v16f32		=  95,
	v32f32		=  96,
	v1f64		=  97,
	v2f64		=  98,
	v4f64		=  99,
	v8f64		=  100,
	v16f64		=  101,

    
      FIRST_FP_VECTOR_VALUETYPE = v2f16,
      LAST_FP_VECTOR_VALUETYPE = v16f64,

      FIRST_VECTOR_VALUETYPE = v1i1,
      LAST_VECTOR_VALUETYPE  = v16f64,

      x86mmx         =  102,   // This is an X86 MMX value

      Glue           =  103,   // This glues nodes together during pre-RA sched

      isVoid         =  104,   // This has no value

      Untyped        =  105,   // This value takes a register, but has
            // unspecified type.  The register class
            // will be determined by the opcode.

      LAST_VALUETYPE =  106,   // This always remains at the end of the list.

      // This is the current maximum for LAST_VALUETYPE.
      // MVT::MAX_ALLOWED_VALUETYPE is used for asserts and to size bit vectors
      // This value must be a multiple of 32.
      MAX_ALLOWED_VALUETYPE = 128,

      // Metadata - This is MDNode or MDString.
      Metadata       = 250,

      // iPTRAny - An int value the size of the pointer of the current
      // target to any address space. This must only be used internal to
      // tblgen. Other than for overloading, we treat iPTRAny the same as iPTR.
      iPTRAny        = 251,

      // vAny - A vector with any length and element size. This is used
      // for intrinsics that have overloadings based on vector types.
      // This is only for tblgen's consumption!
      vAny           = 252,

      // fAny - Any floating-point or vector floating-point value. This is used
      // for intrinsics that have overloadings based on floating-point types.
      // This is only for tblgen's consumption!
      fAny           = 253,

      // iAny - An integer or vector integer value of any bit width. This is
      // used for intrinsics that have overloadings based on integer bit widths.
      // This is only for tblgen's consumption!
      iAny           = 254,

      // iPTR - An int value the size of the pointer of the current
      // target.  This should only be used internal to tblgen!
      iPTR           = 255
    };

    SimpleValueType SimpleTy;

    MVT() : SimpleTy((SimpleValueType)(INVALID_SIMPLE_VALUE_TYPE)) {}
    MVT(SimpleValueType SVT) : SimpleTy(SVT) { }

    bool operator>(const MVT& S)  const { return SimpleTy >  S.SimpleTy; }
    bool operator<(const MVT& S)  const { return SimpleTy <  S.SimpleTy; }
    bool operator==(const MVT& S) const { return SimpleTy == S.SimpleTy; }
    bool operator!=(const MVT& S) const { return SimpleTy != S.SimpleTy; }
    bool operator>=(const MVT& S) const { return SimpleTy >= S.SimpleTy; }
    bool operator<=(const MVT& S) const { return SimpleTy <= S.SimpleTy; }

    /// isFloatingPoint - Return true if this is a FP, or a vector FP type.
    bool isFloatingPoint() const {
      return ((SimpleTy >= MVT::FIRST_FP_VALUETYPE &&
               SimpleTy <= MVT::LAST_FP_VALUETYPE) ||
              (SimpleTy >= MVT::FIRST_FP_VECTOR_VALUETYPE &&
               SimpleTy <= MVT::LAST_FP_VECTOR_VALUETYPE));
    }

    /// isInteger - Return true if this is an integer, or a vector integer type.
    bool isInteger() const {
      return ((SimpleTy >= MVT::FIRST_INTEGER_VALUETYPE &&
               SimpleTy <= MVT::LAST_INTEGER_VALUETYPE) ||
              (SimpleTy >= MVT::FIRST_INTEGER_VECTOR_VALUETYPE &&
               SimpleTy <= MVT::LAST_INTEGER_VECTOR_VALUETYPE));
    }

    /// isVector - Return true if this is a vector value type.
    bool isVector() const {
      return (SimpleTy >= MVT::FIRST_VECTOR_VALUETYPE &&
              SimpleTy <= MVT::LAST_VECTOR_VALUETYPE);
    }


    /// is1BitVector - Return true if this is a 1-bit vector type.
    bool is1BitVector() const {
    return (SimpleTy == MVT::v1i1);
    }

    /// is2BitVector - Return true if this is a 2-bit vector type.
    bool is2BitVector() const {
    return (SimpleTy == MVT::v2i1 ||
		SimpleTy == MVT::v1i2);
    }

    /// is4BitVector - Return true if this is a 4-bit vector type.
    bool is4BitVector() const {
    return (SimpleTy == MVT::v4i1 ||
		SimpleTy == MVT::v2i2 ||
		SimpleTy == MVT::v1i4);
    }

    /// is8BitVector - Return true if this is a 8-bit vector type.
    bool is8BitVector() const {
    return (SimpleTy == MVT::v8i1 ||
		SimpleTy == MVT::v4i2 ||
		SimpleTy == MVT::v2i4 ||
		SimpleTy == MVT::v1i8);
    }

    /// is16BitVector - Return true if this is a 16-bit vector type.
    bool is16BitVector() const {
    return (SimpleTy == MVT::v16i1 ||
		SimpleTy == MVT::v8i2 ||
		SimpleTy == MVT::v4i4 ||
		SimpleTy == MVT::v2i8 ||
		SimpleTy == MVT::v1i16 ||
		SimpleTy == MVT::v1f16);
    }

    /// is32BitVector - Return true if this is a 32-bit vector type.
    bool is32BitVector() const {
    return (SimpleTy == MVT::v32i1 ||
		SimpleTy == MVT::v16i2 ||
		SimpleTy == MVT::v8i4 ||
		SimpleTy == MVT::v4i8 ||
		SimpleTy == MVT::v2i16 ||
		SimpleTy == MVT::v1i32 ||
		SimpleTy == MVT::v2f16 ||
		SimpleTy == MVT::v1f32);
    }

    /// is64BitVector - Return true if this is a 64-bit vector type.
    bool is64BitVector() const {
    return (SimpleTy == MVT::v64i1 ||
		SimpleTy == MVT::v32i2 ||
		SimpleTy == MVT::v16i4 ||
		SimpleTy == MVT::v8i8 ||
		SimpleTy == MVT::v4i16 ||
		SimpleTy == MVT::v2i32 ||
		SimpleTy == MVT::v1i64 ||
		SimpleTy == MVT::v4f16 ||
		SimpleTy == MVT::v2f32 ||
		SimpleTy == MVT::v1f64);
    }

    /// is128BitVector - Return true if this is a 128-bit vector type.
    bool is128BitVector() const {
    return (SimpleTy == MVT::v128i1 ||
		SimpleTy == MVT::v64i2 ||
		SimpleTy == MVT::v32i4 ||
		SimpleTy == MVT::v16i8 ||
		SimpleTy == MVT::v8i16 ||
		SimpleTy == MVT::v4i32 ||
		SimpleTy == MVT::v2i64 ||
		SimpleTy == MVT::v1i128 ||
		SimpleTy == MVT::v8f16 ||
		SimpleTy == MVT::v4f32 ||
		SimpleTy == MVT::v2f64);
    }

    /// is256BitVector - Return true if this is a 256-bit vector type.
    bool is256BitVector() const {
    return (SimpleTy == MVT::v256i1 ||
		SimpleTy == MVT::v128i2 ||
		SimpleTy == MVT::v64i4 ||
		SimpleTy == MVT::v32i8 ||
		SimpleTy == MVT::v16i16 ||
		SimpleTy == MVT::v8i32 ||
		SimpleTy == MVT::v4i64 ||
		SimpleTy == MVT::v2i128 ||
		SimpleTy == MVT::v1i256 ||
		SimpleTy == MVT::v16f16 ||
		SimpleTy == MVT::v8f32 ||
		SimpleTy == MVT::v4f64);
    }

    /// is512BitVector - Return true if this is a 512-bit vector type.
    bool is512BitVector() const {
    return (SimpleTy == MVT::v512i1 ||
		SimpleTy == MVT::v256i2 ||
		SimpleTy == MVT::v128i4 ||
		SimpleTy == MVT::v64i8 ||
		SimpleTy == MVT::v32i16 ||
		SimpleTy == MVT::v16i32 ||
		SimpleTy == MVT::v8i64 ||
		SimpleTy == MVT::v4i128 ||
		SimpleTy == MVT::v2i256 ||
		SimpleTy == MVT::v1i512 ||
		SimpleTy == MVT::v32f16 ||
		SimpleTy == MVT::v16f32 ||
		SimpleTy == MVT::v8f64);
    }

    /// is1024BitVector - Return true if this is a 1024-bit vector type.
    bool is1024BitVector() const {
    return (SimpleTy == MVT::v1024i1 ||
		SimpleTy == MVT::v512i2 ||
		SimpleTy == MVT::v256i4 ||
		SimpleTy == MVT::v128i8 ||
		SimpleTy == MVT::v64i16 ||
		SimpleTy == MVT::v32i32 ||
		SimpleTy == MVT::v16i64 ||
		SimpleTy == MVT::v8i128 ||
		SimpleTy == MVT::v4i256 ||
		SimpleTy == MVT::v2i512 ||
		SimpleTy == MVT::v1i1024 ||
		SimpleTy == MVT::v64f16 ||
		SimpleTy == MVT::v32f32 ||
		SimpleTy == MVT::v16f64);
    }


    /// isOverloaded - Return true if this is an overloaded type for TableGen.
    bool isOverloaded() const {
      return (SimpleTy==MVT::iAny || SimpleTy==MVT::fAny ||
              SimpleTy==MVT::vAny || SimpleTy==MVT::iPTRAny);
    }

    /// isPow2VectorType - Returns true if the given vector is a power of 2.
    bool isPow2VectorType() const {
      unsigned NElts = getVectorNumElements();
      return !(NElts & (NElts - 1));
    }

    /// getPow2VectorType - Widens the length of the given vector MVT up to
    /// the nearest power of 2 and returns that type.
    MVT getPow2VectorType() const {
      if (isPow2VectorType())
        return *this;

      unsigned NElts = getVectorNumElements();
      unsigned Pow2NElts = 1 << Log2_32_Ceil(NElts);
      return MVT::getVectorVT(getVectorElementType(), Pow2NElts);
    }

    /// getScalarType - If this is a vector type, return the element type,
    /// otherwise return this.
    MVT getScalarType() const {
      return isVector() ? getVectorElementType() : *this;
    }


    MVT getVectorElementType() const {
        switch (SimpleTy) {
        default:
            llvm_unreachable("Not a vector MVT!");
	case v1i1:  case v2i1:  case v4i1:  case v8i1:  case v16i1:  case v32i1:  case v64i1:  case v128i1:  case v256i1:  case v512i1:  case v1024i1:  return i1;
	case v1i2:  case v2i2:  case v4i2:  case v8i2:  case v16i2:  case v32i2:  case v64i2:  case v128i2:  case v256i2:  case v512i2:  return i2;
	case v1i4:  case v2i4:  case v4i4:  case v8i4:  case v16i4:  case v32i4:  case v64i4:  case v128i4:  case v256i4:  return i4;
	case v1i8:  case v2i8:  case v4i8:  case v8i8:  case v16i8:  case v32i8:  case v64i8:  case v128i8:  return i8;
	case v1f16:  case v2f16:  case v4f16:  case v8f16:  case v16f16:  case v32f16:  case v64f16:  return f16;
	case v1i16:  case v2i16:  case v4i16:  case v8i16:  case v16i16:  case v32i16:  case v64i16:  return i16;
	case v1f32:  case v2f32:  case v4f32:  case v8f32:  case v16f32:  case v32f32:  return f32;
	case v1i32:  case v2i32:  case v4i32:  case v8i32:  case v16i32:  case v32i32:  return i32;
	case v1f64:  case v2f64:  case v4f64:  case v8f64:  case v16f64:  return f64;
	case v1i64:  case v2i64:  case v4i64:  case v8i64:  case v16i64:  return i64;
	case v1i128:  case v2i128:  case v4i128:  case v8i128:  return i128;
	case v1i256:  case v2i256:  case v4i256:  return i256;
	case v1i512:  case v2i512:  return i512;
	case v1i1024:  return i1024;

      }
    }




    unsigned getVectorNumElements() const {
      switch (SimpleTy) {
      default:
        llvm_unreachable("Not a vector MVT!");
      case v1i1: case v1i2: case v1i4: case v1i8: case v1i16: case v1f16: case v1i32: case v1f32: case v1i64: case v1f64: case v1i128: case v1i256: case v1i512: case v1i1024:  return 1;
      case v1024i1:  return 1024;
      case v128i1: case v128i2: case v128i4: case v128i8:  return 128;
      case v16i1: case v16i2: case v16i4: case v16i8: case v16i16: case v16f16: case v16i32: case v16f32: case v16i64: case v16f64:  return 16;
      case v2i1: case v2i2: case v2i4: case v2i8: case v2i16: case v2f16: case v2i32: case v2f32: case v2i64: case v2f64: case v2i128: case v2i256: case v2i512:  return 2;
      case v256i1: case v256i2: case v256i4:  return 256;
      case v32i1: case v32i2: case v32i4: case v32i8: case v32i16: case v32f16: case v32i32: case v32f32:  return 32;
      case v4i1: case v4i2: case v4i4: case v4i8: case v4i16: case v4f16: case v4i32: case v4f32: case v4i64: case v4f64: case v4i128: case v4i256:  return 4;
      case v512i1: case v512i2:  return 512;
      case v64i1: case v64i2: case v64i4: case v64i8: case v64i16: case v64f16:  return 64;
      case v8i1: case v8i2: case v8i4: case v8i8: case v8i16: case v8f16: case v8i32: case v8f32: case v8i64: case v8f64: case v8i128:  return 8;

      }
    }

    


    unsigned getSizeInBits() const {
    switch (SimpleTy) {
      default:
        llvm_unreachable("getSizeInBits called on extended MVT.");
      case Other:
        llvm_unreachable("Value type is non-standard value, Other.");
      case iPTR:
        llvm_unreachable("Value type size is target-dependent. Ask TLI.");
      case iPTRAny:
      case iAny:
      case fAny:
      case vAny:
        llvm_unreachable("Value type is overloaded.");
      case Metadata:
        llvm_unreachable("Value type is metadata.");
      case i1: case v1i1: return 1;
      case i2: case v2i1: case v1i2: return 2;
      case i4: case v4i1: case v2i2: case v1i4: return 4;
      case i8: case v8i1: case v4i2: case v2i4: case v1i8: return 8;
      case i16: case f16: case v16i1: case v8i2: case v4i4: case v2i8: case v1i16: case v1f16: return 16;
      case i32: case f32: case v32i1: case v16i2: case v8i4: case v4i8: case v2i16: case v1i32: case v2f16: case v1f32: return 32;
      case i64: case f64: case v64i1: case v32i2: case v16i4: case v8i8: case v4i16: case v2i32: case v1i64: case v4f16: case v2f32: case v1f64: return 64;
      case i128: case v128i1: case v64i2: case v32i4: case v16i8: case v8i16: case v4i32: case v2i64: case v1i128: case v8f16: case v4f32: case v2f64: return 128;
      case i256: case v256i1: case v128i2: case v64i4: case v32i8: case v16i16: case v8i32: case v4i64: case v2i128: case v1i256: case v16f16: case v8f32: case v4f64: return 256;
      case i512: case v512i1: case v256i2: case v128i4: case v64i8: case v32i16: case v16i32: case v8i64: case v4i128: case v2i256: case v1i512: case v32f16: case v16f32: case v8f64: return 512;
      case i1024: case v1024i1: case v512i2: case v256i4: case v128i8: case v64i16: case v32i32: case v16i64: case v8i128: case v4i256: case v2i512: case v1i1024: case v64f16: case v32f32: case v16f64: return 1024;

      case x86mmx: return 64;
      case f80 :  return 80;
      case f128:
      case ppcf128: return 128;
      }
    }



    unsigned getScalarSizeInBits() const {
      return getScalarType().getSizeInBits();
    }

    /// getStoreSize - Return the number of bytes overwritten by a store
    /// of the specified value type.
    unsigned getStoreSize() const {
      return (getSizeInBits() + 7) / 8;
    }

    /// getStoreSizeInBits - Return the number of bits overwritten by a store
    /// of the specified value type.
    unsigned getStoreSizeInBits() const {
      return getStoreSize() * 8;
    }

    /// Return true if this has more bits than VT.
    bool bitsGT(MVT VT) const {
      return getSizeInBits() > VT.getSizeInBits();
    }

    /// Return true if this has no less bits than VT.
    bool bitsGE(MVT VT) const {
      return getSizeInBits() >= VT.getSizeInBits();
    }

    /// Return true if this has less bits than VT.
    bool bitsLT(MVT VT) const {
      return getSizeInBits() < VT.getSizeInBits();
    }

    /// Return true if this has no more bits than VT.
    bool bitsLE(MVT VT) const {
      return getSizeInBits() <= VT.getSizeInBits();
    }


    static MVT getFloatingPointVT(unsigned BitWidth) {
      switch (BitWidth) {
      default:
        llvm_unreachable("Bad bit width!");
      case 16:
        return MVT::f16;
      case 32:
        return MVT::f32;
      case 64:
        return MVT::f64;
      case 80:
        return MVT::f80;
      case 128:
        return MVT::f128;
      }
    }


    static MVT getIntegerVT(unsigned BitWidth) {
      switch (BitWidth) {
      default:
        return (MVT::SimpleValueType)(MVT::INVALID_SIMPLE_VALUE_TYPE);
      case 1:
        return MVT::i1;
      case 2:
        return MVT::i2;
      case 4:
        return MVT::i4;
      case 8:
        return MVT::i8;
      case 16:
        return MVT::i16;
      case 32:
        return MVT::i32;
      case 64:
        return MVT::i64;
      case 128:
        return MVT::i128;
      case 256:
        return MVT::i256;
      case 512:
        return MVT::i512;
      case 1024:
        return MVT::i1024;

      }
    }



    static MVT getVectorVT(MVT VT, unsigned NumElements) {
      switch (VT.SimpleTy) {
      default:
        break;
      case MVT::i1:
        if (NumElements == 1)  return MVT::v1i1;
        if (NumElements == 2)  return MVT::v2i1;
        if (NumElements == 4)  return MVT::v4i1;
        if (NumElements == 8)  return MVT::v8i1;
        if (NumElements == 16)  return MVT::v16i1;
        if (NumElements == 32)  return MVT::v32i1;
        if (NumElements == 64)  return MVT::v64i1;
        if (NumElements == 128)  return MVT::v128i1;
        if (NumElements == 256)  return MVT::v256i1;
        if (NumElements == 512)  return MVT::v512i1;
        if (NumElements == 1024)  return MVT::v1024i1;
        break;
      case MVT::i2:
        if (NumElements == 1)  return MVT::v1i2;
        if (NumElements == 2)  return MVT::v2i2;
        if (NumElements == 4)  return MVT::v4i2;
        if (NumElements == 8)  return MVT::v8i2;
        if (NumElements == 16)  return MVT::v16i2;
        if (NumElements == 32)  return MVT::v32i2;
        if (NumElements == 64)  return MVT::v64i2;
        if (NumElements == 128)  return MVT::v128i2;
        if (NumElements == 256)  return MVT::v256i2;
        if (NumElements == 512)  return MVT::v512i2;
        break;
      case MVT::i4:
        if (NumElements == 1)  return MVT::v1i4;
        if (NumElements == 2)  return MVT::v2i4;
        if (NumElements == 4)  return MVT::v4i4;
        if (NumElements == 8)  return MVT::v8i4;
        if (NumElements == 16)  return MVT::v16i4;
        if (NumElements == 32)  return MVT::v32i4;
        if (NumElements == 64)  return MVT::v64i4;
        if (NumElements == 128)  return MVT::v128i4;
        if (NumElements == 256)  return MVT::v256i4;
        break;
      case MVT::i8:
        if (NumElements == 1)  return MVT::v1i8;
        if (NumElements == 2)  return MVT::v2i8;
        if (NumElements == 4)  return MVT::v4i8;
        if (NumElements == 8)  return MVT::v8i8;
        if (NumElements == 16)  return MVT::v16i8;
        if (NumElements == 32)  return MVT::v32i8;
        if (NumElements == 64)  return MVT::v64i8;
        if (NumElements == 128)  return MVT::v128i8;
        break;
      case MVT::i16:
        if (NumElements == 1)  return MVT::v1i16;
        if (NumElements == 2)  return MVT::v2i16;
        if (NumElements == 4)  return MVT::v4i16;
        if (NumElements == 8)  return MVT::v8i16;
        if (NumElements == 16)  return MVT::v16i16;
        if (NumElements == 32)  return MVT::v32i16;
        if (NumElements == 64)  return MVT::v64i16;
        break;
      case MVT::i32:
        if (NumElements == 1)  return MVT::v1i32;
        if (NumElements == 2)  return MVT::v2i32;
        if (NumElements == 4)  return MVT::v4i32;
        if (NumElements == 8)  return MVT::v8i32;
        if (NumElements == 16)  return MVT::v16i32;
        if (NumElements == 32)  return MVT::v32i32;
        break;
      case MVT::i64:
        if (NumElements == 1)  return MVT::v1i64;
        if (NumElements == 2)  return MVT::v2i64;
        if (NumElements == 4)  return MVT::v4i64;
        if (NumElements == 8)  return MVT::v8i64;
        if (NumElements == 16)  return MVT::v16i64;
        break;
      case MVT::i128:
        if (NumElements == 1)  return MVT::v1i128;
        if (NumElements == 2)  return MVT::v2i128;
        if (NumElements == 4)  return MVT::v4i128;
        if (NumElements == 8)  return MVT::v8i128;
        break;
      case MVT::i256:
        if (NumElements == 1)  return MVT::v1i256;
        if (NumElements == 2)  return MVT::v2i256;
        if (NumElements == 4)  return MVT::v4i256;
        break;
      case MVT::i512:
        if (NumElements == 1)  return MVT::v1i512;
        if (NumElements == 2)  return MVT::v2i512;
        break;
      case MVT::i1024:
        if (NumElements == 1)  return MVT::v1i1024;
        break;
      case MVT::f16:
        if (NumElements == 1)  return MVT::v1f16;
        if (NumElements == 2)  return MVT::v2f16;
        if (NumElements == 4)  return MVT::v4f16;
        if (NumElements == 8)  return MVT::v8f16;
        if (NumElements == 16)  return MVT::v16f16;
        if (NumElements == 32)  return MVT::v32f16;
        if (NumElements == 64)  return MVT::v64f16;
        break;
      case MVT::f32:
        if (NumElements == 1)  return MVT::v1f32;
        if (NumElements == 2)  return MVT::v2f32;
        if (NumElements == 4)  return MVT::v4f32;
        if (NumElements == 8)  return MVT::v8f32;
        if (NumElements == 16)  return MVT::v16f32;
        if (NumElements == 32)  return MVT::v32f32;
        break;
      case MVT::f64:
        if (NumElements == 1)  return MVT::v1f64;
        if (NumElements == 2)  return MVT::v2f64;
        if (NumElements == 4)  return MVT::v4f64;
        if (NumElements == 8)  return MVT::v8f64;
        if (NumElements == 16)  return MVT::v16f64;
        break;

      }
      return (MVT::SimpleValueType)(MVT::INVALID_SIMPLE_VALUE_TYPE);
    }


    /// Return the value type corresponding to the specified type.  This returns
    /// all pointers as iPTR.  If HandleUnknown is true, unknown types are
    /// returned as Other, otherwise they are invalid.
    static MVT getVT(Type *Ty, bool HandleUnknown = false);

  };
  /// EVT - Extended Value Type.  Capable of holding value types which are not
  /// native for any processor (such as the i12345 type), as well as the types
  /// a MVT can represent.
  struct EVT {
  private:
    MVT V;
    Type *LLVMTy;

  public:
    EVT() : V((MVT::SimpleValueType)(MVT::INVALID_SIMPLE_VALUE_TYPE)),
            LLVMTy(0) {}
    EVT(MVT::SimpleValueType SVT) : V(SVT), LLVMTy(0) { }
    EVT(MVT S) : V(S), LLVMTy(0) {}

    bool operator==(EVT VT) const {
      return !(*this != VT);
    }
    bool operator!=(EVT VT) const {
      if (V.SimpleTy != VT.V.SimpleTy)
        return true;
      if (V.SimpleTy < 0)
        return LLVMTy != VT.LLVMTy;
      return false;
    }

    /// getFloatingPointVT - Returns the EVT that represents a floating point
    /// type with the given number of bits.  There are two floating point types
    /// with 128 bits - this returns f128 rather than ppcf128.
    static EVT getFloatingPointVT(unsigned BitWidth) {
      return MVT::getFloatingPointVT(BitWidth);
    }

    /// getIntegerVT - Returns the EVT that represents an integer with the given
    /// number of bits.
    static EVT getIntegerVT(LLVMContext &Context, unsigned BitWidth) {
      MVT M = MVT::getIntegerVT(BitWidth);
      if (M.SimpleTy >= 0)
        return M;
      return getExtendedIntegerVT(Context, BitWidth);
    }

    /// getVectorVT - Returns the EVT that represents a vector NumElements in
    /// length, where each element is of type VT.
    static EVT getVectorVT(LLVMContext &Context, EVT VT, unsigned NumElements) {
      MVT M = MVT::getVectorVT(VT.V, NumElements);
      if (M.SimpleTy >= 0)
        return M;
      return getExtendedVectorVT(Context, VT, NumElements);
    }

    /// changeVectorElementTypeToInteger - Return a vector with the same number
    /// of elements as this vector, but with the element type converted to an
    /// integer type with the same bitwidth.
    EVT changeVectorElementTypeToInteger() const {
      if (!isSimple())
        return changeExtendedVectorElementTypeToInteger();
      MVT EltTy = getSimpleVT().getVectorElementType();
      unsigned BitWidth = EltTy.getSizeInBits();
      MVT IntTy = MVT::getIntegerVT(BitWidth);
      MVT VecTy = MVT::getVectorVT(IntTy, getVectorNumElements());
      assert(VecTy.SimpleTy >= 0 &&
             "Simple vector VT not representable by simple integer vector VT!");
      return VecTy;
    }

    /// isSimple - Test if the given EVT is simple (as opposed to being
    /// extended).
    bool isSimple() const {
      return V.SimpleTy >= 0;
    }

    /// isExtended - Test if the given EVT is extended (as opposed to
    /// being simple).
    bool isExtended() const {
      return !isSimple();
    }

    /// isFloatingPoint - Return true if this is a FP, or a vector FP type.
    bool isFloatingPoint() const {
      return isSimple() ? V.isFloatingPoint() : isExtendedFloatingPoint();
    }

    /// isInteger - Return true if this is an integer, or a vector integer type.
    bool isInteger() const {
      return isSimple() ? V.isInteger() : isExtendedInteger();
    }

    /// isVector - Return true if this is a vector value type.
    bool isVector() const {
      return isSimple() ? V.isVector() : isExtendedVector();
    }

    /// is16BitVector - Return true if this is a 16-bit vector type.
    bool is16BitVector() const {
      return isSimple() ? V.is16BitVector() : isExtended16BitVector();
    }

    /// is32BitVector - Return true if this is a 32-bit vector type.
    bool is32BitVector() const {
      return isSimple() ? V.is32BitVector() : isExtended32BitVector();
    }

    /// is64BitVector - Return true if this is a 64-bit vector type.
    bool is64BitVector() const {
      return isSimple() ? V.is64BitVector() : isExtended64BitVector();
    }

    /// is128BitVector - Return true if this is a 128-bit vector type.
    bool is128BitVector() const {
      return isSimple() ? V.is128BitVector() : isExtended128BitVector();
    }

    /// is256BitVector - Return true if this is a 256-bit vector type.
    bool is256BitVector() const {
      return isSimple() ? V.is256BitVector() : isExtended256BitVector();
    }

    /// is512BitVector - Return true if this is a 512-bit vector type.
    bool is512BitVector() const {
      return isSimple() ? V.is512BitVector() : isExtended512BitVector();
    }

    /// is1024BitVector - Return true if this is a 1024-bit vector type.
    bool is1024BitVector() const {
      return isSimple() ? V.is1024BitVector() : isExtended1024BitVector();
    }

    /// isOverloaded - Return true if this is an overloaded type for TableGen.
    bool isOverloaded() const {
      return (V==MVT::iAny || V==MVT::fAny || V==MVT::vAny || V==MVT::iPTRAny);
    }

    /// isByteSized - Return true if the bit size is a multiple of 8.
    bool isByteSized() const {
      return (getSizeInBits() & 7) == 0;
    }

    /// isRound - Return true if the size is a power-of-two number of bytes.
    bool isRound() const {
      unsigned BitSize = getSizeInBits();
      return BitSize >= 8 && !(BitSize & (BitSize - 1));
    }

    /// bitsEq - Return true if this has the same number of bits as VT.
    bool bitsEq(EVT VT) const {
      if (EVT::operator==(VT)) return true;
      return getSizeInBits() == VT.getSizeInBits();
    }

    /// bitsGT - Return true if this has more bits than VT.
    bool bitsGT(EVT VT) const {
      if (EVT::operator==(VT)) return false;
      return getSizeInBits() > VT.getSizeInBits();
    }

    /// bitsGE - Return true if this has no less bits than VT.
    bool bitsGE(EVT VT) const {
      if (EVT::operator==(VT)) return true;
      return getSizeInBits() >= VT.getSizeInBits();
    }

    /// bitsLT - Return true if this has less bits than VT.
    bool bitsLT(EVT VT) const {
      if (EVT::operator==(VT)) return false;
      return getSizeInBits() < VT.getSizeInBits();
    }

    /// bitsLE - Return true if this has no more bits than VT.
    bool bitsLE(EVT VT) const {
      if (EVT::operator==(VT)) return true;
      return getSizeInBits() <= VT.getSizeInBits();
    }


    /// getSimpleVT - Return the SimpleValueType held in the specified
    /// simple EVT.
    MVT getSimpleVT() const {
      assert(isSimple() && "Expected a SimpleValueType!");
      return V;
    }

    /// getScalarType - If this is a vector type, return the element type,
    /// otherwise return this.
    EVT getScalarType() const {
      return isVector() ? getVectorElementType() : *this;
    }

    /// getVectorElementType - Given a vector type, return the type of
    /// each element.
    EVT getVectorElementType() const {
      assert(isVector() && "Invalid vector type!");
      if (isSimple())
        return V.getVectorElementType();
      return getExtendedVectorElementType();
    }

    /// getVectorNumElements - Given a vector type, return the number of
    /// elements it contains.
    unsigned getVectorNumElements() const {
      assert(isVector() && "Invalid vector type!");
      if (isSimple())
        return V.getVectorNumElements();
      return getExtendedVectorNumElements();
    }

    /// getSizeInBits - Return the size of the specified value type in bits.
    unsigned getSizeInBits() const {
      if (isSimple())
        return V.getSizeInBits();
      return getExtendedSizeInBits();
    }

    /// getStoreSize - Return the number of bytes overwritten by a store
    /// of the specified value type.
    unsigned getStoreSize() const {
      return (getSizeInBits() + 7) / 8;
    }

    /// getStoreSizeInBits - Return the number of bits overwritten by a store
    /// of the specified value type.
    unsigned getStoreSizeInBits() const {
      return getStoreSize() * 8;
    }

    /// getRoundIntegerType - Rounds the bit-width of the given integer EVT up
    /// to the nearest power of two (and at least to eight), and returns the
    /// integer EVT with that number of bits.
    EVT getRoundIntegerType(LLVMContext &Context) const {
      assert(isInteger() && !isVector() && "Invalid integer type!");
      unsigned BitWidth = getSizeInBits();
      if (BitWidth <= 8)
        return EVT(MVT::i8);
      return getIntegerVT(Context, 1 << Log2_32_Ceil(BitWidth));
    }

    /// getHalfSizedIntegerVT - Finds the smallest simple value type that is
    /// greater than or equal to half the width of this EVT. If no simple
    /// value type can be found, an extended integer value type of half the
    /// size (rounded up) is returned.
    EVT getHalfSizedIntegerVT(LLVMContext &Context) const {
      assert(isInteger() && !isVector() && "Invalid integer type!");
      unsigned EVTSize = getSizeInBits();
      for (unsigned IntVT = MVT::FIRST_INTEGER_VALUETYPE;
          IntVT <= MVT::LAST_INTEGER_VALUETYPE; ++IntVT) {
        EVT HalfVT = EVT((MVT::SimpleValueType)IntVT);
        if (HalfVT.getSizeInBits() * 2 >= EVTSize)
          return HalfVT;
      }
      return getIntegerVT(Context, (EVTSize + 1) / 2);
    }

    /// isPow2VectorType - Returns true if the given vector is a power of 2.
    bool isPow2VectorType() const {
      unsigned NElts = getVectorNumElements();
      return !(NElts & (NElts - 1));
    }

    /// getPow2VectorType - Widens the length of the given vector EVT up to
    /// the nearest power of 2 and returns that type.
    EVT getPow2VectorType(LLVMContext &Context) const {
      if (!isPow2VectorType()) {
        unsigned NElts = getVectorNumElements();
        unsigned Pow2NElts = 1 <<  Log2_32_Ceil(NElts);
        return EVT::getVectorVT(Context, getVectorElementType(), Pow2NElts);
      }
      else {
        return *this;
      }
    }

    /// getEVTString - This function returns value type as a string,
    /// e.g. "i32".
    std::string getEVTString() const;

    /// getTypeForEVT - This method returns an LLVM type corresponding to the
    /// specified EVT.  For integer types, this returns an unsigned type.  Note
    /// that this will abort for types that cannot be represented.
    Type *getTypeForEVT(LLVMContext &Context) const;

    /// getEVT - Return the value type corresponding to the specified type.
    /// This returns all pointers as iPTR.  If HandleUnknown is true, unknown
    /// types are returned as Other, otherwise they are invalid.
    static EVT getEVT(Type *Ty, bool HandleUnknown = false);

    intptr_t getRawBits() const {
      if (isSimple())
        return V.SimpleTy;
      else
        return (intptr_t)(LLVMTy);
    }

    /// compareRawBits - A meaningless but well-behaved order, useful for
    /// constructing containers.
    struct compareRawBits {
      bool operator()(EVT L, EVT R) const {
        if (L.V.SimpleTy == R.V.SimpleTy)
          return L.LLVMTy < R.LLVMTy;
        else
          return L.V.SimpleTy < R.V.SimpleTy;
      }
    };

  private:
    // Methods for handling the Extended-type case in functions above.
    // These are all out-of-line to prevent users of this header file
    // from having a dependency on Type.h.
    EVT changeExtendedVectorElementTypeToInteger() const;
    static EVT getExtendedIntegerVT(LLVMContext &C, unsigned BitWidth);
    static EVT getExtendedVectorVT(LLVMContext &C, EVT VT,
                                   unsigned NumElements);
    bool isExtendedFloatingPoint() const;
    bool isExtendedInteger() const;
    bool isExtendedVector() const;
    bool isExtended16BitVector() const;
    bool isExtended32BitVector() const;
    bool isExtended64BitVector() const;
    bool isExtended128BitVector() const;
    bool isExtended256BitVector() const;
    bool isExtended512BitVector() const;
    bool isExtended1024BitVector() const;
    EVT getExtendedVectorElementType() const;
    unsigned getExtendedVectorNumElements() const;
    unsigned getExtendedSizeInBits() const;
  };


} // End llvm namespace

#endif
