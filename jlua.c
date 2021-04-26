// compile with gcc -fPIC -Wall -I/usr/include/lua5.3 -shared -o jlua.so jlua.c -llua5.3
// on android:  gcc -shared -o jlua.so  -I$PREFIX/include/lua5.4     -llua5.4 -L$PREFIX/lib/ jlua.c 
// this is a try to make a J ffi ibrary to get Lua into J.
// initially, to get tables of J objects.
// The opposite, J as library to Lua is not possible on Android due to lack of j.so
//
#include <stdio.h>
#include <string.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

typedef lua_State LL ;
//*

// representation of J values in Lua
typedef struct JValue {
  size_t conv;
  size_t type;
  size_t numel;
  size_t rank;
  char shapval[1]; // variable part 
} JValue; // check syntax

#define checkJValue(L) (JValue *)luaL_checkudata(L,1,"jsoftware.JValue")

int jljval2string (LL *L){ // expects jvalue on stack
  JValue *v = checkJValue(L);
  lua_pushfstring(L,"Jvalue, %d bytes",lua_rawlen(L,-1)); // prints byte size TODO extend to print type, rank, shape
  return 1;
}


// module functions go here
static const struct luaL_Reg jlib_f []={
  {NULL,NULL}
};

// meta methods of Jvalue go here
static const struct luaL_Reg jlib_m []={
  {"__tostring",jljval2string},
  {NULL,NULL}
};

int luaopen_jlib(LL *L){
  luaL_newmetatable(L,"jsoftware.JValue");
  // metatable.__index = metatable
  lua_pushvalue(L,-1);           //duplicate metatable
  lua_setfield(L,-2,"__index");  // 
  luaL_setfuncs(L,jlib_m,0);     // set metamethod in mtable, no upvalues

  luaL_newlib(L, jlib_f);
  // register J types in a table, both as number and reverse
  // J data type names
  // TODO optimise out setting and getting the global "tmp"
  const char * JTYPESBLOCK = "tmp={boolean=1, literal=2, integer=4, float=8, complex=16, boxed=32, extended=64, rational=128, sparse_boolean=1024, sparse_literal=2048, sparse_integer=4096, sparse_float=8192, sparse_complex=16384, sparse_boxed=13107, symbol=65536, literal2=131072, literal4=262144}; for k,v in pairs(tmp) do tmp[v]=k end";
  int err = luaL_dostring(L, JTYPESBLOCK);
  if(err){
    fprintf(stderr, "%s\n", lua_tostring(L,-1));
  };
  lua_getglobal(L,"tmp");
  
  // TODO: should make read-only table
  lua_setfield(L,-2,"types");
  lua_pushnil(L);		//remove tmp from globals
  lua_setglobal(L,"tmp");
  return 1;
};

// */  

// './jlua.so jlinit *' cd ''
lua_State * jlinit (void){ // initializo Lua, return pointer to state
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  luaL_requiref(L,"jlib",*luaopen_jlib,1); // Require jlib and store in global
  lua_pop(L,1); 			   //discard unneeded jlib on the stack.
//  luaopen_jlib(L);
  return L;
}
/* './jlua.so lua_close i x' cd state
int jlclose (LL *L){
  lua_close(L);
  return 0;
}
// */

// './jlua.so jldo i x *c' cd state;'a=123'
// or fun one: for k,v in pairs(_G) do print(k,v) end
int jldo (lua_State *L, char* cmd) {
  int error;
  error=luaL_loadstring(L,cmd) || lua_pcall(L,0,0,0);
  if(error){
    fprintf(stderr, "%s\n", lua_tostring(L,-1));
    lua_pop(L,1);
  }
  return 0;
}

// foo=:'./jlua.so jltype *c x *c' cd s,<,'a'
const char* jltype (LL *L, char* var) {
  lua_getglobal(L,var);
  return lua_typename(L,lua_type(L,-1));
}

// data from J to Lua
// for tomorrow.
// non-exposed function to create userdata from J
// test in J 
// foo=: 3!:1]i.2 3 4
// p=: mema 124 NB. 124-:#foo)
// foo memw p,0 124
// (lib, ' jlpushJValue i x x i')cd L, p;124
// (lib, ' lua_setglobal i x *c') cd L,<'test'
//  ev'print(test)'
//  can also be used as i x *c i passing foo directly.
int jlpushJValue (LL *L, char* str, int len){
  void* mem;
  mem=lua_newuserdata(L,len);
  memcpy(mem,str,len);
  luaL_getmetatable(L,"jsoftware.JValue");
  lua_setmetatable(L,-2);  // set metatable
  return 1; // 1 new userdatum on the stack
}

/* very basicinterpreter chunk
  char buff[256];
  int error;
  while (fgets(buff,sizeof(buff),stdin) != NULL) {
    error=luaL_loadstring(L,buff)||lua_pcall(L,0,0,0);
    if(error){
      fprintf(stderr, "%s\n", lua_tostring(L,-1));
      lua_pop(L,1);
    }
  }
  */

// vim: ts=2:sw=2:et
