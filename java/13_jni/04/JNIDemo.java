public class JNIDemo{

	static {
		System.loadLibrary("native");
	}

	public native int hello(int[] a);
	
	public static void main(String args[]){
		JNIDemo d = new JNIDemo();

		int [] a = {1,2,3};
		
		System.out.println(d.hello(a));
	}
} 
