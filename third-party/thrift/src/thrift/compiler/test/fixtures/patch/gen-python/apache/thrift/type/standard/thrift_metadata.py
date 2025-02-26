#
# Autogenerated by Thrift
#
# DO NOT EDIT
#  @generated
#

from __future__ import annotations

import apache.thrift.metadata.thrift_types as _fbthrift_metadata


import facebook.thrift.annotation.java.thrift_metadata

import facebook.thrift.annotation.thrift.thrift_metadata

# TODO (ffrancet): This general pattern can be optimized by using tuples and dicts
# instead of re-generating thrift structs
def _fbthrift_gen_metadata_struct_UriStruct(metadata_struct: _fbthrift_metadata.ThriftMetadata) -> _fbthrift_metadata.ThriftMetadata:
    qualified_name = "standard.UriStruct"

    if qualified_name in metadata_struct.structs:
        return metadata_struct
    fields = [
        _fbthrift_metadata.ThriftField(id=1, type=_fbthrift_metadata.ThriftType(t_primitive=_fbthrift_metadata.ThriftPrimitiveType.THRIFT_STRING_TYPE), name="scheme", is_optional=False, structured_annotations=[
        ]),
        _fbthrift_metadata.ThriftField(id=2, type=_fbthrift_metadata.ThriftType(t_list=_fbthrift_metadata.ThriftListType(valueType=_fbthrift_metadata.ThriftType(t_primitive=_fbthrift_metadata.ThriftPrimitiveType.THRIFT_STRING_TYPE))), name="domain", is_optional=False, structured_annotations=[
        ]),
        _fbthrift_metadata.ThriftField(id=4, type=_fbthrift_metadata.ThriftType(t_list=_fbthrift_metadata.ThriftListType(valueType=_fbthrift_metadata.ThriftType(t_primitive=_fbthrift_metadata.ThriftPrimitiveType.THRIFT_STRING_TYPE))), name="path", is_optional=False, structured_annotations=[
        ]),
        _fbthrift_metadata.ThriftField(id=5, type=_fbthrift_metadata.ThriftType(t_map=_fbthrift_metadata.ThriftMapType(keyType=_fbthrift_metadata.ThriftType(t_primitive=_fbthrift_metadata.ThriftPrimitiveType.THRIFT_STRING_TYPE),valueType=_fbthrift_metadata.ThriftType(t_primitive=_fbthrift_metadata.ThriftPrimitiveType.THRIFT_STRING_TYPE))), name="query", is_optional=False, structured_annotations=[
        ]),
        _fbthrift_metadata.ThriftField(id=6, type=_fbthrift_metadata.ThriftType(t_primitive=_fbthrift_metadata.ThriftPrimitiveType.THRIFT_STRING_TYPE), name="fragment", is_optional=False, structured_annotations=[
        ]),
    ]
    struct_dict = dict(metadata_struct.structs)
    struct_dict[qualified_name] = _fbthrift_metadata.ThriftStruct(name=qualified_name, fields=fields,
        is_union=False,
        structured_annotations=[
            _fbthrift_metadata.ThriftConstStruct(type=_fbthrift_metadata.ThriftStructType(name="thrift.Experimental"), fields= {  }),
        ])
    new_struct = metadata_struct(structs=struct_dict)

     # scheme
     # domain
     # path
     # key
     # val  # query
     # fragment

    return new_struct
def gen_metadata_struct_UriStruct() -> _fbthrift_metadata.ThriftMetadata:
    return _fbthrift_gen_metadata_struct_UriStruct(_fbthrift_metadata.ThriftMetadata(structs={}, enums={}, exceptions={}, services={}))

# TODO (ffrancet): This general pattern can be optimized by using tuples and dicts
# instead of re-generating thrift structs
def _fbthrift_gen_metadata_struct_TypeUri(metadata_struct: _fbthrift_metadata.ThriftMetadata) -> _fbthrift_metadata.ThriftMetadata:
    qualified_name = "standard.TypeUri"

    if qualified_name in metadata_struct.structs:
        return metadata_struct
    fields = [
        _fbthrift_metadata.ThriftField(id=1, type=_fbthrift_metadata.ThriftType(t_primitive=_fbthrift_metadata.ThriftPrimitiveType.THRIFT_STRING_TYPE), name="uri", is_optional=False, structured_annotations=[
        ]),
        _fbthrift_metadata.ThriftField(id=2, type=_fbthrift_metadata.ThriftType(t_primitive=_fbthrift_metadata.ThriftPrimitiveType.THRIFT_BINARY_TYPE), name="typeHashPrefixSha2_256", is_optional=False, structured_annotations=[
        ]),
    ]
    struct_dict = dict(metadata_struct.structs)
    struct_dict[qualified_name] = _fbthrift_metadata.ThriftStruct(name=qualified_name, fields=fields,
        is_union=True,
        structured_annotations=[
        ])
    new_struct = metadata_struct(structs=struct_dict)

     # uri
     # typeHashPrefixSha2_256

    return new_struct
