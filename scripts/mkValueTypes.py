#
# Generate ValueTypes.h and ValueTypes.td
#
# Rob Cameron   May 16, 2014


from string import Template
import re, os, shutil


def calculateTypeData(maxwidth, minvecwidth=32):
    enumTypeLine =    "\t%s\t\t=  %s,\n"
    defTypeLine = "def %s\t: ValueType<%s , %s>;\n"
    fw = 1
    fws = []
    while (fw <= maxwidth): fws.append(fw); fw = fw * 2
    enumVal = 0
    VectorSizeMap = {}
    for f in fws: VectorSizeMap[f] = []
    VectorFieldSizeMap = {}
    for f in fws: VectorFieldSizeMap[f] = []
    VTmap = {}
    VTmap['integer_type_lines'] = ''
    VTmap['int_value_type_defs'] = ''
    intTypes = ""
    intVecTypes = ""
    for fw in fws:
        enumVal += 1
        iType = "i%s" % fw
        VTmap['integer_type_lines'] += enumTypeLine % (iType, enumVal)
        VTmap['int_value_type_defs'] += defTypeLine % (iType, fw, enumVal)
    VTmap['first_integer_type'] = 'i%s' % fws[0]
    VTmap['last_integer_type'] = 'i%s' % fws[-1]
    VTmap['f16'] = enumVal + 1
    VTmap['f32'] = VTmap['f16'] + 1
    VTmap['f64'] = VTmap['f32'] + 1
    VTmap['f80'] = VTmap['f64'] + 1
    VTmap['f128'] = VTmap['f80'] + 1
    VTmap['ppcf128'] = VTmap['f128'] + 1

    enumVal = enumVal + 6
    VTmap['integer_vector_type_lines'] = ''
    VTmap['int_vector_value_type_defs'] = ''
    for fw in fws:
        for fieldcount in [f for f in fws if f * fw <= maxwidth and f * fw >= minvecwidth]:
            enumVal += 1
            vType = "v%si%s" % (fieldcount, fw)
            VTmap['integer_vector_type_lines'] += enumTypeLine % (vType, enumVal)
            VTmap['int_vector_value_type_defs'] += defTypeLine % (vType, fieldcount * fw, enumVal)
            VectorSizeMap[fw * fieldcount].append(vType)
            VectorFieldSizeMap[fw].append(vType)
    VTmap['first_integer_vector_type'] = "v%si%s" % (minvecwidth/fws[0], fws[0])
    VTmap['last_integer_vector_type'] = vType
    VTmap['float_vector_type_lines'] = ''
    VTmap['float_vector_value_type_defs'] = ''
    for fpfw in [16,32,64]:
        for fieldcount in [f for f in fws if f * fpfw <= maxwidth]:
            enumVal += 1
            vType = "v%sf%s" % (fieldcount, fpfw)
            VTmap['float_vector_type_lines'] += enumTypeLine % (vType, enumVal)
            VTmap['float_vector_value_type_defs'] += defTypeLine % (vType, fieldcount * fpfw, enumVal)
            VectorSizeMap[fpfw * fieldcount].append(vType)
            VectorFieldSizeMap[fpfw].append(vType)
    VTmap['first_float_vector_type'] = "v%sf%s" % (1, 16)
    VTmap['last_float_vector_type'] = vType
    VTmap['x86mmx'] = enumVal + 1
    VTmap['Glue'] = VTmap['x86mmx'] + 1
    VTmap['isVoid'] = VTmap['Glue'] + 1
    VTmap['Untyped'] = VTmap['isVoid'] + 1
    VTmap['last_value_type'] = VTmap['Untyped'] + 1
    VTmap['max_value_type'] = ((VTmap['last_value_type'] - 1)/32 + 1)*32
    return (VTmap, VectorSizeMap, VectorFieldSizeMap)


isXXXBitVectorTemplate = r"""
    /// is${width}BitVector - Return true if this is a ${width}-bit vector type.
    bool is${width}BitVector() const {
    return (${typetest});
    }
"""

def make_isXXXBitFunctions(VectorSizeMap):
    fns = ""
    for s in sorted(VectorSizeMap.keys()):
        tests = ["SimpleTy == MVT::%s" % t for t in VectorSizeMap[s]]
        if tests != []: testexpr = " ||\n\t\t".join(tests)
        else: testexpr = 0
        fns += Template(isXXXBitVectorTemplate).substitute(width = repr(s), typetest = testexpr)
    return fns

getVectorElementTypeTemplate = r"""
    MVT getVectorElementType() const {
        switch (SimpleTy) {
        default:
            llvm_unreachable("Not a vector MVT!");
${vector_element_type_cases}
      }
    }

"""

def make_getVectorElementType(VectorFieldSizeMap):
    tCases = ""
    for s in sorted(VectorFieldSizeMap.keys()):
        fCases = ["case %s: " % t for t in VectorFieldSizeMap[s] if t[-3] == 'f']
        iCases = ["case %s: " % t  for t in VectorFieldSizeMap[s] if t[-3] != 'f']
        if fCases != []:
            tCases += "\t%s return f%s;\n" % (" ".join(fCases), s)
        if iCases != []:
            tCases += "\t%s return i%s;\n" % (" ".join(iCases), s)
    return Template(getVectorElementTypeTemplate).substitute(vector_element_type_cases = tCases)

