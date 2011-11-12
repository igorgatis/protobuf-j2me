solutions = [
  {
    "name"        : ".",
    # NOTE: Usage of '.git' extension requires fix in gclient.
    "url"         : "https://code.google.com/p/protobuf-j2me/.git",
    "deps_file"   : "DEPS",
    "custom_deps" : {
      "third_party/gyp":
          "http://gyp.googlecode.com/svn/trunk",
      "third_party/protobuf/trunk":
          "http://protobuf.googlecode.com/svn/tags/2.4.1",
    },
    "safesync_url": "",
  },
]