def gen_metadata_struct_TypeUri() -> _fbthrift_metadata.ThriftMetadata:
    return _fbthrift_gen_metadata_struct_TypeUri(_fbthrift_metadata.ThriftMetadata(structs={}, enums={}, exceptions={}, services={}))

# TODO (ffrancet): This general pattern can be optimized by using tuples and dicts
# instead of re-generating thrift structs
def _fbthrift_gen_metadata_struct_TypeName(metadata_struct: _fbthrift_metadata.ThriftMetadata) -> _fbthrift_metadata.ThriftMetadata:
    qualified_name = "standard.TypeName"

    if qualified_name in metadata_struct.structs:
        return metadata_struct
    fields = [
        _fbthrift_metadata.ThriftField(id=1, type=_fbthrift_metadata.ThriftType(t_enum=_fbthrift_metadata.ThriftEnumType(name="standard.Void")), name="boolType", is_optional=False, structured_annotations=[
        ]),
        _fbthrift_metadata.ThriftField(id=2, type=_fbthrift_metadata.ThriftType(t_enum=_fbthrift_metadata.ThriftEnumType(name="standard.Void")), name="byteType", is_optional=False, structured_annotations=[
        ]),
        _fbthrift_metadata.ThriftField(id=3, type=_fbthrift_metadata.ThriftType(t_enum=_fbthrift_metadata.ThriftEnumType(name="standard.Void")), name="i16Type", is_optional=False, structured_annotations=[
        ]),
        _fbthrift_metadata.ThriftField(id=4, type=_fbthrift_metadata.ThriftType(t_enum=_fbthrift_metadata.ThriftEnumType(name="standard.Void")), name="i32Type", is_optional=False, structured_annotations=[
        ]),
        _fbthrift_metadata.ThriftField(id=5, type=_fbthrift_metadata.ThriftType(t_enum=_fbthrift_metadata.ThriftEnumType(name="standard.Void")), name="i64Type", is_optional=False, structured_annotations=[
        ]),
        _fbthrift_metadata.ThriftField(id=6, type=_fbthrift_metadata.ThriftType(t_enum=_fbthrift_metadata.ThriftEnumType(name="standard.Void")), name="floatType", is_optional=False, structured_annotations=[
        ]),
        _fbthrift_metadata.ThriftField(id=7, type=_fbthrift_metadata.ThriftType(t_enum=_fbthrift_metadata.ThriftEnumType(name="standard.Void")), name="doubleType", is_optional=False, structured_annotations=[
        ]),
        _fbthrift_metadata.ThriftField(id=8, type=_fbthrift_metadata.ThriftType(t_enum=_fbthrift_metadata.ThriftEnumType(name="standard.Void")), name="stringType", is_optional=False, structured_annotations=[
        ]),
        _fbthrift_metadata.ThriftField(id=9, type=_fbthrift_metadata.ThriftType(t_enum=_fbthrift_metadata.ThriftEnumType(name="standard.Void")), name="binaryType", is_optional=False, structured_annotations=[
        ]),
        _fbthrift_metadata.ThriftField(id=10, type=_fbthrift_metadata.ThriftType(t_union=_fbthrift_metadata.ThriftUnionType(name="standard.TypeUri")), name="enumType", is_optional=False, structured_annotations=[
        ]),
        _fbthrift_metadata.ThriftField(id=11, type=_fbthrift_metadata.ThriftType(t_union=_fbthrift_metadata.ThriftUnionType(name="standard.TypeUri")), name="structType", is_optional=False, structured_annotations=[
        ]),
        _fbthrift_metadata.ThriftField(id=12, type=_fbthrift_metadata.ThriftType(t_union=_fbthrift_metadata.ThriftUnionType(name="standard.TypeUri")), name="unionType", is_optional=False, structured_annotations=[
        ]),
        _fbthrift_metadata.ThriftField(id=13, type=_fbthrift_metadata.ThriftType(t_union=_fbthrift_metadata.ThriftUnionType(name="standard.TypeUri")), name="exceptionType", is_optional=False, structured_annotations=[
        ]),
        _fbthrift_metadata.ThriftField(id=14, type=_fbthrift_metadata.ThriftType(t_enum=_fbthrift_metadata.ThriftEnumType(name="standard.Void")), name="listType", is_optional=False, structured_annotations=[
        ]),
        _fbthrift_metadata.ThriftField(id=15, type=_fbthrift_metadata.ThriftType(t_enum=_fbthrift_metadata.ThriftEnumType(name="standard.Void")), name="setType", is_optional=False, structured_annotations=[
        ]),
        _fbthrift_metadata.ThriftField(id=16, type=_fbthrift_metadata.ThriftType(t_enum=_fbthrift_metadata.ThriftEnumType(name="standard.Void")), name="mapType", is_optional=False, structured_annotations=[
        ]),
    ]
    struct_dict = dict(metadata_struct.structs)
    struct_dict[qualified_name] = _fbthrift_metadata.ThriftStruct(name=qualified_name, fields=fields,
        is_union=True,
        structured_annotations=[
        ])
    new_struct = metadata_struct(structs=struct_dict)

    new_struct = _fbthrift_gen_metadata_enum_Void(new_struct) # boolType
    new_struct = _fbthrift_gen_metadata_enum_Void(new_struct) # byteType
    new_struct = _fbthrift_gen_metadata_enum_Void(new_struct) # i16Type
    new_struct = _fbthrift_gen_metadata_enum_Void(new_struct) # i32Type
    new_struct = _fbthrift_gen_metadata_enum_Void(new_struct) # i64Type
    new_struct = _fbthrift_gen_metadata_enum_Void(new_struct) # floatType
    new_struct = _fbthrift_gen_metadata_enum_Void(new_struct) # doubleType
    new_struct = _fbthrift_gen_metadata_enum_Void(new_struct) # stringType
    new_struct = _fbthrift_gen_metadata_enum_Void(new_struct) # binaryType
    new_struct = _fbthrift_gen_metadata_struct_TypeUri(new_struct) # enumType
    new_struct = _fbthrift_gen_metadata_struct_TypeUri(new_struct) # structType
    new_struct = _fbthrift_gen_metadata_struct_TypeUri(new_struct) # unionType
    new_struct = _fbthrift_gen_metadata_struct_TypeUri(new_struct) # exceptionType
    new_struct = _fbthrift_gen_metadata_enum_Void(new_struct) # listType
    new_struct = _fbthrift_gen_metadata_enum_Void(new_struct) # setType
    new_struct = _fbthrift_gen_metadata_enum_Void(new_struct) # mapType

    return new_struct