getVectorElementNumElementsTemplate = r"""
    unsigned getVectorNumElements() const {
      switch (SimpleTy) {
      default:
        llvm_unreachable("Not a vector MVT!");
${vector_element_num_cases}
      }
    }

    """
vecNumRe =  re.compile("v([0-9]+)[if][0-9]+")

def make_getVectorNumElements(VectorSizeMap):
    nCases = ""
    byElemCount = {}
    for s in sorted(VectorSizeMap.keys()):
        for t in VectorSizeMap[s]:
            m = vecNumRe.match(t)
            e = m.group(1)
            if not byElemCount.has_key(e): byElemCount[e] = ""
            byElemCount[e] += "case %s: " % t
    for e in sorted(byElemCount.keys()):
        nCases += "      %s return %s;\n" % (byElemCount[e], e)
    return Template(getVectorElementNumElementsTemplate).substitute(vector_element_num_cases = nCases)

getSizeInBitsTemplate = r"""
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
${size_in_bits_cases}
      case x86mmx: return 64;
      case f80 :  return 80;
      case f128:
      case ppcf128: return 128;
      }
    }

"""
def make_getSizeInBits(VectorSizeMap):
    sCases = ""
    for s in sorted(VectorSizeMap.keys()):
        vCases = ["case %s:" % v for v in VectorSizeMap[s]]
        sCases += "      case i%s: " % s
        if s in [16,32,64]: sCases += "case f%s: " % s
        sCases += " ".join(vCases)
        sCases += " return %s;\n" % s
    return Template(getSizeInBitsTemplate).substitute(size_in_bits_cases = sCases)

getIntegerVTTemplate = r"""
    static MVT getIntegerVT(unsigned BitWidth) {
      switch (BitWidth) {
      default:
        return (MVT::SimpleValueType)(MVT::INVALID_SIMPLE_VALUE_TYPE);
${integer_VT_cases}
      }
    }
"""

def make_getIntegerVT(VectorFieldSizeMap):
    iCases = ""
    for s in sorted(VectorFieldSizeMap.keys()):
        iCases += "      case %s:\n        return MVT::i%s;\n" % (s, s)
    return Template(getIntegerVTTemplate).substitute(integer_VT_cases = iCases)



getVectorVTTemplate = r"""
    static MVT getVectorVT(MVT VT, unsigned NumElements) {
      switch (VT.SimpleTy) {
      default:
        break;
${vector_VT_cases}
      }
      return (MVT::SimpleValueType)(MVT::INVALID_SIMPLE_VALUE_TYPE);
    }
"""

def make_getVectorVT(VectorFieldSizeMap):
    iCases = ""
    fCases = ""
    for s in sorted(VectorFieldSizeMap.keys()):
        iCases += "      case MVT::i%s:\n" % s
        for t in VectorFieldSizeMap[s]:
            if t[-3] != 'f':
                # integer case
                m = vecNumRe.match(t)
                e = m.group(1)
                iCases += "        if (NumElements == %s)  return MVT::%s;\n" % (e, t)
        iCases += "        break;\n"
    for s in [16, 32, 64]:
        fCases += "      case MVT::f%s:\n" % s
        for t in VectorFieldSizeMap[s]:
            if t[-3] == 'f':
                # float case
                m = vecNumRe.match(t)
                e = m.group(1)
                fCases += "        if (NumElements == %s)  return MVT::%s;\n" % (e, t)
        fCases += "        break;\n"
    return Template(getVectorVTTemplate).substitute(vector_VT_cases = iCases+fCases)


def makeValueTypes_h(maxwidth, minwidth=32):
    f = open('ValueTypes.h.pytemplate')
    t = Template(f.read())
    f.close()
    (VTmap, VectorSizeMap, VectorFieldSizeMap) = calculateTypeData(maxwidth, minwidth)
    VTmap['isXXXBitFunctions'] = make_isXXXBitFunctions(VectorSizeMap)
    VTmap['getVectorElementType'] = make_getVectorElementType(VectorFieldSizeMap)
    VTmap['getVectorNumElements'] = make_getVectorNumElements(VectorSizeMap)
    VTmap['getSizeInBits'] = make_getSizeInBits(VectorSizeMap)
    VTmap['getIntegerVT'] = make_getIntegerVT(VectorFieldSizeMap)
    VTmap['getVectorVT'] = make_getVectorVT(VectorFieldSizeMap)
    content = t.substitute(VTmap)
    if os.path.exists("ValueTypes.h") and not os.path.exists("ValueTypes.h.bak"):
		shutil.move("ValueTypes.h", "ValueTypes.h.bak")
    f = open("ValueTypes.h", "w")
    f.write(content)
    f.close()

def makeValueTypes_td(maxwidth, minwidth=32):
    f = open('ValueTypes.td.pytemplate')
    t = Template(f.read())
    f.close()
    (VTmap, VectorSizeMap, VectorFieldSizeMap) = calculateTypeData(maxwidth, minwidth)
    content = t.substitute(VTmap)
    if os.path.exists("ValueTypes.td") and  not os.path.exists("ValueTypes.td.bak"):
		shutil.move("ValueTypes.td", "ValueTypes.td.bak")
    f = open("ValueTypes.td", "w")
    f.write(content)
    f.close()

if __name__ == '__main__':
    makeValueTypes_h(1024, 1)
    makeValueTypes_td(1024, 1)
