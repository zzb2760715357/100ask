public class JNIDemo{

	static {
		System.loadLibrary("native");
	}

	public native String hello(String str);
	
	public static void main(String args[]){
		JNIDemo d = new JNIDemo();
		System.out.println(d.hello("I am frome java"));
	}
} 
