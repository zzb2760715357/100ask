#include <jni.h>
#include <stdio.h>


jint c_hello(JNIEnv *env, jobject cls,jint num)
{
	printf("JNI Hello World! %d\r\n",num);

	return 520;
}


static const JNINativeMethod methods[]={
	{"hello","(I)I",c_hello}
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

