{
    "targets": [
        {
            "target_name": "nogdb_js",
            "cflags_cc": [
                "-std=c++11",
                "-Wall",
                "-fexceptions"
            ],
            'conditions': [
                ['OS=="mac"', {
                  'xcode_settings': {
                    'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
                  }
                }]
            ],
            "sources": [
                "src/init_module.cc",
                "src/condition.cc",
                "src/context.cc",
                "src/record.cc",
                "src/txn.cc",
                "src/toV8type.cc"
            ],
            "include_dirs": [
                "src",
                "/usr/local/incude",
                "<!(node -e \"require('nan')\")"
            ],
            "libraries": [
                "-lnogdb"
            ]

        }
    ]
}
