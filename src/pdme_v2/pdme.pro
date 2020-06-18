TEMPLATES = app
QT += svg 

CONFIG += qt

GRAPHICS_DIR=graphics
DATA_DIR=../common/data
PARSER_DIR=../common/parser
DIALOGS_DIR=./dialogs
UI_DIR=./ui
COMMON_DIR=../common
INCLUDEPATH += ./ $$PARSER_DIR

SOURCES += 	globals.cpp \
			powergui.cpp \
			$$PARSER_DIR/parser.cpp \
			commands.cpp \
            main.cpp 

SOURCES += 	$$GRAPHICS_DIR/gpx_edition_window.cpp \
            $$GRAPHICS_DIR/gpx_edition_scene.cpp \
            $$GRAPHICS_DIR/library_scene.cpp \
            $$GRAPHICS_DIR/gpx_atomic.cpp \
            $$GRAPHICS_DIR/gpx_block.cpp \
            $$GRAPHICS_DIR/gpx_coupled.cpp \
            $$GRAPHICS_DIR/gpx_inport.cpp \
            $$GRAPHICS_DIR/gpx_connection_cross.cpp \
            $$GRAPHICS_DIR/gpx_connection_inport.cpp \
            $$GRAPHICS_DIR/gpx_connection_outport.cpp \
            $$GRAPHICS_DIR/gpx_connection_node.cpp \
            $$GRAPHICS_DIR/gpx_connection_point.cpp \
            $$GRAPHICS_DIR/gpx_temp_connection.cpp \
            $$GRAPHICS_DIR/gpx_edge.cpp \
            $$GRAPHICS_DIR/gpx_outport.cpp \
            $$GRAPHICS_DIR/gpx_textbox.cpp \
            $$GRAPHICS_DIR/library_window.cpp \
            $$GRAPHICS_DIR/gpx_edit.cpp

SOURCES += 	$$DATA_DIR/io.cpp \
           	$$DATA_DIR/coupled.cpp \
           	$$DATA_DIR/graphic.cpp \
           	$$DATA_DIR/line.cpp \
           	$$DATA_DIR/model.cpp \
           	$$DATA_DIR/parameter.cpp \
           	$$DATA_DIR/point.cpp

SOURCES +=  $$DIALOGS_DIR/dlg_change_priority.cpp \
            $$DIALOGS_DIR/dlg_edit_model.cpp \
            $$DIALOGS_DIR/dlg_edit_list_parameter.cpp \
            $$DIALOGS_DIR/dlg_change_library.cpp \
            $$DIALOGS_DIR/dlg_change_parameters.cpp \
            $$DIALOGS_DIR/dlg_set_path_tools.cpp \
            $$DIALOGS_DIR/dlg_menu_setup.cpp \
            $$DIALOGS_DIR/dlg_change_annotation.cpp \
            $$DIALOGS_DIR/dlg_menu_edit.cpp

HEADERS += 	powergui.h \	
  			$$PARSER_DIR/parser.h \
  			user_menu_action.h \
			globals.h \
			authors.h \
			commands.h

HEADERS += 	$$DATA_DIR/model.h \	
            $$DATA_DIR/atomic.h \	
            $$DATA_DIR/graphic.h \	
            $$DATA_DIR/point.h \	
            $$DATA_DIR/port.h \	
            $$DATA_DIR/connection.h \	
            $$DATA_DIR/line.h \	
            $$DATA_DIR/coupled.h 	

HEADERS +=  $$GRAPHICS_DIR/gpx_edition_window.h \
            $$GRAPHICS_DIR/gpx_edition_scene.h \
            $$GRAPHICS_DIR/library_scene.h \
            $$GRAPHICS_DIR/gpx_block.h \
            $$GRAPHICS_DIR/gpx_atomic.h \
            $$GRAPHICS_DIR/gpx_coupled.h \
            $$GRAPHICS_DIR/gpx_inport.h \
            $$GRAPHICS_DIR/gpx_outport.h \
            $$GRAPHICS_DIR/gpx_temp_connection.h \
            $$GRAPHICS_DIR/gpx_connection_cross.h \
            $$GRAPHICS_DIR/gpx_connection_inport.h \
            $$GRAPHICS_DIR/gpx_connection_outport.h \
            $$GRAPHICS_DIR/gpx_connection_point.h \
            $$GRAPHICS_DIR/gpx_connection_node.h \
            $$GRAPHICS_DIR/gpx_edge.h \
            $$GRAPHICS_DIR/gpx_textbox.h \
            $$GRAPHICS_DIR/library_window.h \
            $$GRAPHICS_DIR/gpx_edit.h

HEADERS +=  $$DIALOGS_DIR/dlg_change_priority.h \
            $$DIALOGS_DIR/dlg_edit_model.h \
            $$DIALOGS_DIR/dlg_edit_list_parameter.h \
            $$DIALOGS_DIR/dlg_change_library.h \
            $$DIALOGS_DIR/dlg_change_parameters.h \
            $$DIALOGS_DIR/dlg_set_path_tools.h \
            $$DIALOGS_DIR/dlg_menu_setup.h \
            $$DIALOGS_DIR/dlg_change_annotation.h \
            $$DIALOGS_DIR/dlg_menu_edit.h

FORMS += 	$$UI_DIR/main_window.ui \
         	$$UI_DIR/dlg_edit_model.ui \
         	$$UI_DIR/dlg_change_priority.ui \
         	$$UI_DIR/dlg_change_library.ui \
         	$$UI_DIR/dlg_edit_list_parameter.ui \
		 	$$UI_DIR/dlg_set_path_tools.ui \
		 	$$UI_DIR/dlg_menu_setup.ui \
		 	$$UI_DIR/dlg_change_annotation.ui \
		 	$$UI_DIR/dlg_menu_edit.ui

RESOURCES += pdme.qrc
RC_FILE += pdme.rc

INCLUDEPATH += $$GRAPHICS_DIR $$DATA_DIR $$COMMON_DIR
OBJECTS_DIR = obj
MOC_DIR = moc
unix {
  QMAKE_STRIP = strip -R .comment
  QMAKE_POST_LINK=$(STRIP) $(TARGET)
  TARGET = ../../bin/pdme
}
win32 {
  TARGET = ../../../tmp_win_installer/powerdevs/bin/pdme
}

