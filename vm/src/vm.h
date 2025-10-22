#pragma once

#include "binary.h"
#include <stdint.h>

enum InstructionKind {
    InstructionNoop = 0x00,

    InstructionJmp = 0x01,
    InstructionJEQZ = 0x02,

    InstructionPush = 0x10,

    InstructionAdd = 0x20,
    InstructionSub = 0x30,
    InstructionMul = 0x40,

    InstructionEq = 0xa0,
    InstructionLt = 0xa1,
    InstructionLe = 0xa2,
    InstructionGt = 0xa3,
    InstructionGe = 0xa4,

    InstructionLAnd = 0xaa,
    InstructionLOr = 0xab,
    InstructionLNeg = 0xb0,

    InstructionFetch = 0xc0,
    InstructionStore = 0xc1,

    InstructionPrintC = 0xd0,
    InstructionPrintV = 0xd1,
};

void run_vm(struct BinaryFile *bin);