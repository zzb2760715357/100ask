public class JNIDemo{

	static {
		System.loadLibrary("native");
	}

	public native void hello();
	
	public static void main(String args[]){
		JNIDemo d = new JNIDemo();
		d.hello();
	}
} 
