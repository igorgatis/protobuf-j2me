# What is protobuf-j2me?

protobuf-j2me is a *very small* Java implementation of [Google Protocol Buffers](http://code.google.com/apis/protocolbuffers/docs).

Although initially intended for Java 2 Micro Edition (J2ME) runtime environment, both runtime and generated code are fully compatible with *Java 2 Standard Edition* (J2SE and J2EE) as well as *Android 1.5+*.

*Users*: please [let us know](mailto:protobuf-j2me@googlegroups.com) which projects this library is being used on. Also, make sure file bugs.

# Why another Java implementation?

Short answer: needed a very small library compatible with J2ME environment.

Google's official protobuf support for Java generates code which is incompatible with J2ME environment. For example, generated code uses generics which is not compatible with J2ME platform. Besides, although Google's official protobuf compiler for Java already provides a lite version, both runtime and generated code are still fairly big for J2ME standards.

### protobuf 2.4.1 (Google Official) vs protobuf-j2me

|| Feature                || protobuf || protobuf (lite-runtime) || protobuf-j2me ||
|| *Runtime library size* ||   420KB  ||     77KB     ||     22KB      ||
|| *Generated code size*  ||   100%   ||     73%      ||     26%       ||
|| *Enumerations*         ||   Yes    ||     Yes      ||     Yes       ||
|| *Unknown fields*       ||   Yes    ||     Yes      ||      No       ||
|| *Immutability*         ||   Yes    ||     Yes      ||      No       ||
|| *Groups (Deprecated)*  ||   Yes    ||     Yes      ||      No       ||
|| *Reflection*           ||   Yes    ||      No      ||      No       ||
|| *Descriptors*          ||   Yes    ||      No      ||      No       ||

For details regarding these differences see [DesignDecisions].

Finally protobuf-j2me compiler can be used as a [protobuf plugin](http://code.google.com/apis/protocolbuffers/docs/reference/other.html) or as a standalone binary.

# One-minute Introduction

Sample from [Protobuf Documentation Page](http://code.google.com/apis/protocolbuffers/docs):
{{{
message Person {
  required int32 id = 1;
  required string name = 2;
  optional string email = 3;
}
}}}

Compile:
{{{
protoc --proto_path=. sample.proto --j2me_out=.
}}}

Sample code:
<code language="java">
Person person = new Person();
person.setId(1);
person.setName("foo");
person.setEmail("bar@gmail.com");
</code>

# Helping

Feel free to [LinuxBuildInstructions get the code] and send me patches.
