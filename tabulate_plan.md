tabulation levels for symbols

valued
TYPE1: global_variable, procedures
level 0: tabulated - name was evaluated and symbol is in symbol table
level 1: resolved - the outside facing info is resolved (signature, type, etc)
level 2: implemented - the non-outside facing info is resolved (initial value, rqir, etc)

object
TYPE2: class, enumeration, category
level 0: tabulated
level 1: implemented
(no resolved stage)

eager
TYPE3: static_variable, local_variable, code
level 0: tabulated
(everything is done at once)