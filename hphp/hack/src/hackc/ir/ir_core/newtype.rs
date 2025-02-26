// Copyright (c) Facebook, Inc. and its affiliates. All Rights Reserved.

// Re-export some types in from hhbc so users of `ir` don't have to figure out
// which random stuff to get from `ir` and which to get elsewhere.
pub use hhbc::BareThisOp;
pub use hhbc::ClassishKind;
pub use hhbc::CollectionType;
pub use hhbc::ContCheckOp;
pub use hhbc::FCallArgsFlags;
pub use hhbc::FatalOp;
pub use hhbc::IncDecOp;
pub use hhbc::InitPropOp;
pub use hhbc::IsLogAsDynamicCallOp;
pub use hhbc::IsTypeOp;
pub use hhbc::IterId;
pub use hhbc::MOpMode;
pub use hhbc::ObjMethodOp;
pub use hhbc::QueryMOp;
pub use hhbc::ReadonlyOp;
pub use hhbc::SetOpOp;
pub use hhbc::SpecialClsRef;
pub use hhbc::SrcLoc;
pub use hhbc::TypeStructResolveOp;
use newtype::newtype_int;

use crate::string_intern::StringInterner;
use crate::UnitStringId;

macro_rules! interned_hhbc_id {
    ($name: ident, $hhbc: ident) => {
        #[derive(Copy, Clone, Debug, Eq, PartialEq, Hash)]
        pub struct $name {
            pub id: UnitStringId,
        }

        impl $name {
            pub fn new(id: UnitStringId) -> Self {
                Self { id }
            }

            pub fn from_hhbc<'a>(id: hhbc::$hhbc<'a>, strings: &mut StringInterner<'a>) -> Self {
                Self::new(strings.intern_str(id.as_ffi_str()))
            }

            pub fn from_str<'a>(
                name: &str,
                alloc: &'a bumpalo::Bump,
                strings: &mut StringInterner<'a>,
            ) -> Self {
                let name = ffi::Str::new_str(alloc, name);
                Self::new(strings.intern_str(name))
            }

            pub fn to_hhbc<'a>(
                self,
                alloc: &'a bumpalo::Bump,
                strings: &StringInterner<'a>,
            ) -> hhbc::$hhbc<'a> {
                hhbc::$hhbc::new(strings.lookup(self.id).to_ffi_str(alloc))
            }
        }
    };
}

interned_hhbc_id!(ClassId, ClassName);
pub type ClassIdMap<T> = indexmap::map::IndexMap<ClassId, T, newtype::BuildIdHasher<u32>>;

interned_hhbc_id!(ConstId, ConstName);
interned_hhbc_id!(FunctionId, FunctionName);
interned_hhbc_id!(MethodId, MethodName);
interned_hhbc_id!(PropId, PropName);

// A BlockId represents a Block within a Func.
newtype_int!(BlockId, u32, BlockIdMap, BlockIdSet);

// A InstrId represents a Instr within a Func.
newtype_int!(InstrId, u32, InstrIdMap, InstrIdSet);
pub type InstrIdIndexSet = indexmap::set::IndexSet<InstrId, newtype::BuildIdHasher<u32>>;

// A LiteralId represents a Literal within a Func.
newtype_int!(LiteralId, u32, LiteralIdMap, LiteralIdSet);

// A LocId represents a SrcLoc interned within a Func.
newtype_int!(LocId, u32, LocIdMap, LocIdSet);

// A VarId represents an internal variable which is removed by the ssa pass.
// They are disjoint from LocalIds.
newtype_int!(VarId, u32, VarIdMap, VarIdSet);

/// An ValueId can be either an InstrId or a LiteralId.
///
/// Note that special care has been taken to make sure this encodes to the same
/// size as a u32:
///   InstrId values are encoded as non-negative values.
///   LiteralId values are encoded as binary negation (so negative values).
///   None is encoded as i32::MIN_INT.
#[derive(Copy, Clone, Debug, Eq, Hash, PartialEq)]
pub struct ValueId {
    raw: i32,
}

impl ValueId {
    const NONE: i32 = i32::MIN;

    pub fn raw(self) -> i32 {
        self.raw
    }

    pub fn from_instr(idx: InstrId) -> Self {
        assert!(idx != InstrId::NONE);
        let InstrId(idx) = idx;
        Self { raw: idx as i32 }
    }

    pub fn from_literal(idx: LiteralId) -> Self {
        assert!(idx != LiteralId::NONE);
        let LiteralId(idx) = idx;
        Self { raw: !(idx as i32) }
    }

    pub fn none() -> Self {
        Self { raw: Self::NONE }
    }

    pub fn expect_instr(self, msg: &str) -> InstrId {
        if self.raw >= 0 {
            InstrId(self.raw as u32)
        } else {
            panic!("{}", msg);
        }
    }

    pub fn full(self) -> FullInstrId {
        if self.raw >= 0 {
            FullInstrId::Instr(InstrId(self.raw as u32))
        } else if self.raw == Self::NONE {
            FullInstrId::None
        } else {
            FullInstrId::Literal(LiteralId((!self.raw) as u32))
        }
    }

    pub fn literal(self) -> Option<LiteralId> {
        if self.raw >= 0 || self.raw == Self::NONE {
            None
        } else {
            Some(LiteralId((!self.raw) as u32))
        }
    }

    pub fn instr(self) -> Option<InstrId> {
        if self.raw >= 0 {
            Some(InstrId(self.raw as u32))
        } else {
            None
        }
    }

    pub fn is_literal(self) -> bool {
        self.raw < 0 && self.raw != Self::NONE
    }

    pub fn is_instr(self) -> bool {
        self.raw >= 0
    }

    pub fn is_none(self) -> bool {
        self.raw == Self::NONE
    }
}

// A 'FullInstrId' can be used with match but takes more memory than
// ValueId. Note that the Literal and Instr variants will never contain
// LiteralId::NONE or InstrId::NONE.
pub enum FullInstrId {
    Instr(InstrId),
    Literal(LiteralId),
    None,
}

pub type ValueIdMap<V> = std::collections::HashMap<ValueId, V, newtype::BuildIdHasher<u32>>;
pub type ValueIdSet = std::collections::HashSet<ValueId, newtype::BuildIdHasher<u32>>;
