public class JNIDemo{

	static {
		System.loadLibrary("native");
	}

	public native int hello(int number);
	
	public static void main(String args[]){
		JNIDemo d = new JNIDemo();
		System.out.println(d.hello(123));
	}
} 
