public class JNIDemo{

	static {
		System.loadLibrary("native");
	}

	public native int[] hello(int[] a);
	
	public static void main(String args[]){
		JNIDemo d = new JNIDemo();

		int [] a = {1,2,3};
		int [] b = null;
		int i;

		b = d.hello(a);

		for(i = 0;i < b.length;i ++)
			System.out.println(b[i]);
	}
} 
