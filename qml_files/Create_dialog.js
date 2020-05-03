function create_dialog(parent, z, message, opacity_anim_duration, is_busy_running, is_static) {
    var comp = Qt.createComponent("./My_dialog.qml");
    var obj = comp.createObject(parent,
                                {
                                    z: z,
                                    message: message,
                                    opacity_anim_duration: opacity_anim_duration,
                                    is_busy_running: is_busy_running,
                                    is_static: is_static
                                }
                                )
}
