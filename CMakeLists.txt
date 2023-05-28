cmake_minimum_required(VERSION 3.5)

project(UVAIGC VERSION 1.0 LANGUAGES CXX)



IF (WIN32)
          MESSAGE("是win系统")
       
ELSEIF (APPLE)
        MESSAGE("是苹果系统")
        
        set (CMAKE_PREFIX_PATH "/Users/linximing/Qt/6.2.4/macos")
        #构建通用二进制指令
        set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
        set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0")
        #设置 macOS SDK 根目录。
#        set(CMAKE_OSX_SYSROOT, "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk")
        #设置代码签名身份
#        set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY, "Your Code Sign Identity")
       

ELSEIF (UNIX)
    MESSAGE("是lin系统")
ENDIF ()





set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#添加第三方库
#PROJECT_SOURCE_DIR项目目录

#第三方
set(DSF_Lib_DIR "${PROJECT_SOURCE_DIR}/Lib")
IF (WIN32)
          MESSAGE("是win系统")
        set(DSF_Lib_DIR "${PROJECT_SOURCE_DIR}/Lib/WINLib")
        set(ONNX_Lib_DIR "${DSF_Lib_DIR}/onnxruntime-win-x64-1.14.1")

ELSEIF (APPLE)
        MESSAGE("是苹果系统")
        set(DSF_Lib_DIR "${PROJECT_SOURCE_DIR}/Lib/MACLib")
        set(ONNX_Lib_DIR "${DSF_Lib_DIR}/onnxruntime-osx-universal2-1.14.1")


ELSEIF (UNIX)
    MESSAGE("是lin系统")
ENDIF ()



#第三方 头文件
# ONNXRUNTIME
 # 绝对路径引入 头文件
 include_directories("${ONNX_Lib_DIR}/include")
 include_directories("${DSF_Lib_DIR}/fftw3/include")

 include_directories("${PROJECT_SOURCE_DIR}/Basis")
include_directories("${PROJECT_SOURCE_DIR}/AudioVocalSeparation")
include_directories("${PROJECT_SOURCE_DIR}/AudioVocalSeparationUI")
include_directories("${PROJECT_SOURCE_DIR}/SettingSpanel")
include_directories("${PROJECT_SOURCE_DIR}/SettingSpanel/Download")
include_directories("${PROJECT_SOURCE_DIR}/SettingSpanel/IAP_and_update/UpdateManager")
include_directories("${PROJECT_SOURCE_DIR}/SettingSpanel/MyIAP_C/aes/plusaes")
include_directories("${PROJECT_SOURCE_DIR}/SettingSpanel/MyIAP_C/httphttp")
include_directories("${PROJECT_SOURCE_DIR}/user/UserModule")
include_directories("${PROJECT_SOURCE_DIR}/user")

#message("${DSF_Lib_DIR}自己德行message to display" )


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

#自己添加系统库
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network Multimedia MultimediaWidgets Svg)

#添加多语言
set(TS_FILES
    translations/UVAIGC_zh_CN.ts
    translations/UVAIGC_es_US.ts
    # 添加其他语言的翻译文件
)
qt_add_lupdate(UVAIGC ${TS_FILES})

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui

        #基础类
        Basis/TaskBasis.cpp
        Basis/TaskBasis.hpp
        Basis/SRTQTime.cpp
        Basis/SRTQTime.h
        AudioVocalSeparation/AudioVocalSeparation.cpp
        AudioVocalSeparation/AudioVocalSeparation.hpp
        
        AudioVocalSeparation/AVS_type.h
        AudioVocalSeparation/AudioFile.h
        #音频分离
        AudioVocalSeparation/AVS_TaskProcessor.hpp
        AudioVocalSeparation/AVS_TaskProcessor.cpp
        AudioVocalSeparation/AVS_TaskProcessingStrategy.hpp
        AudioVocalSeparation/AVS_TaskProcessingStrategy.cpp
        AudioVocalSeparation/AVS_MDXNet_ONNX_Strategy.hpp
        AudioVocalSeparation/AVS_MDXNet_ONNX_Strategy.cpp
        AudioVocalSeparation/AVS_Task.hpp
        AudioVocalSeparation/AVS_Task.cpp
        AudioVocalSeparation/LoaderFile/AVS_LoaderFile.hpp
        AudioVocalSeparation/LoaderFile/AVS_LoaderFile.cpp
        AudioVocalSeparation/LoaderFile/AVS_QTLoaderFile.hpp
        AudioVocalSeparation/LoaderFile/AVS_QTLoaderFile.cpp
        AudioVocalSeparation/SaveFile/AVS_SaveFile.cpp
        AudioVocalSeparation/SaveFile/AVS_SaveFile.hpp
        AudioVocalSeparation/SaveFile/AVS_SaveFileModule.hpp
        AudioVocalSeparation/SaveFile/AVS_SaveFileModule.cpp
        
        
       AudioVocalSeparationUI/TaskCustomItem.cpp
        AudioVocalSeparationUI/TaskCustomItem.h
        AudioVocalSeparationUI/TaskCustomItem.ui
