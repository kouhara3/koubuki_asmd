###############################################################################
# Tools
###############################################################################

#add_executable(asmd_Coordinate Coordinate.cpp)
#target_link_libraries(asmd_Coordinate kobuki_asmd)

add_executable(asmd_Map Map.cpp )
target_link_libraries(asmd_Map kobuki_asmd ${QT_LIBRARIES} ${ROS_LIBRARIES} ${catkin_LIBRARIES})

#add_executable(asmd_demo_kobuki_simple_run simple_run.cpp)
#target_link_libraries(asmd_demo_kobuki_simple_run kobuki_asmd)

#add_executable(asmd_test_event test_sensor2.cpp)
#target_link_libraries(asmd_test_event kobuki_asmd)

#add_executable(asmd_MapShow MapShow.cpp )
#target_link_libraries(asmd_MapShow kobuki_asmd ${QT_LIBRARIES} ${ROS_LIBRARIES} ${catkin_LIBRARIES})

#add_executable(asmd_MapTopicTest MapTopicTest.cpp)
#target_link_libraries(asmd_MapTopicTest kobuki_asmd ${QT_LIBRARIES} ${ROS_LIBRARIES} ${catkin_LIBRARIES})

#add_executable(asmd_Utilities Utilities.cpp)
#target_link_libraries(asmd_Utilities kobuki_asmd)

#add_executable(asmd_KobukiManager KobukiManager.cpp)
#target_link_libraries(asmd_KobukiManager kobuki_asmd)


install(TARGETS 
        ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
	LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
	RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
