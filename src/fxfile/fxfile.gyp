#
# Copyright (c) 2013 Leon Lee author. All rights reserved.
#
#   homepage: http://www.flychk.com
#   e-mail:   mailto:flychk@flychk.com
#
# Use of this source code is governed by a GPLv3 license that can be
# found in the LICENSE file.

{
    'variables':
    {
    },
    
    'includes':
    [
        '../../build/common.gypi',
    ],
    
    'target_defaults':
    {
        'default_configuration' : 'Debug-x86-Unicode',
        
        'dependencies':
        [
            '../xpr/xpr.gyp:*',
        ],

        'configurations':
        {
            'Debug-x86-Unicode':
            {
                'inherit_from': ['Debug-x86-MFC-Unicode_Base'],
                
                'msvs_configuration_attributes':
                {
                    'OutputDirectory':       '../../bin/x86-unicode',
                    'IntermediateDirectory': '../../obj/xpr/dbg-x86-unicode',
                },
                
                'msvs_settings':
                {
                    'VCLinkerTool': 
                    {
                        'OutputFile': '$(OutDir)\$(ProjectName)_dbg.exe',
                        'AdditionalLibraryDirectories':
                        [
                            '../../lib/gfl/libW',
                            '../../lib/vld/lib/Win32',
                        ],
                        'AdditionalDependencies':
                        [
                            'libxpr_dbg.lib',
                        ],
                    },
                },
            },
            
            'Release-x86-Unicode':
            {
                'inherit_from': ['Release-x86-MFC-Unicode_Base'],
                
                'msvs_configuration_attributes':
                {
                    'OutputDirectory':       '../../bin/x86-unicode',
                    'IntermediateDirectory': '../../obj/xpr/rel-x86-unicode',
                },
                
                'msvs_settings':
                {
                    'VCLinkerTool': 
                    {
                        'OutputFile': '$(OutDir)\$(ProjectName).exe',
                        'AdditionalLibraryDirectories':
                        [
                            '../../lib/gfl/libW',
                        ],
                        'AdditionalDependencies':
                        [
                            'libxpr.lib',
                        ],
                    },
                },
            },
            
            'Debug-x86-Multibyte':
            {
                'inherit_from': ['Debug-x86-MFC-Multibyte_Base'],
                
                'msvs_configuration_attributes':
                {
                    'OutputDirectory':       '../../bin/x86-multibyte',
                    'IntermediateDirectory': '../../obj/xpr/dbg-x86-multibyte',
                },
                
                'msvs_settings':
                {
                    'VCLinkerTool': 
                    {
                        'OutputFile': '$(OutDir)\$(ProjectName)_dbg.exe',
                        'AdditionalLibraryDirectories':
                        [
                            '../../lib/gfl/lib',
                            '../../lib/vld/lib/Win32',
                        ],
                        'AdditionalDependencies':
                        [
                            'libxpr_dbg.lib',
                        ],
                    },
                },
            },
            
            'Release-x86-Multibyte':
            {
                'inherit_from': ['Release-x86-MFC-Multibyte_Base'],
                
                'msvs_configuration_attributes':
                {
                    'OutputDirectory':       '../../bin/x86-multibyte',
                    'IntermediateDirectory': '../../obj/xpr/rel-x86-multibyte',
                },
                
                'msvs_settings':
                {
                    'VCLinkerTool': 
                    {
                        'OutputFile': '$(OutDir)\$(ProjectName).exe',
                        'AdditionalLibraryDirectories':
                        [
                            '../../lib/gfl/lib',
                        ],
                        'AdditionalDependencies':
                        [
                            'libxpr.lib',
                        ],
                    },
                },
            },

            'conditions':
            [
                [ 'target_arch!="x86"',
                    {
                        'Debug-x64-Unicode':
                        {
                            'inherit_from': ['Debug-x64-MFC-Unicode_Base'],
                            
                            'msvs_configuration_attributes':
                            {
                                'OutputDirectory':       '../../bin/x64',
                                'IntermediateDirectory': '../../obj/xpr/dbg-x64',
                            },
                            
                            'msvs_settings':
                            {
                                'VCLinkerTool': 
                                {
                                    'OutputFile': '$(OutDir)\$(ProjectName)_dbg.exe',
                                    'AdditionalLibraryDirectories':
                                    [
                                        '../../lib/gfl/lib64W',
                                        '../../lib/vld/lib/Win64',
                                    ],
                                    'AdditionalDependencies':
                                    [
                                        'libxpr_dbg.lib',
                                    ],
                                },
                            },
                        },

                        'Release-x64-Unicode':
                        {
                            'inherit_from': ['Release-x64-MFC-Unicode_Base'],
                            
                            'msvs_configuration_attributes':
                            {
                                'OutputDirectory':       '../../bin/x64',
                                'IntermediateDirectory': '../../obj/xpr/rel-x64',
                            },
                            
                            'msvs_settings':
                            {
                                'VCLinkerTool': 
                                {
                                    'OutputFile': '$(OutDir)\$(ProjectName).exe',
                                    'AdditionalLibraryDirectories':
                                    [
                                        '../../lib/gfl/lib64W',
                                    ],
                                    'AdditionalDependencies':
                                    [
                                        'libxpr.lib',
                                    ],
                                },
                            },
                        },
                    },
                ],
            ],
        },
    },

    'targets':
    [
        {
            'target_name': 'fxfile',
            
            'type': 'executable',
            
            'defines':
            [
            ],
            
            'include_dirs':
            [
                './',
                '../../src/xpr/include',
                '../../src/base',
                '../../src/fxfile-crash',
                '../../lib/gfl/include',
                '../../lib/vld/include',
            ],
            
            'libraries':
            [
                'shlwapi.lib',
                'msimg32.lib',
                'Mpr.lib',
                'htmlhelp.lib',
                'Version.lib',
                'Imm32.lib',
                'setupapi.lib',
                'libgfl.lib',
            ],
            
            'msvs_settings':
            {
                'VCLinkerTool': 
                {
                    'AdditionalLibraryDirectories':
                    [
                        '$(OutDir)',
                    ]
                },
            },
            
            'msvs_precompiled_header': 'stdafx.h',
            'msvs_precompiled_source': 'stdafx.cpp',
            
            'sources':
            [
                '../base/conf_file.cpp',
                '../base/conf_file.h',
                '../base/conf_file_ex.h',
                '../base/crc32.c',
                '../base/crc32.h',
                '../base/def.h',
                '../base/def_win.h',
                '../base/fnmatch.cpp',
                '../base/fnmatch.h',
                '../base/format_string_table.cpp',
                '../base/format_string_table.h',
                '../base/language_pack.cpp',
                '../base/language_pack.h',
                '../base/language_table.cpp',
                '../base/language_table.h',
                '../base/md5.c',
                '../base/md5.h',
                '../base/pattern.h',
                '../base/pidl_win.cpp',
                '../base/pidl_win.h',
                '../base/sha.c',
                '../base/sha.h',
                '../base/shell_enumerator.h',
                '../base/shell_enumerator_win.cpp',
                '../base/shell_item.h',
                '../base/shell_item_win.cpp',
                '../base/string_table.cpp',
                '../base/string_table.h',
                '../base/update_info_manager.cpp',
                '../base/update_info_manager.h',
                '../base/updater_def.h',
                '../base/xml.cpp',
                '../base/xml.h',

                './gui/BCMenu.cpp',
                './gui/BCMenu.h',
                './gui/BtnST.cpp',
                './gui/BtnST.h',
                './gui/ColourPickerXP.cpp',
                './gui/ColourPickerXP.h',
                './gui/DlgToolBar.cpp',
                './gui/DlgToolBar.h',
                './gui/DragDropToolBar.cpp',
                './gui/DragDropToolBar.h',
                './gui/DragImage.cpp',
                './gui/DragImage.h',
                './gui/DrivePieCtrl.cpp',
                './gui/DrivePieCtrl.h',
                './gui/DropMenuButton.cpp',
                './gui/DropMenuButton.h',
                './gui/DropSource.cpp',
                './gui/DropSource.h',
                './gui/DropTarget.cpp',
                './gui/DropTarget.h',
                './gui/EditScroll.cpp',
                './gui/EditScroll.h',
                './gui/FileDialogST.cpp',
                './gui/FileDialogST.h',
                './gui/FlatHeaderCtrl.cpp',
                './gui/FlatHeaderCtrl.h',
                './gui/gdi.cpp',
                './gui/gdi.h',
                './gui/IconList.cpp',
                './gui/IconList.h',
                './gui/ListCtrlEx.cpp',
                './gui/ListCtrlEx.h',
                './gui/ListCtrlPrint.cpp',
                './gui/ListCtrlPrint.h',
                './gui/MemDC.h',
                './gui/Print.cpp',
                './gui/Print.h',
                './gui/ResizingDialog.cpp',
                './gui/ResizingDialog.h',
                './gui/SizeGripWnd.cpp',
                './gui/SizeGripWnd.h',
                './gui/StatusBar.cpp',
                './gui/StatusBar.h',
                './gui/StatusBarObserver.h',
                './gui/SysTray.cpp',
                './gui/SysTray.h',
                './gui/TabCtrl.cpp',
                './gui/TabCtrl.h',
                './gui/TabCtrlObserver.h',
                './gui/TextProgressCtrl.cpp',
                './gui/TextProgressCtrl.h',
                './gui/TreeCtrlEx.cpp',
                './gui/TreeCtrlEx.h',
                './gui/WindowScroller.cpp',
                './gui/WindowScroller.h',
                './gui/XPTheme.cpp',
                './gui/XPTheme.h',

                './gui/rebar/CustomizeDialog.cpp',
                './gui/rebar/CustomizeDialog.h',
                './gui/rebar/DllLoader.cpp',
                './gui/rebar/DllLoader.h',
                './gui/rebar/FrameWndEx.cpp',
                './gui/rebar/FrameWndEx.h',
                './gui/rebar/GlobalData.h',
                './gui/rebar/MenuBar.cpp',
                './gui/rebar/MenuBar.h',
                './gui/rebar/SizableReBar.cpp',
                './gui/rebar/SizableReBar.h',
                './gui/rebar/ToolBarEx.cpp',
                './gui/rebar/ToolBarEx.h',
                './gui/rebar/WinAppEx.cpp',
                './gui/rebar/WinAppEx.h',

                './accel_table.cpp',
                './accel_table.h',
                './activate_bar.cpp',
                './activate_bar.h',
                './address_bar.cpp',
                './address_bar.h',
                './address_bar_observer.h',
                './adv_file_change_watcher.cpp',
                './adv_file_change_watcher.h',
                './app_ver.cpp',
                './app_ver.h',
                './bb_info_file.cpp',
                './bb_info_file.h',
                './bitmap.cpp',
                './bitmap.h',
                './bookmark.cpp',
                './bookmark.h',
                './bookmark_item_edit_dlg.cpp',
                './bookmark_item_edit_dlg.h',
                './bookmark_set_wnd.cpp',
                './bookmark_set_wnd.h',
                './bookmark_toolbar.cpp',
                './bookmark_toolbar.h',
                './bookmark_toolbar_observer.h',
                './bookmark_wnd.cpp',
                './bookmark_wnd.h',
                './clip_format.cpp',
                './clip_format.h',
                './clipboard.cpp',
                './clipboard.h',
                './cmd_line_parser.cpp',
                './cmd_line_parser.h',
                './command_string_table.cpp',
                './command_string_table.h',
                './conf_dir.cpp',
                './conf_dir.h',
                './contents_wnd.cpp',
                './contents_wnd.h',
                './context_menu.cpp',
                './context_menu.h',
                './crc_checksum.cpp',
                './crc_checksum.h',
                './ctrl_id.h',
                './dlg_state.cpp',
                './dlg_state.h',
                './dlg_state_manager.cpp',
                './dlg_state_manager.h',
                './drive_path_bar.cpp',
                './drive_path_bar.h',
                './drive_shcn.cpp',
                './drive_shcn.h',
                './drive_toolbar.cpp',
                './drive_toolbar.h',
                './drive_toolbar_observer.h',
                './env_path.cpp',
                './env_path.h',
                './explorer_ctrl.cpp',
                './explorer_ctrl.h',
                './explorer_ctrl_observer.h',
                './explorer_ctrl_print.cpp',
                './explorer_ctrl_print.h',
                './explorer_pane.cpp',
                './explorer_pane.h',
                './explorer_status_bar.cpp',
                './explorer_status_bar.h',
                './explorer_view.cpp',
                './explorer_view.h',
                './explorer_view_observer.h',
                './file_change_watcher.cpp',
                './file_change_watcher.h',
                './file_op_thread.cpp',
                './file_op_thread.h',
                './file_op_undo.cpp',
                './file_op_undo.h',
                './file_scrap.cpp',
                './file_scrap.h',
                './file_scrap_ctrl.cpp',
                './file_scrap_ctrl.h',
                './file_scrap_ctrl_observer.h',
                './file_scrap_drop_dlg.cpp',
                './file_scrap_drop_dlg.h',
                './file_scrap_observer.h',
                './file_scrap_pane.cpp',
                './file_scrap_pane.h',
                './file_scrap_toolbar.cpp',
                './file_scrap_toolbar.h',
                './filter.cpp',
                './filter.h',
                './folder_ctrl.cpp',
                './folder_ctrl.h',
                './folder_ctrl_observer.h',
                './folder_pane.cpp',
                './folder_pane.h',
                './folder_pane_observer.h',
                './folder_size.cpp',
                './folder_size.h',
                './folder_view.cpp',
                './folder_view.h',
                './folder_view_observer.h',
                './functors.h',
                './fxfile.rc',
                './fxfile_def.h',
                './history.cpp',
                './history.h',
                './item_data.h',
                './keyboard.cpp',
                './keyboard.h',
                './launcher_manager.cpp',
                './launcher_manager.h',
                './main_coolbar.cpp',
                './main_coolbar.h',
                './main_frame.cpp',
                './main_frame.h',
                './main_menubar.cpp',
                './main_menubar.h',
                './main_toolbar.cpp',
                './main_toolbar.h',
                './option.cpp',
                './option.h',
                './option_manager.cpp',
                './option_manager.h',
                './option_observer.h',
                './path.cpp',
                './path.h',
                './path_bar.cpp',
                './path_bar.h',
                './path_bar_observer.h',
                './picture_viewer.cpp',
                './picture_viewer.h',
                './print_preview_view_ex.cpp',
                './print_preview_view_ex.h',
                './program_ass.cpp',
                './program_ass.h',
                './recent_file_list.cpp',
                './recent_file_list.h',
                './removable_device.cpp',
                './removable_device.h',
                './removable_drive.cpp',
                './removable_drive.h',
                './rename_helper.cpp',
                './rename_helper.h',
                './resource.h',
                './search.h',
                './search_dlg.cpp',
                './search_dlg.h',
                './search_dlg_observer.h',
                './search_file.cpp',
                './search_file.h',
                './search_result_ctrl.cpp',
                './search_result_ctrl.h',
                './search_result_ctrl_observer.h',
                './search_result_pane.cpp',
                './search_result_pane.h',
                './shell.cpp',
                './shell.h',
                './shell_change_notify.cpp',
                './shell_change_notify.h',
                './shell_column.cpp',
                './shell_column.h',
                './shell_icon.cpp',
                './shell_icon.h',
                './shell_new.cpp',
                './shell_new.h',
                './shell_registry.cpp',
                './shell_registry.h',
                './singleton_manager.h',
                './singleton_manager.cpp',
                './single_process.h',
                './single_process.cpp',
                './size_format.cpp',
                './size_format.h',
                './splitter.cpp',
                './splitter.h',
                './stdafx.cpp',
                './stdafx.h',
                './sync_dirs.cpp',
                './sync_dirs.h',
                './sys_img_list.cpp',
                './sys_img_list.h',
                './SystemInfo.cpp',
                './SystemInfo.h',
                './tab_pane.cpp',
                './tab_pane.h',
                './tab_pane_observer.h',
                './tab_type.h',
                './targetver.h',
                './thread.cpp',
                './thread.h',
                './thumb_img_list.cpp',
                './thumb_img_list.h',
                './thumbnail.cpp',
                './thumbnail.h',
                './updater_manager.cpp',
                './updater_manager.h',
                './user_env.cpp',
                './user_env.h',
                './view_set.cpp',
                './view_set.h',
                './vwin32.h',
                './wait_dlg.cpp',
                './wait_dlg.h',
                './win_app.cpp',
                './win_app.h',
                './winapi_ex.cpp',
                './winapi_ex.h',
                './wnet_mgr.cpp',
                './wnet_mgr.h',

                './res/about.bmp',
                './res/config_tree.bmp',
                './res/dos_cmd.ico',
                './res/expand.ico',
                './res/file_scrap.ico',
                './res/file_scrap_drop.bmp',
                './res/fxfile.ico',
                './res/fxfile.rc2',
                './res/goup.ico',
                './res/link.ico',
                './res/not_custom_icon.ico',
                './res/pending.ico',
                './res/preview_next_cold.bmp',
                './res/preview_next_hot.bmp',
                './res/preview_prev_cold.bmp',
                './res/preview_prev_hot.bmp',
                './res/preview_print_cold.bmp',
                './res/preview_print_hot.bmp',
                './res/preview_zoomin_cold.bmp',
                './res/preview_zoomin_hot.bmp',
                './res/preview_zoomout_cold.bmp',
                './res/preview_zoomout_hot.bmp',
                './res/schdec.ico',
                './res/schinc.ico',
                './res/search.bmp',
                './res/search.ico',
                './res/shortcut.ico',
                './res/sync_images.bmp',
                './res/tb_drive.bmp',
                './res/tb_main_cold_large.bmp',
                './res/tb_main_cold_small.bmp',
                './res/tb_main_hot_large.bmp',
                './res/tb_main_hot_small.bmp',
                './res/tb_rename.bmp',
                './res/tip.ico',
                './res/tray.ico',
                './res/tray_xp.ico',
                './res/fxfile.exe.manifest',

                './cfg/cfg_adv_conf_dir_dlg.cpp',
                './cfg/cfg_adv_conf_dir_dlg.h',
                './cfg/cfg_adv_dlg.cpp',
                './cfg/cfg_adv_dlg.h',
                './cfg/cfg_adv_hotkey_dlg.cpp',
                './cfg/cfg_adv_hotkey_dlg.h',
                './cfg/cfg_appearance_color_dlg.cpp',
                './cfg/cfg_appearance_color_dlg.h',
                './cfg/cfg_appearance_color_view_dlg.cpp',
                './cfg/cfg_appearance_color_view_dlg.h',
                './cfg/cfg_appearance_dlg.cpp',
                './cfg/cfg_appearance_dlg.h',
                './cfg/cfg_appearance_file_list_dlg.cpp',
                './cfg/cfg_appearance_file_list_dlg.h',
                './cfg/cfg_appearance_filtering_dlg.cpp',
                './cfg/cfg_appearance_filtering_dlg.h',
                './cfg/cfg_appearance_folder_tree_dlg.cpp',
                './cfg/cfg_appearance_folder_tree_dlg.h',
                './cfg/cfg_appearance_font_dlg.cpp',
                './cfg/cfg_appearance_font_dlg.h',
                './cfg/cfg_appearance_history_dlg.cpp',
                './cfg/cfg_appearance_history_dlg.h',
                './cfg/cfg_appearance_language_dlg.cpp',
                './cfg/cfg_appearance_language_dlg.h',
                './cfg/cfg_appearance_layout_dlg.cpp',
                './cfg/cfg_appearance_layout_dlg.h',
                './cfg/cfg_appearance_size_format_dlg.cpp',
                './cfg/cfg_appearance_size_format_dlg.h',
                './cfg/cfg_appearance_thumbnail_dlg.cpp',
                './cfg/cfg_appearance_thumbnail_dlg.h',
                './cfg/cfg_appearance_view_set_dlg.cpp',
                './cfg/cfg_appearance_view_set_dlg.h',
                './cfg/cfg_dlg.cpp',
                './cfg/cfg_dlg.h',
                './cfg/cfg_dlg_observer.h',
                './cfg/cfg_func_bookmark_dlg.cpp',
                './cfg/cfg_func_bookmark_dlg.h',
                './cfg/cfg_func_dlg.cpp',
                './cfg/cfg_func_dlg.h',
                './cfg/cfg_func_drag_dlg.cpp',
                './cfg/cfg_func_drag_dlg.h',
                './cfg/cfg_func_file_op_dlg.cpp',
                './cfg/cfg_func_file_op_dlg.h',
                './cfg/cfg_func_file_scrap_dlg.cpp',
                './cfg/cfg_func_file_scrap_dlg.h',
                './cfg/cfg_func_program_ass_dlg.cpp',
                './cfg/cfg_func_program_ass_dlg.h',
                './cfg/cfg_func_refresh_dlg.cpp',
                './cfg/cfg_func_refresh_dlg.h',
                './cfg/cfg_func_rename_dlg.cpp',
                './cfg/cfg_func_rename_dlg.h',
                './cfg/cfg_general_dlg.cpp',
                './cfg/cfg_general_dlg.h',
                './cfg/cfg_general_startup_dlg.cpp',
                './cfg/cfg_general_startup_dlg.h',
                './cfg/cfg_general_startup_view_dlg.cpp',
                './cfg/cfg_general_startup_view_dlg.h',
                './cfg/cfg_main_dlg.cpp',
                './cfg/cfg_main_dlg.h',
                './cfg/filter_set_dlg.cpp',
                './cfg/filter_set_dlg.h',
                './cfg/program_ass_set_dlg.cpp',
                './cfg/program_ass_set_dlg.h',
                './cfg/size_format_dlg.cpp',
                './cfg/size_format_dlg.h',

                './cmd/about_dlg.cpp',
                './cmd/about_dlg.h',
                './cmd/about_tab_dlg.cpp',
                './cmd/about_tab_dlg.h',
                './cmd/about_tab_info_dlg.cpp',
                './cmd/about_tab_info_dlg.h',
                './cmd/about_tab_license_dlg.cpp',
                './cmd/about_tab_license_dlg.h',
                './cmd/about_tab_credits_dlg.cpp',
                './cmd/about_tab_credits_dlg.h',
                './cmd/accel_table_dlg.cpp',
                './cmd/accel_table_dlg.h',
                './cmd/attr_time.cpp',
                './cmd/attr_time.h',
                './cmd/attr_time_dlg.cpp',
                './cmd/attr_time_dlg.h',
                './cmd/batch_create.cpp',
                './cmd/batch_create.h',
                './cmd/batch_create_dlg.cpp',
                './cmd/batch_create_dlg.h',
                './cmd/batch_create_tab_dlg.cpp',
                './cmd/batch_create_tab_dlg.h',
                './cmd/batch_create_tab_format_dlg.cpp',
                './cmd/batch_create_tab_format_dlg.h',
                './cmd/batch_create_tab_text_dlg.cpp',
                './cmd/batch_create_tab_text_dlg.h',
                './cmd/batch_rename.cpp',
                './cmd/batch_rename.h',
                './cmd/batch_rename_dlg.cpp',
                './cmd/batch_rename_dlg.h',
                './cmd/batch_rename_edit_dlg.cpp',
                './cmd/batch_rename_edit_dlg.h',
                './cmd/batch_rename_tab_case_dlg.cpp',
                './cmd/batch_rename_tab_case_dlg.h',
                './cmd/batch_rename_tab_delete_dlg.cpp',
                './cmd/batch_rename_tab_delete_dlg.h',
                './cmd/batch_rename_tab_dlg.cpp',
                './cmd/batch_rename_tab_dlg.h',
                './cmd/batch_rename_tab_format_dlg.cpp',
                './cmd/batch_rename_tab_format_dlg.h',
                './cmd/batch_rename_tab_insert_dlg.cpp',
                './cmd/batch_rename_tab_insert_dlg.h',
                './cmd/batch_rename_tab_replace_dlg.cpp',
                './cmd/batch_rename_tab_replace_dlg.h',
                './cmd/batch_rename_tip_dlg.cpp',
                './cmd/batch_rename_tip_dlg.h',
                './cmd/bookmark_edit_dlg.cpp',
                './cmd/bookmark_edit_dlg.h',
                './cmd/cmd_app.cpp',
                './cmd/cmd_app.h',
                './cmd/cmd_attr_time.cpp',
                './cmd/cmd_attr_time.h',
                './cmd/cmd_bookmark.cpp',
                './cmd/cmd_bookmark.h',
                './cmd/cmd_cfg.cpp',
                './cmd/cmd_cfg.h',
                './cmd/cmd_checksum.cpp',
                './cmd/cmd_checksum.h',
                './cmd/cmd_clipboard.cpp',
                './cmd/cmd_clipboard.h',
                './cmd/cmd_column.cpp',
                './cmd/cmd_column.h',
                './cmd/cmd_create_item.cpp',
                './cmd/cmd_create_item.h',
                './cmd/cmd_drive.cpp',
                './cmd/cmd_drive.h',
                './cmd/cmd_execute.cpp',
                './cmd/cmd_execute.h',
                './cmd/cmd_file_list.cpp',
                './cmd/cmd_file_list.h',
                './cmd/cmd_file_oper.cpp',
                './cmd/cmd_file_oper.h',
                './cmd/cmd_file_property.cpp',
                './cmd/cmd_file_property.h',
                './cmd/cmd_file_scrap.cpp',
                './cmd/cmd_file_scrap.h',
                './cmd/cmd_file_split.cpp',
                './cmd/cmd_file_split.h',
                './cmd/cmd_folder_sync.cpp',
                './cmd/cmd_folder_sync.h',
                './cmd/cmd_go_path.cpp',
                './cmd/cmd_go_path.h',
                './cmd/cmd_help.cpp',
                './cmd/cmd_help.h',
                './cmd/cmd_history.cpp',
                './cmd/cmd_history.h',
                './cmd/cmd_main_gui.cpp',
                './cmd/cmd_main_gui.h',
                './cmd/cmd_network.cpp',
                './cmd/cmd_network.h',
                './cmd/cmd_pic_conv.cpp',
                './cmd/cmd_pic_conv.h',
                './cmd/cmd_pic_viewer.cpp',
                './cmd/cmd_pic_viewer.h',
                './cmd/cmd_print.cpp',
                './cmd/cmd_print.h',
                './cmd/cmd_program_ass.cpp',
                './cmd/cmd_program_ass.h',
                './cmd/cmd_recent.cpp',
                './cmd/cmd_recent.h',
                './cmd/cmd_refresh.cpp',
                './cmd/cmd_refresh.h',
                './cmd/cmd_rename.cpp',
                './cmd/cmd_rename.h',
                './cmd/cmd_search.cpp',
                './cmd/cmd_search.h',
                './cmd/cmd_search_result.cpp',
                './cmd/cmd_search_result.h',
                './cmd/cmd_select_file.cpp',
                './cmd/cmd_select_file.h',
                './cmd/cmd_send_mail.cpp',
                './cmd/cmd_send_mail.h',
                './cmd/cmd_sort.cpp',
                './cmd/cmd_sort.h',
                './cmd/cmd_style.cpp',
                './cmd/cmd_style.h',
                './cmd/cmd_tab.cpp',
                './cmd/cmd_tab.h',
                './cmd/cmd_tools.cpp',
                './cmd/cmd_tools.h',
                './cmd/cmd_tray.cpp',
                './cmd/cmd_tray.h',
                './cmd/cmd_undo.cpp',
                './cmd/cmd_undo.h',
                './cmd/cmd_window.cpp',
                './cmd/cmd_window.h',
                './cmd/cmd_working_folder.cpp',
                './cmd/cmd_working_folder.h',
                './cmd/column_set_dlg.cpp',
                './cmd/column_set_dlg.h',
                './cmd/crc_create.cpp',
                './cmd/crc_create.h',
                './cmd/crc_create_dlg.cpp',
                './cmd/crc_create_dlg.h',
                './cmd/crc_verify.cpp',
                './cmd/crc_verify.h',
                './cmd/crc_verify_dlg.cpp',
                './cmd/crc_verify_dlg.h',
                './cmd/create_item_dlg.cpp',
                './cmd/create_item_dlg.h',
                './desktop_background.cpp',
                './desktop_background.h',
                './cmd/dos_cmd_dlg.cpp',
                './cmd/dos_cmd_dlg.h',
                './cmd/drive_detail_dlg.cpp',
                './cmd/drive_detail_dlg.h',
                './cmd/drive_dlg.cpp',
                './cmd/drive_dlg.h',
                './cmd/drive_info_dlg.cpp',
                './cmd/drive_info_dlg.h',
                './cmd/file_combine.cpp',
                './cmd/file_combine.h',
                './cmd/file_combine_dlg.cpp',
                './cmd/file_combine_dlg.h',
                './cmd/file_list.cpp',
                './cmd/file_list.h',
                './cmd/file_list_dlg.cpp',
                './cmd/file_list_dlg.h',
                './cmd/file_merge.cpp',
                './cmd/file_merge.h',
                './cmd/file_merge_dlg.cpp',
                './cmd/file_merge_dlg.h',
                './cmd/file_scrap_group_dlg.cpp',
                './cmd/file_scrap_group_dlg.h',
                './cmd/file_split.cpp',
                './cmd/file_split.h',
                './cmd/file_split_dlg.cpp',
                './cmd/file_split_dlg.h',
                './cmd/folder_sync_dlg.cpp',
                './cmd/folder_sync_dlg.h',
                './cmd/folder_sync_opt_dlg.cpp',
                './cmd/folder_sync_opt_dlg.h',
                './cmd/format_parser.cpp',
                './cmd/format_parser.h',
                './cmd/go_path_dlg.cpp',
                './cmd/go_path_dlg.h',
                './cmd/history_dlg.cpp',
                './cmd/history_dlg.h',
                './cmd/input_dlg.cpp',
                './cmd/input_dlg.h',
                './cmd/multi_rename.cpp',
                './cmd/multi_rename.h',
                './cmd/param_exec_dlg.cpp',
                './cmd/param_exec_dlg.h',
                './cmd/pic_conv.cpp',
                './cmd/pic_conv.h',
                './cmd/pic_conv_color_dlg.cpp',
                './cmd/pic_conv_color_dlg.h',
                './cmd/pic_conv_dlg.cpp',
                './cmd/pic_conv_dlg.h',
                './cmd/pic_conv_opt_dlg.cpp',
                './cmd/pic_conv_opt_dlg.h',
                './cmd/proc_shared_file_dlg.cpp',
                './cmd/proc_shared_file_dlg.h',
                './cmd/program_ass_dlg.cpp',
                './cmd/program_ass_dlg.h',
                './cmd/search_loc_dlg.cpp',
                './cmd/search_loc_dlg.h',
                './cmd/search_loc_edit_dlg.cpp',
                './cmd/search_loc_edit_dlg.h',
                './cmd/sel_filter_dlg.cpp',
                './cmd/sel_filter_dlg.h',
                './cmd/sel_name_dlg.cpp',
                './cmd/sel_name_dlg.h',
                './cmd/shortcut_dlg.cpp',
                './cmd/shortcut_dlg.h',
                './cmd/single_rename_dlg.cpp',
                './cmd/single_rename_dlg.h',
                './cmd/text_merge.cpp',
                './cmd/text_merge.h',
                './cmd/text_merge_dlg.cpp',
                './cmd/text_merge_dlg.h',
                './cmd/tip_dlg.cpp',
                './cmd/tip_dlg.h',
                './cmd/title_dlg.cpp',
                './cmd/title_dlg.h',
                './cmd/trash.cpp',
                './cmd/trash.h',

                './cmd/router/cmd_command.cpp',
                './cmd/router/cmd_command.h',
                './cmd/router/cmd_command_map.cpp',
                './cmd/router/cmd_command_map.h',
                './cmd/router/cmd_context.cpp',
                './cmd/router/cmd_context.h',
                './cmd/router/cmd_executor.cpp',
                './cmd/router/cmd_executor.h',
                './cmd/router/cmd_parameter_define.h',
                './cmd/router/cmd_parameters.cpp',
                './cmd/router/cmd_parameters.h',
            ],
        },
    ],
}