#        AudioVocalSeparationUI/TaskListOBJ.cpp
#        AudioVocalSeparationUI/TaskListOBJ.h
        AudioVocalSeparationUI/TaskListWidgetsClass.cpp
       AudioVocalSeparationUI/TaskListWidgetsClass.h
       AudioVocalSeparationUI/TaskListWidgetsClass.ui
        AudioVocalSeparationUI/TaskQListWidget.cpp
        AudioVocalSeparationUI/TaskQListWidget.h
        AudioVocalSeparationUI/QProgressIndicator.cpp
        AudioVocalSeparationUI/QProgressIndicator.h
        AudioVocalSeparationUI/AVS_Task_QTtool.hpp
        AudioVocalSeparationUI/AVS_Task_QTtool.cpp
        AudioVocalSeparationUI/AVS_TaskProcessor_QT.hpp
        AudioVocalSeparationUI/AVS_TaskProcessor_QT.cpp
        AudioVocalSeparationUI/MySlider.cpp
        AudioVocalSeparationUI/MySlider.h
        AudioVocalSeparationUI/VideoQtWidgets.cpp
        AudioVocalSeparationUI/VideoQtWidgets.h
        AudioVocalSeparationUI/VideoQtWidgets.ui
        
        
        #avs界面
#        AudioVocalSeparationUI/avs_tasklistui.h
#        AudioVocalSeparationUI/avs_tasklistui.cpp
        
       #设置
        SettingSpanel/Settings_info.cpp
        SettingSpanel/Settings_info.hpp
        SettingSpanel/SettingSpanel_main.cpp
        SettingSpanel/SettingSpanel_main.h
        SettingSpanel/SettingSpanelQtWidgetsClass.cpp
        SettingSpanel/SettingSpanelQtWidgetsClass.h
        SettingSpanel/SettingSpanelQtWidgetsClass.ui
         SettingSpanel/avs_settingqtwidgetsclass.ui
         SettingSpanel/avs_settingqtwidgetsclass.h
         SettingSpanel/avs_settingqtwidgetsclass.cpp
         Resource.qrc
       SettingSpanel/Download/DownloadCustomItem.ui
       SettingSpanel/Download/DownloadCustomItem.h
       SettingSpanel/Download/DownloadCustomItem.cpp
       SettingSpanel/Download/DownloadListOBJ.cpp
       SettingSpanel/Download/DownloadListOBJ.h
       SettingSpanel/Download/DownloadListWidgetsClass.h
       SettingSpanel/Download/DownloadListWidgetsClass.cpp
       SettingSpanel/Download/DownloadTool.cpp
       SettingSpanel/Download/DownloadTool.h
       SettingSpanel/IAP_and_update/UpdateManager/UpdateManager.cpp
       SettingSpanel/IAP_and_update/UpdateManager/UpdateManager.hpp
       #用户
       user/UserModule/UserModule.cpp
       user/UserModule/UserModule.hpp
       user/UserMainWindow.h
       user/UserMainWindow.cpp
       user/UserMainWindow.ui
       user/loginwindow.cpp
       user/loginwindow.h
       user/loginwindow.ui
       user/userinfowindow.h
       user/userinfowindow.cpp
       user/userinfowindow.ui
      user/registerwndow.h
      user/registerwndow.cpp
      user/registerwndow.ui
      user/usermemberwindow.cpp
      user/usermemberwindow.h
      user/usermemberwindow.ui
      user/UserDeviceCustomDialog.cpp
      user/UserDeviceCustomDialog.hpp

      UVAIGC.rc
         
         
        ${TS_FILES}
)
qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(UVAIGC
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET UVAIGC APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation


else()
    if(ANDROID)
        add_library(UVAIGC SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(UVAIGC
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(UVAIGC PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

#自己添加库
target_link_libraries(UVAIGC PRIVATE Qt6::Network Qt6::Multimedia Qt6::MultimediaWidgets Qt6::Svg)

#添加第三方库
IF (WIN32)
     MESSAGE("是win系统")
     target_link_libraries(UVAIGC PRIVATE "${ONNX_Lib_DIR}/lib/onnxruntime.lib")
     target_link_libraries(UVAIGC PRIVATE "${DSF_Lib_DIR}/fftw3/lib/libfftw3-3.lib")


ELSEIF (APPLE)
        MESSAGE("是苹果系统")
        target_link_libraries(UVAIGC PRIVATE "${ONNX_Lib_DIR}/lib/libonnxruntime.dylib")
        target_link_libraries(UVAIGC PRIVATE "${DSF_Lib_DIR}/fftw3/lib/libfftw3.a")

ELSEIF (UNIX)
    MESSAGE("是lin系统")
ENDIF ()


#翻译
add_custom_target(translations_target DEPENDS ${QM_FILES})


set_target_properties(UVAIGC PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

#语言翻译 生成的文件
set_source_files_properties(${TS_FILES}
    PROPERTIES OUTPUT_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/translations")
qt_add_lrelease(UVAIGC TS_FILES ${TS_FILES} QM_FILES_OUTPUT_VARIABLE qm_files)
install(FILES ${qm_files} DESTINATION "translations")

install(TARGETS UVAIGC
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(UVAIGC)
endif()
