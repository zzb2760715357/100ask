
#include <stdio.h>  
#include <jni.h> 


jint create_vm(JavaVM** jvm, JNIEnv** env) 
{  
    JavaVMInitArgs args;  
    JavaVMOption options[1];  
    args.version = JNI_VERSION_1_6;  
    args.nOptions = 1;  
    options[0].optionString = "-Djava.class.path=./";  
    args.options = options;  
    args.ignoreUnrecognized = JNI_FALSE;  
    return JNI_CreateJavaVM(jvm, (void **)env, &args);  
}  


int main(int argc, char **argv)
{
	JavaVM* jvm;
	JNIEnv* env;

	jclass cls;
	int ret = 0;

	jmethodID mid;
		
	if (create_vm(&jvm, &env)) {
		printf("can not create jvm\n");
		return -1;
	}

	cls = (*env)->FindClass(env, "Hello");
	if (cls == NULL) {
		printf("can not find hello class\n");
		ret = -1;
		goto destroy;
	}
	
	mid = (*env)->GetStaticMethodID(env, cls, "main","([Ljava/lang/String;)V");
	if (mid == NULL) {
		ret = -1;
		printf("can not get method\n");
		goto destroy;
	}

	(*env)->CallStaticVoidMethod(env, cls, mid, NULL);

destroy:

	(*jvm)->DestroyJavaVM(jvm);
	return ret;
}


