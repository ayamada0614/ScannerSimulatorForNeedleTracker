PROJECT(ScannerSimulatorForNeedleTracker)

cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

find_package(OpenIGTLink REQUIRED)

include(${OpenIGTLink_USE_FILE} )

include_directories(
  #${OpenCV_INCLUDE_DIRS}
  #${OpenIGTLink_INCLUDE_DIRS}
)

ADD_EXECUTABLE(ScannerSimulatorForNeedleTracker   ScannerSimulatorForNeedleTracker.cxx)
TARGET_LINK_LIBRARIES(ScannerSimulatorForNeedleTracker OpenIGTLink)

#ADD_EXECUTABLE(ImagerServer   ImagerServer.cxx)
#TARGET_LINK_LIBRARIES(ImagerServer ${OpenCV_LIBS} OpenIGTLink)
#TARGET_LINK_LIBRARIES(ImagerServer ${OpenCV_LIBRARIES} ${OpenCV_LIBS} OpenIGTLink)
#TARGET_LINK_LIBRARIES(ImagerServer ${OpenCV_LIBRARIES} OpenIGTLink )
#TARGET_LINK_LIBRARIES(ImagerServer OpenIGTLink)

