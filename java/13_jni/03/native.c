#include <jni.h>
#include <stdio.h>


jstring c_hello(JNIEnv *env, jobject cls,jstring str )
{
	const jbyte *cstr;
	cstr = (*env)->GetStringUTFChars(env, str, NULL);
	if (cstr == NULL) {
		return NULL; 
	}

	printf("JNI get string from java:%s\r\n",cstr);
	(*env)->ReleaseStringUTFChars(env, str, cstr);

	return (*env)->NewStringUTF(env, "JNI return value");
}


static const JNINativeMethod methods[]={
	{"hello","(Ljava/lang/String;)Ljava/lang/String;",(void *)c_hello}
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved)
{
	JNIEnv *env;
	jclass cls;
	if ((*jvm)->GetEnv(jvm, (void **)&env, JNI_VERSION_1_4)) {
		return JNI_ERR; 
	}
	cls = (*env)->FindClass(env, "JNIDemo");
	if (cls == NULL) {
		return JNI_ERR;
	}
	
	if ((*env)->RegisterNatives(env,cls,methods,sizeof(methods)/sizeof(methods[0]))<0)
		return JNI_ERR;
	
	return JNI_VERSION_1_4;
}

