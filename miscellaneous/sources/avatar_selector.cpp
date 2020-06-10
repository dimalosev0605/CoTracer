#include "./miscellaneous/headers/avatar_selector.h"

const QString file_prefix = "file://";

void Avatar_selector::handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data)
{
    jint ok_res = QAndroidJniObject::getStaticField<jint>("android/app/Activity", "RESULT_OK");
    if(receiverRequestCode == 101 && resultCode == ok_res) {
        QAndroidJniObject uri = data.callObjectMethod("getData", "()Landroid/net/Uri;");
        QAndroidJniObject data = QAndroidJniObject::getStaticObjectField("android/provider/MediaStore$MediaColumns", "DATA", "Ljava/lang/String;");
        QAndroidJniEnvironment env;
        jobjectArray projection = (jobjectArray)env->NewObjectArray(1, env->FindClass("java/lang/String"), NULL);
        jobject projection_android_data = env->NewStringUTF(data.toString().toStdString().c_str());
        env->SetObjectArrayElement(projection, 0, projection_android_data);
        QAndroidJniObject content_resolver = QtAndroid::androidActivity().callObjectMethod("getContentResolver", "()Landroid/content/ContentResolver;");
        QAndroidJniObject cursor = content_resolver.callObjectMethod("query", "(Landroid/net/Uri;[Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;)Landroid/database/Cursor;", uri.object<jobject>(), projection, NULL, NULL, NULL);
        jint column_index = cursor.callMethod<jint>("getColumnIndex", "(Ljava/lang/String;)I", data.object<jstring>());
        cursor.callMethod<jboolean>("moveToFirst", "()Z");
        QAndroidJniObject result = cursor.callObjectMethod("getString", "(I)Ljava/lang/String;", column_index);

        m_selected_image_path = result.toString();

        if(m_selected_image_path.isEmpty()) return;

        QString path_for_qml_image_item = file_prefix + m_selected_image_path;
        emit image_selected(path_for_qml_image_item);
    }
}

void Avatar_selector::select_avatar()
{
    QAndroidJniObject action_pick = QAndroidJniObject::getStaticObjectField("android/content/Intent", "ACTION_PICK", "Ljava/lang/String;");
    QAndroidJniObject external_content_uri = QAndroidJniObject::getStaticObjectField("android/provider/MediaStore$Images$Media", "EXTERNAL_CONTENT_URI", "Landroid/net/Uri;");
    QAndroidJniObject intent = QAndroidJniObject("android/content/Intent", "(Ljava/lang/String;Landroid/net/Uri;)V", action_pick.object<jstring>(), external_content_uri.object<jobject>());
    if(action_pick.isValid() && intent.isValid()) {
        intent.callObjectMethod("setType", "(Ljava/lang/String;)Landroid/content/Intent;", QAndroidJniObject::fromString("image/*").object<jstring>());
        QtAndroid::startActivity(intent.object<jobject>(), 101, this);
    }
}

QString Avatar_selector::get_selected_image_path() const
{
    return m_selected_image_path;
}
