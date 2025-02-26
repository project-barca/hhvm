// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the "hack" directory of this source tree.
//
// @generated SignedSource<<2a4f4fa2931519884ee6d2f8e305d959>>
//
// To regenerate this file, run:
//   hphp/hack/src/oxidized_regen.sh

use arena_trait::TrivialDrop;
use no_pos_hash::NoPosHash;
use ocamlrep_derive::FromOcamlRepIn;
use ocamlrep_derive::ToOcamlRep;
use serde::Deserialize;
use serde::Serialize;

pub use crate::pos::map;
#[allow(unused_imports)]
use crate::*;

#[rust_to_ocaml(attr = "deriving (eq, ord, show)")]
pub type PosOrDecl<'a> = pos::Pos<'a>;

/// The decl and file of a position.
#[derive(
    Clone,
    Debug,
    Deserialize,
    Eq,
    FromOcamlRepIn,
    Hash,
    NoPosHash,
    Ord,
    PartialEq,
    PartialOrd,
    Serialize,
    ToOcamlRep
)]
#[repr(C)]
pub struct Ctx<'a> {
    #[serde(deserialize_with = "arena_deserializer::arena", borrow)]
    pub decl: Option<decl_reference::DeclReference<'a>>,
    #[serde(deserialize_with = "arena_deserializer::arena", borrow)]
    pub file: &'a relative_path::RelativePath<'a>,
}
impl<'a> TrivialDrop for Ctx<'a> {}
arena_deserializer::impl_deserialize_in_arena!(Ctx<'arena>);
