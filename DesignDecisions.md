This page includes design decisions made while implementing protobuf-j2me runtime and compiler towards the goal of small code size footprint.

## Protobuf Feature Coverage ##

  * **Enums**: Protobuf **enums are supported** in two ways: generated Enum classes or via numeric constants (c-style) which produces smaller code.

  * **Unknown fields**: **Not supported**. Unknown fields are discarded by protobuf-j2me classes. Since protobuf-j2me is meant to be used by end-point devices such as mobile phones, the use cases were supported for unknown fields is required are very rare. Thus for the sake making runtime library even smaller, support for feature was intentionally dropped.

  * **Groups**: **Not supported**. Since support for protobuf groups is deprecated, this feature is intentionally not present in protobuf-j2me.

  * **Reflection**: **Not supported**. Reflection support requires a significant amount of extra code. Support for this feature was intentionally left out.

  * **Descriptors**: **Not supported**. Descriptors require a significant amount of literals in source code. Support for this feature was intentionally left out.


## Immutability ##

Google's official Java implementation generates immutable messages. These messages can be built or modified through _Builder Pattern_. protobuf-j2me does not support immutable messages.

## Generated source code compatibility ##

protobuf-j2me generated code is compatible with Java 2 v1.3 specification (required by J2ME), thus it does not support generics.