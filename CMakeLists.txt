project(mybc)

set(SOURCE_FILES main.c lexer.c include/lexer.h parser-sem.c vmachine.c include/main.h include/parser.h vmachine.h)
add_executable(mybc ${SOURCE_FILES})
