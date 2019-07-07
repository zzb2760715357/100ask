#include <jni.h>
#include <stdio.h>


jint c_hello(JNIEnv *env, jobject cls,jintArray arr )
{
	jint *carr;
	jint i, sum = 0;
	carr = (*env)->GetIntArrayElements(env, arr, NULL);
	if (carr == NULL) {
		return 0; /* exception occurred */
	}
	for (i=0; i< (*env)->GetArrayLength(env, arr); i++) {
		sum += carr[i];
	}
	(*env)->ReleaseIntArrayElements(env, arr, carr, 0);
	return sum;
}


static const JNINativeMethod methods[]={
	{"hello","([I)I",(void *)c_hello}
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

