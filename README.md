** _WORK IN PROGRESS_**: not to be used for anything serious.

Attempt to interface J with Lua and viceversa.

Currently working on using Lua as a library to J.

Functional:
- [x]: creation of Lua states (init_plua_'')
- [x]: closing of Lua states  (close_plua_'')
- [x]: executing Lua chunks (i.e. strings) in a Lua state from J (`ev_plua_'print"hello World!"'`)
- [x]: loads jlib into Lua by C-code
- [ ]: J binary data userdata in Lua (to be tested)
- [ ]: transfer and storing J data in Lua
- [ ]: transfer Lua and J data from Lua to J
- [ ]: conversion between J and Lua datatypes
- [ ]: executing J phrases from Lua
- [ ]: defining J words in Lua
- [ ]: Using J words in Lua on J Userdata
- [ ]: Same on Lua types  
