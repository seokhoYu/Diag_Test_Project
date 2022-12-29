SET(PROJECT_NAME diag_server)

set(THREADS_PREFER_PTHREAD_FLAG ON)

PROJECT(${PROJECT_NAME})
CMAKE_MINIMUM_REQUIRED( VERSION 3.16 )

MESSAGE(${PROJECT_NAME})
ADD_EXECUTABLE(diag_server diag_app.c)
find_package(Threads REQUIRED)
target_link_libraries(diag_server PRIVATE Threads::Threads)


#MESSAGE(FATAL_ERROR "Fatal error occured!")
