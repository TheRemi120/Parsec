#Déclaration du projet
project(my_project)

#link_directories(/usr/local/lib)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/)

#find_package(CUDA REQUIRED) 
#set(OpenCV_DIR "/usr/local/lib/OpenCV/")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} ".")

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

list(APPEND HEADER_REP ${ROOT_DIR}/)


set(CPP_FILES ${ROOT_DIR}/main_rasp.cpp ${ROOT_DIR}/mylib.cpp)
#set(CU_FILES ${ROOT_DIR}/main_nano.cu ${ROOT_DIR}/mylib.cu)


#set(CUDA_SEPARABLE_COMPILATION ON)
#set(CUDA_NVCC_FLAGS "-lineinfo;-I\"${ROOT_DIR}/inc\" -I\"${ROOT_DIR}/\" -I\"/usr/local/cuda/samples/common/inc/\"")


add_executable(my_project ${CPP_FILES} ${CU_FILES} ${HEADER_FILES})

set_target_properties(my_project PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(my_project PUBLIC ${ROOT_DIR}/)
target_link_libraries(my_project Qt5::Widgets)
target_link_libraries(my_project Qt5::Core)
target_link_libraries(my_project opencv_core opencv_highgui opencv_imgproc opencv_imgcodecs opencv_video opencv_videoio)
