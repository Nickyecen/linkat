-- Test dependency graph

--!include("very_helpful_lua_file.lua")
--!include("test15.lua")

test15(very_helpful_lua_file_function(PREVIOUS_IMPORTANT_CONSTANT))
