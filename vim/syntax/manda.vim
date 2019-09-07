if !exists("g:main_syntax")
  if version < 600
    syntax clear
  elseif exists("b:current_syntax")
    finish
  endif
  let g:main_syntax = 'manda'
  syntax region mandaFold start="{" end="}" transparent fold
endif

syntax case match

" Keywords
syntax keyword mandaConditional if else match with
syntax keyword mandaRepeat do while for in
syntax keyword mandaBool true false
syntax keyword mandaConstant void
syntax keyword mandaKeyword import from
syntax keyword mandaTypedef type class implement sum

" Operators

" Values

" Other...
syntax keyword mandaTodo contained TODO
syntax match mandaLineComment "//.*" contains=mandaTodo,@Spell

" Highlighting rules
highlight default link mandaConditional Conditional
highlight default link mandaRepeat Repeat
highlight default link mandaBool Boolean
highlight default link mandaConstant Constant
highlight default link mandaKeyword Keyword
highlight default link mandaTodo Todo
