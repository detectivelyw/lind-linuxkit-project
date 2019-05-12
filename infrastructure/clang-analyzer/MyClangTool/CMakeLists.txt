set(LLVM_LINK_COMPONENTS
  ${LLVM_TARGETS_TO_BUILD}
  Option
  Support
)

add_clang_executable(MyClangTool
  MyClangTool.cpp
)

target_link_libraries(MyClangTool
  PRIVATE
  clangTooling
  clangAST
  clangFrontend
  clangASTMatchers
  clangBasic
)

install(TARGETS MyClangTool
  RUNTIME DESTINATION bin
)
