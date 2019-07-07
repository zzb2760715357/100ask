
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
	jmethodID cid;

	jobject jobj;
	jstring jstr;

	jfieldID nameID;
	jfieldID ageID;

	int r;
		
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

	cid = (*env)->GetMethodID(env, cls,	"<init>", "()V");
	if (cid == NULL) {
		ret = -1;
		printf("can not get constructor method");
		goto destroy;
	}

	jobj = (*env)->NewObject(env, cls, cid);
	if (jobj == NULL) {
		ret = -1;
		printf("can not create object");
		goto destroy;
	}

	nameID = (*env)->GetFieldID(env, cls, "name", "Ljava/lang/String;");
	if (nameID == NULL) {
		ret = -1;
		printf("can not get field name");
		goto destroy;
	}
	jstr = (*env)->NewStringUTF(env, "Bill");
	(*env)->SetObjectField(env, jobj, nameID, jstr);

	ageID = (*env)->GetFieldID(env, cls, "age", "I");
	if (ageID == NULL) {
		ret = -1;
		printf("can not get field age");
		goto destroy;
	}
	(*env)->SetIntField(env, jobj, ageID, 10);

	mid = (*env)->GetMethodID(env, cls, "sayhello_to","(Ljava/lang/String;)I");
	if (mid == NULL) {
		ret = -1;
		printf("can not get method\n");
		goto destroy;
	}

	jstr = (*env)->NewStringUTF(env, "weidongshan@qq.com");

	r = (*env)->CallIntMethod(env, jobj, mid, jstr);
	printf("ret = %d\n", r);

destroy:

	(*jvm)->DestroyJavaVM(jvm);
	return ret;
}