def gen_metadata_struct_TypeName() -> _fbthrift_metadata.ThriftMetadata:
    return _fbthrift_gen_metadata_struct_TypeName(_fbthrift_metadata.ThriftMetadata(structs={}, enums={}, exceptions={}, services={}))



def _fbthrift_gen_metadata_enum_StandardProtocol(metadata_struct: _fbthrift_metadata.ThriftMetadata) -> _fbthrift_metadata.ThriftMetadata:
    qualified_name = "standard.StandardProtocol"

    if qualified_name in metadata_struct.enums:
        return metadata_struct
    elements = {
        0: "Custom",
        1: "Binary",
        2: "Compact",
        3: "Json",
        4: "SimpleJson",
    }
    enum_dict = dict(metadata_struct.enums)
    enum_dict[qualified_name] = _fbthrift_metadata.ThriftEnum(name=qualified_name, elements=elements, structured_annotations=[])
    new_struct = metadata_struct(enums=enum_dict)

    return new_struct

def gen_metadata_enum_StandardProtocol() -> _fbthrift_metadata.ThriftMetadata:
    return _fbthrift_gen_metadata_enum_StandardProtocol(_fbthrift_metadata.ThriftMetadata(structs={}, enums={}, exceptions={}, services={}))
def _fbthrift_gen_metadata_enum_Void(metadata_struct: _fbthrift_metadata.ThriftMetadata) -> _fbthrift_metadata.ThriftMetadata:
    qualified_name = "standard.Void"

    if qualified_name in metadata_struct.enums:
        return metadata_struct
    elements = {
        0: "NoValue",
    }
    enum_dict = dict(metadata_struct.enums)
    enum_dict[qualified_name] = _fbthrift_metadata.ThriftEnum(name=qualified_name, elements=elements, structured_annotations=[])
    new_struct = metadata_struct(enums=enum_dict)

    return new_struct

def gen_metadata_enum_Void() -> _fbthrift_metadata.ThriftMetadata:
    return _fbthrift_gen_metadata_enum_Void(_fbthrift_metadata.ThriftMetadata(structs={}, enums={}, exceptions={}, services={}))

def getThriftModuleMetadata() -> _fbthrift_metadata.ThriftMetadata:
    meta = _fbthrift_metadata.ThriftMetadata(structs={}, enums={}, exceptions={}, services={})
    meta = _fbthrift_gen_metadata_enum_StandardProtocol(meta)
    meta = _fbthrift_gen_metadata_enum_Void(meta)
    meta = _fbthrift_gen_metadata_struct_UriStruct(meta)
    meta = _fbthrift_gen_metadata_struct_TypeUri(meta)
    meta = _fbthrift_gen_metadata_struct_TypeName(meta)
    return meta
