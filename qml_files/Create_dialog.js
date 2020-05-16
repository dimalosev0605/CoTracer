function create_dialog(parent,
                       z,
                       message,
                       opacity_anim_duration,
                       is_busy_indicator_running,
                       is_opacity_anim_running,
                       is_destroy) {
    var comp = Qt.createComponent("./My_dialog.qml");
    var obj = comp.createObject(parent,
                                {
                                    z: z,
                                    message: message,
                                    opacity_anim_duration: opacity_anim_duration,
                                    is_busy_indicator_running: is_busy_indicator_running,
                                    is_opacity_anim_running: is_opacity_anim_running,
                                    is_destroy: is_destroy
                                }
                                )
}
