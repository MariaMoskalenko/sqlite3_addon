{
  "targets": [
    {
      "target_name": "sqlite3_addon",
      "sources": [ "addon_main.cc", "KeyValueStorageWrapper.cpp", "CommandQueue.cpp",
                    "CommandOpenDataBase.cpp", "CommandCloseDataBase.cpp", "CommandExecuteQuery.cpp",
                    "sqlite3.c", "CSqlite3Wrapper.cpp" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
