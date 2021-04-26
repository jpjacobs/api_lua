NB. J interface code for Lua


coclass 'plua'
lib=: './jlua.so'
NB. jl-prefix functions are low level, not to be used directly
jlfuncs =: ".;._2]0 : 0 NB. returns<args (lua_State *==LL for brevity)
'jlinit *'             NB. LL       <''
'lua_close n x'        NB. void     <LL
'lua_setglobal i x *c' NB. int <LL, string
'jldo i x *c'          NB. int code <LL, command string
'jltype *c x *c      ' NB. typename <LL, global var name
)
NB. 'jlclose i x'     NB. int code <LL
NB. register all jl-functions
3 : 0 ''
for_f. jlfuncs do.
  ({.&.;: f)=: (lib,' ',f)&cd
end.
0 0$0
)

NB. TODO:
NB. implement data passing J->Lua (initially just blackbox)
NB. implement data passing Lua->J
NB. do decent error handling
NB. do argument checking


NB. initialiso LuaState and load libs
init=: 3 : '0 0$L=: jlinit 0#0'
NB. close state
close=: 3 : 0
  lua_close L
  erase'L'
)
NB. return Lua Type of global var
type=: 3 : 0
  tpt=.jltype L,<y
  memr 0 _1,~0{::tpt
)
NB. evaluate string in Lua State
NB. TODO: expand to return data if {. of string is '='
ev =: 3 : 0
  jldo L,<y
  0 0$0
)

NB. Lua Table
NB. adverb: m is table name
NB. monad: look up keys
NB. dyad: set keys
NB. for both keys and vals, items are considered
NB. both can be boxed in case of boxed 1D keys, values are unboxed before use
NB. all keys, values EXCEPT for strings and numbers are serialised by 3!:1
NB. symbols might or might not work, or eat your cat.

NB. tab=: 1 : 0
NB. NB. TODO: 
NB. NB. - storage of generic J values -> userdata, but equality test needed
NB. NB. monad: lookup
NB. 
NB. for_k. y do.
NB. 
NB. str=: ''
NB. :
NB. NB. dyad: set key=value
NB. 
NB. )
