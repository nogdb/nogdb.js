{
    "targets": [
        {
            "target_name": "nogdb_js",
            "cflags_cc": [
                "-std=c++11",
                "-Wall"
            ],
            "sources": [
                "src/init_module.cc",
                "src/classFilter.cc",
                "src/condition.cc",
                "src/context.cc",
                "src/record.cc",
                "src/txn.cc",
                "src/class.cc"
                ,"src/edge.cc"
                ,"src/db.cc"
                ,"src/traverse.cc"
                ,"src/property.cc"
                ,"src/vertex.cc"
                ,"src/toV8type.cc"
            ],
            "include_dirs": [
                "src",
                "/usr/local/incude",
                "<!(node -e \"require('nan')\")"
            ],
            "libraries": [
                "-lnogdb",
		"-latomic"
            ]

        }
    ]
}
