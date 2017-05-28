add_executable(pts main.cpp scheduler.cpp task.cpp sqlite.cpp ${SRC})
target_link_libraries(pts ${CMAKE_THREAD_LIBS_INIT} sqlite3 dl)
